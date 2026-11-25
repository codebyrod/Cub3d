#!/usr/bin/env bash
# =============================================================================
#  run_tests.sh - bateria de testes do Cub3D
#
#  Uso (na raiz do projeto, onde esta o Makefile):
#      ./run_tests.sh            roda todos os blocos
#      ./run_tests.sh 2 4 7      roda so os blocos 2, 4 e 7
#      ./run_tests.sh -l         lista os blocos
#
#  Variaveis de ambiente:
#      BIN=./cub3D        binario a testar (padrao: ./cub3D ou ./cub3d)
#      NO_VALGRIND=1      nao usa valgrind
#      SKIP_BUILD=1       pula o bloco 1 (nao roda make fclean)
#      USE_REAL_DISPLAY=1 usa o seu DISPLAY em vez do Xvfb
#      SMOKE=2            segundos que um mapa valido deve ficar rodando
#      TMO=20             timeout das execucoes que devem terminar sozinhas
#      BIGMAP=1           inclui o mapa de 1000x1000 no bloco 14
#      RSS_SECS=20        duracao da medicao de memoria no bloco 16
#
#  Dependencias opcionais (o que faltar vira SKIP, nao FAIL):
#      valgrind, Xvfb (+xdpyinfo), xdotool, norminette
#
#  Tudo que o script gera fica em tests_cub3d/ (mapas, texturas, logs).
#  Mapas para jogar na mao: tests_cub3d/maps/manual/
# =============================================================================

ROOT=$(pwd)
T=tests_cub3d
FIX=$T/maps
TEX=$T/textures
WORK=$T/work
LOGS=$T/logs
SMOKE=${SMOKE:-2}
TMO=${TMO:-20}
XDISP=${XDISP:-:99}

if [ -t 1 ]; then
	G=$'\e[32m'; R=$'\e[31m'; Y=$'\e[33m'; C=$'\e[36m'; N=$'\e[0m'
else
	G=''; R=''; Y=''; C=''; N=''
fi

PASS=0; FAIL=0; SKIP=0; WARN=0
FAILED=(); MANUAL=()
CUR=0
RC=0
ENVP=()
BIN=${BIN:-}
XVFB_PID=""

ALL_BLOCKS=(1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18)
declare -A BNAME=(
	[1]="Build"
	[2]="Argumentos e nome do arquivo"
	[3]="O arquivo em si"
	[4]="Identificadores e linhas de config"
	[5]="Cores"
	[6]="Caminhos de textura (parser)"
	[7]="Mapa: caracteres, formato e fechamento"
	[8]="Saida do parser (manual)"
	[9]="Carga das texturas no jogo"
	[10]="Spawn e primeiro frame"
	[11]="Render (manual)"
	[12]="Movimento e rotacao (manual)"
	[13]="Colisao (manual)"
	[14]="Mapas de formato extremo"
	[15]="Ambiente do mlx"
	[16]="Saida e memoria"
	[17]="Stress do parser"
	[18]="Regressao e limpeza final"
)

# ----------------------------------------------------------------------------
#  Saida
# ----------------------------------------------------------------------------
ok()   { PASS=$((PASS+1)); printf '  %s[ OK ]%s %s\n' "$G" "$N" "$1"; }
skip() { SKIP=$((SKIP+1)); printf '  %s[SKIP]%s %s%s\n' "$Y" "$N" "$1" "${2:+  ($2)}"; }
warn() { WARN=$((WARN+1)); printf '  %s[WARN]%s %s\n' "$Y" "$N" "$1"; }
info() { printf '  %s[INFO]%s %s\n' "$C" "$N" "$1"; }
manual() { MANUAL+=("B$CUR: $1"); }

save_logs() {
	local n=${1//[^A-Za-z0-9._-]/_}
	mkdir -p "$LOGS"
	cp "$T/out" "$LOGS/B$CUR.$n.out" 2>/dev/null
	cp "$T/err" "$LOGS/B$CUR.$n.err" 2>/dev/null
	cp "$T/vg.log" "$LOGS/B$CUR.$n.vg" 2>/dev/null
}

fail() {
	FAIL=$((FAIL+1))
	printf '  %s[FAIL]%s %s\n' "$R" "$N" "$1"
	[ -n "${2:-}" ] && printf '         %s\n' "$2"
	FAILED+=("B$CUR: $1")
	save_logs "$1"
}

rc_label() {
	case $1 in
		0)   echo "exit 0 (aceitou / terminou normal)" ;;
		1)   echo "exit 1" ;;
		124) echo "timeout (ainda rodando)" ;;
		134) echo "ABORT (double free / assert)" ;;
		139) echo "SEGFAULT" ;;
		42)  echo "valgrind: erro de memoria ou leak" ;;
		*)   echo "exit $1" ;;
	esac
}

# ----------------------------------------------------------------------------
#  Execucao
# ----------------------------------------------------------------------------
VG_OPTS=(--leak-check=full --show-leak-kinds=all --errors-for-leak-kinds=all
	--track-fds=yes --error-exitcode=42)

# run [-n] args...   (-n = sem valgrind).  Resultado em $RC, $T/out, $T/err, $T/vg.log
run() {
	local use_vg=$USE_VG
	if [ "${1:-}" = "-n" ]; then use_vg=0; shift; fi
	: >"$T/vg.log"
	if [ "$use_vg" = 1 ]; then
		timeout "$TMO" "${ENVP[@]}" valgrind -q "${VG_OPTS[@]}" \
			--log-file="$T/vg.log" "$BIN" "$@" >"$T/out" 2>"$T/err"
	else
		timeout "$TMO" "${ENVP[@]}" "$BIN" "$@" >"$T/out" 2>"$T/err"
	fi
	RC=$?
}

# fds abertos que o programa nao fechou (ignora os herdados do shell)
fd_leaks() {
	awk '/Open file descriptor/ {want=1; next}
	     want==1 { if ($0 !~ /inherited/) n++; want=0 }
	     END { print n+0 }' "$T/vg.log" 2>/dev/null
}

need_gui() {
	[ "$HAVE_GUI" = 1 ] && return 0
	skip "$1" "sem Xvfb/DISPLAY"
	return 1
}

# Confere "erro limpo": exit 1, 'Error' na 1a linha do stderr, mensagem na 2a,
# nada de 'Error' em stdout, sem leak, sem fd aberto.
chk_error() {
	local name=$1 why="" l1 l2
	case $RC in
		1) ;;
		0)   why="aceitou o arquivo (exit 0)" ;;
		124) why="nao terminou: provavelmente abriu a janela" ;;
		139) why="SEGFAULT" ;;
		134) why="ABORT (double free / assert)" ;;
		42)  why="valgrind achou erro ou leak" ;;
		*)   why="exit=$RC (esperado 1)" ;;
	esac
	if [ "$RC" -eq 1 ]; then
		l1=$(head -n1 "$T/err"); l2=$(sed -n 2p "$T/err")
		[ "$l1" = "Error" ] || why="${why:+$why; }1a linha de stderr='${l1:0:40}' (esperado 'Error')"
		[ -n "$l2" ] || why="${why:+$why; }sem mensagem na 2a linha"
		grep -q '^Error' "$T/out" 2>/dev/null && why="${why:+$why; }'Error' foi para stdout"
	fi
	if [ "$USE_VG" = 1 ] && [ "$(fd_leaks)" -gt 0 ]; then
		why="${why:+$why; }fd aberto sem close (--track-fds)"
	fi
	if [ -z "$why" ]; then ok "$name"; else fail "$name" "$why"; fi
}

expect_error() { local name=$1; shift; run "$@"; chk_error "$name"; }

expect_error_gui() {
	local name=$1; shift
	need_gui "$name" || return
	TMO=60 run "$@"
	chk_error "$name"
}

# mapa valido: tem que ficar rodando ate o timeout (exit 124)
expect_alive() {
	local name=$1; shift
	need_gui "$name" || return
	TMO=${ALIVE_T:-$SMOKE} run -n "$@"
	if [ "$RC" -eq 124 ]; then
		ok "$name"
	else
		fail "$name" "$(rc_label $RC); stderr: $(head -c 120 "$T/err" | tr '\n' ' ')"
	fi
}

# so exige: nao crashar (aceita erro limpo ou rodando)
expect_safe() {
	local name=$1; shift
	TMO=$SMOKE run -n "$@"
	case $RC in
		1|124) ok "$name  (exit $RC)" ;;
		*) fail "$name" "$(rc_label $RC)" ;;
	esac
}

# ----------------------------------------------------------------------------
#  Geracao de fixtures
# ----------------------------------------------------------------------------
rep() { head -c "$2" /dev/zero | tr '\0' "$1"; }   # rep CHAR N

# mk_xpm nome RRGGBB largura altura
mk_xpm() {
	local f="$TEX/t_$1.xpm" w=$3 h=$4 row i
	row=$(rep a "$w")
	{
		echo '/* XPM */'; echo "static char *t_$1[] = {"
		echo "\"$w $h 1 1\","; echo "\"a c #$2\","
		for ((i = 1; i < h; i++)); do echo "\"$row\","; done
		echo "\"$row\""; echo '};'
	} >"$f"
}

# metade vermelha, metade azul: serve para ver espelhamento
mk_xpm_half() {
	local f="$TEX/t_half.xpm" row i
	row="$(rep a 32)$(rep b 32)"
	{
		echo '/* XPM */'; echo 'static char *t_half[] = {'
		echo '"64 64 2 1",'; echo '"a c #FF0000",'; echo '"b c #0000FF",'
		for ((i = 1; i < 64; i++)); do echo "\"$row\","; done
		echo "\"$row\""; echo '};'
	} >"$f"
}

reset_hdr() {
	H_NO="NO ./$TEX/t_n.xpm"; H_SO="SO ./$TEX/t_s.xpm"
	H_WE="WE ./$TEX/t_w.xpm"; H_EA="EA ./$TEX/t_e.xpm"
	H_F="F 220,100,0";        H_C="C 225,30,0"
}
hdr() { printf '%s\n' "$H_NO" "$H_SO" "$H_WE" "$H_EA" "$H_F" "$H_C"; }

MAP=$'111111\n100001\n1000N1\n111111'

# mk caminho [mapa]   -> escreve $FIX/caminho com o cabecalho atual
mk() {
	local f="$FIX/$1.cub" map
	if [ $# -ge 2 ]; then map=$2; else map=$MAP; fi
	mkdir -p "$(dirname "$f")"
	{ hdr; printf '%s\n' "$map"; } >"$f"
}

# gen_room largura altura -> sala fechada, spawn N em (1,1)
gen_room() {
	local w=$1 h=$2 mid
	rep 1 "$w"; echo
	echo "1N$(rep 0 $((w - 3)))1"
	mid="1$(rep 0 $((w - 2)))1"
	[ "$h" -gt 3 ] && yes "$mid" | head -n $((h - 3))
	rep 1 "$w"; echo
}

gen_fixtures() {
	rm -rf "$FIX" "$WORK"
	mkdir -p "$FIX/good" "$FIX/bad" "$FIX/manual" "$TEX" "$WORK" "$LOGS"
	mk_xpm n FF0000 64 64; mk_xpm s 00FF00 64 64
	mk_xpm w 0000FF 64 64; mk_xpm e FFFF00 64 64
	mk_xpm_half
	mk_xpm big   FF00FF 128 128; mk_xpm small 00FFFF 16 16
	mk_xpm tall  FF8800 64 128;  mk_xpm one   FFFFFF 1 1
	mk_xpm huge  8800FF 1024 1024
	cp "$TEX/t_n.xpm" "$TEX/minha pedra.xpm"
	reset_hdr

	# ---- arquivos de bloco 3
	mk good/base
	: >"$FIX/bad/arq_vazio.cub"
	printf '\n\n  \n\t\n' >"$FIX/bad/arq_so_branco.cub"
	head -c 4096 /dev/urandom >"$FIX/bad/arq_random.cub"
	{ hdr; printf '11\0001\n'; printf '%s\n' "$MAP"; } >"$FIX/bad/arq_nul.cub"
	{ printf '\xEF\xBB\xBF'; cat "$FIX/good/base.cub"; } >"$FIX/bad/arq_bom.cub"
	sed 's/$/\r/' "$FIX/good/base.cub" >"$FIX/bad/arq_crlf.cub"
	{ hdr; printf '%s' "$MAP"; } >"$FIX/good/arq_sem_nl_final.cub"
	{ hdr; rep 1 200000; echo; } >"$FIX/bad/arq_linha_enorme.cub"

	# ---- bloco 4: identificadores
	for id in NO SO WE EA; do ( eval "H_$id=''"; mk "bad/cfg_falta_$id" ); done
	( H_F='';  mk bad/cfg_falta_F )
	( H_C='';  mk bad/cfg_falta_C )
	( H_SO="$H_NO";        mk bad/cfg_NO_duplicado )
	( H_C='F 1,2,3';       mk bad/cfg_F_duplicado )
	( H_NO="no ${H_NO#NO }";  mk bad/cfg_id_minusculo )
	( H_NO="NOO ${H_NO#NO }"; mk bad/cfg_id_NOO )
	( H_NO="XX ${H_NO#NO }";  mk bad/cfg_id_desconhecido )
	( H_NO='# comentario';    mk bad/cfg_comentario )
	( H_NO='NO';              mk bad/cfg_NO_sem_valor )
	( H_NO='NO    ';          mk bad/cfg_NO_so_espacos )
	( H_NO="$H_NO extra";     mk bad/cfg_valor_com_lixo )
	{ hdr; echo "$H_NO"; printf '%s\n' "$MAP"; } >"$FIX/bad/cfg_setimo_id.cub"
	( H_NO=$'NO\t'"./$TEX/t_n.xpm";         mk good/cfg_tab )
	( H_NO="   $H_NO";                      mk good/cfg_espacos_antes )
	( H_NO="$H_NO    ";                     mk good/cfg_espacos_fim )
	( H_NO="NO      ./$TEX/t_n.xpm";        mk good/cfg_varios_espacos )
	{ printf '%s\n' "$H_F" "$H_C" "$H_EA" "$H_WE" "$H_SO" "$H_NO"; printf '%s\n' "$MAP"; } \
		>"$FIX/good/cfg_ordem_livre.cub"
	{ for l in "$H_NO" "$H_SO" "$H_WE" "$H_EA" "$H_F" "$H_C"; do printf '%s\n\n' "$l"; done
	  printf '%s\n' "$MAP"; } >"$FIX/good/cfg_linhas_vazias.cub"

	# ---- bloco 5: cores
	BAD_COL=('256,0,0' '0,256,0' '0,0,256' '-1,0,0' '1,2' '1,2,3,4' '1,,3' ',1,2'
		'a,b,c' '1.5,2,3' '1;2;3' '220,100,0,' '99999999999,0,0' '1,2,3abc')
	GOOD_COL=('0,0,0' '255,255,255' '220, 100, 0' '220 ,100 , 0' '010,020,030')
	local i
	for i in "${!BAD_COL[@]}"; do
		( H_F="F ${BAD_COL[$i]}"; mk "bad/cor_F_$i" )
		( H_C="C ${BAD_COL[$i]}"; mk "bad/cor_C_$i" )
	done
	( H_F='F';  mk bad/cor_F_sem_valor )
	for i in "${!GOOD_COL[@]}"; do
		( H_F="F ${GOOD_COL[$i]}"; H_C="C ${GOOD_COL[$i]}"; mk "good/cor_$i" )
	done
	( H_F='F 255,0,0'; H_C='C 0,0,255'; mk manual/cores_vermelho_azul )

	# ---- bloco 6: texturas
	( H_NO="NO ./$TEX/nao_existe.xpm"; mk bad/tex_inexistente_NO )
	( H_SO="SO ./$TEX/nao_existe.xpm"; mk bad/tex_inexistente_SO )
	( H_WE="WE ./$TEX/nao_existe.xpm"; mk bad/tex_inexistente_WE )
	( H_EA="EA ./$TEX/nao_existe.xpm"; mk bad/tex_inexistente_EA )
	( H_NO="NO ./$TEX";                mk bad/tex_diretorio )
	cp "$TEX/t_n.xpm" "$WORK/semperm.xpm"; chmod 000 "$WORK/semperm.xpm"
	( H_NO="NO ./$WORK/semperm.xpm";   mk bad/tex_sem_permissao )
	( H_NO="NO $ROOT/$TEX/t_n.xpm";    mk good/tex_absoluto )
	( H_NO="NO ./$TEX/minha pedra.xpm"; mk good/tex_com_espaco )
	( H_NO="NO ./$TEX/../textures/t_n.xpm"; mk good/tex_com_pontos )
	( H_SO="SO ./$TEX/t_n.xpm"; H_WE="WE ./$TEX/t_n.xpm"; H_EA="EA ./$TEX/t_n.xpm"; mk good/tex_mesma_4_faces )

	# ---- bloco 7: mapas
	mk bad/map_sem_spawn      $'111111\n100001\n100001\n111111'
	mk bad/map_dois_spawns    $'111111\n1N00S1\n100001\n111111'
	mk bad/map_tres_spawns    $'111111\n1N0ES1\n100001\n111111'
	mk bad/map_aberto_dir     $'111111\n100000\n1000N1\n111111'
	mk bad/map_aberto_topo    $'110111\n100001\n1000N1\n111111'
	mk bad/map_aberto_esq     $'111111\n000001\n1000N1\n111111'
	mk bad/map_aberto_base    $'111111\n100001\n1000N1\n111011'
	mk bad/map_buraco_meio    $'111111\n100 01\n1000N1\n111111'
	mk bad/map_spawn_borda    $'1N1111\n100001\n100001\n111111'
	mk bad/map_regiao_aberta  $'1111111\n1N00001\n1111111\n0000000\n1111111'
	mk bad/map_char_invalido  $'111111\n100201\n1000N1\n111111'
	mk bad/map_minusculo      $'111111\n100001\n1000n1\n111111'
	mk bad/map_tab_no_mapa    $'111111\n10\t001\n1000N1\n111111'
	mk bad/map_linha_vazia    $'111111\n100001\n\n1000N1\n111111'
	mk bad/map_linha_espacos  $'111111\n100001\n   \n1000N1\n111111'
	mk bad/map_texto_apos     $'111111\n100001\n1000N1\n111111\n\nlixo'
	mk bad/map_ultima_aberta  $'111111\n100001\n1000N0'
	hdr >"$FIX/bad/map_ausente.cub"
	{ printf '%s\n' "$MAP"; hdr; } >"$FIX/bad/map_antes_config.cub"

	mk good/map_minimo_3x3     $'111\n1N1\n111'
	mk good/map_irregular      $'11111111\n1000001\n1N0001\n111111'
	mk good/map_indentado      $'  1111\n  1N01\n  1111'
	mk good/map_vazio_interno  $'11111111\n1N000001\n10111101\n101 1101\n11111111'
	mk good/map_corredor       $'11111\n1N001\n11111'
	mk good/map_varias_salas   $'1111111111\n1N00100001\n1000100001\n1000000001\n1111111111'
	{ hdr; printf '%s\n\n\n\n' "$MAP"; } >"$FIX/good/map_linhas_apos.cub"
	{ hdr; printf '%s\n' "$MAP" | sed 's/$/   /'; } >"$FIX/good/map_espacos_fim_linha.cub"
	mk good/spawn_N "$MAP"
	mk good/spawn_S "${MAP/N/S}"
	mk good/spawn_E "${MAP/N/E}"
	mk good/spawn_W "${MAP/N/W}"
	mk good/spawn_encostado   $'11111\n1N001\n11111'

	# ---- bloco 9: texturas no jogo
	cp "$FIX/good/base.cub" "$WORK/nao_xpm.xpm"
	: >"$WORK/vazio.xpm"
	head -c 200 "$TEX/t_n.xpm" >"$WORK/truncado.xpm"
	for f in NO SO WE EA; do
		for k in nao_xpm vazio truncado; do
			( eval "H_$f=\"$f ./$WORK/$k.xpm\""; mk "bad/load_${f}_$k" )
		done
	done
	( H_NO="NO ./$TEX/t_big.xpm"; H_SO="SO ./$TEX/t_small.xpm"
	  H_WE="WE ./$TEX/t_tall.xpm"; H_EA="EA ./$TEX/t_one.xpm"; mk good/load_tamanhos_diferentes )
	( H_NO="NO ./$TEX/t_huge.xpm"; mk good/load_textura_1024 )
	( H_NO="NO ./$TEX/t_tall.xpm"; H_SO="SO ./$TEX/t_tall.xpm"
	  H_WE="WE ./$TEX/t_tall.xpm"; H_EA="EA ./$TEX/t_tall.xpm"; mk good/load_nao_quadrada )
	( H_NO="NO ./$TEX/t_one.xpm"; H_SO="SO ./$TEX/t_one.xpm"
	  H_WE="WE ./$TEX/t_one.xpm"; H_EA="EA ./$TEX/t_one.xpm"; mk good/load_1x1 )

	# ---- bloco 14: mapas extremos
	local d
	for d in 50x50 100x5 5x100 200x200 500x500 120x6; do
		mk "good/big_$d" "$(gen_room "${d%x*}" "${d#*x}")"
	done
	[ "${BIGMAP:-0}" = 1 ] && mk good/big_1000x1000 "$(gen_room 1000 1000)"

	# ---- manuais (para jogar na mao)
	mk manual/spawn_N "$MAP"; mk manual/spawn_S "${MAP/N/S}"
	mk manual/spawn_E "${MAP/N/E}"; mk manual/spawn_W "${MAP/N/W}"
	mk manual/arena "$(gen_room 12 12)"
	( H_NO="NO ./$TEX/t_half.xpm"; H_SO="SO ./$TEX/t_half.xpm"
	  H_WE="WE ./$TEX/t_half.xpm"; H_EA="EA ./$TEX/t_half.xpm"
	  mk manual/espelhamento "$(gen_room 12 12)" )
	mk manual/quina_diagonal $'11111111\n1N000001\n10000001\n10001001\n10000101\n10000001\n11111111'
	mk manual/corredor_longo "$(gen_room 120 3)"
	mk manual/sala_minima $'111\n1N1\n111'
}

# ----------------------------------------------------------------------------
#  Ambiente
# ----------------------------------------------------------------------------
setup_env() {
	HAVE_VG=0; command -v valgrind >/dev/null 2>&1 && HAVE_VG=1
	USE_VG=$HAVE_VG; [ "${NO_VALGRIND:-0}" = 1 ] && USE_VG=0
	HAVE_XDO=0; command -v xdotool >/dev/null 2>&1 && HAVE_XDO=1
	HAVE_GUI=0
	if [ "${USE_REAL_DISPLAY:-0}" != 1 ] && command -v Xvfb >/dev/null 2>&1; then
		Xvfb "$XDISP" -screen 0 1280x1024x24 >/dev/null 2>&1 &
		XVFB_PID=$!
		export DISPLAY=$XDISP
		local i
		for i in $(seq 1 25); do
			if command -v xdpyinfo >/dev/null 2>&1; then
				xdpyinfo >/dev/null 2>&1 && { HAVE_GUI=1; break; }
			else
				sleep 1
				kill -0 "$XVFB_PID" 2>/dev/null && { HAVE_GUI=1; break; }
			fi
			sleep 0.3
		done
	elif [ -n "${DISPLAY:-}" ]; then
		HAVE_GUI=1
	fi
	info "valgrind=$USE_VG  gui=$HAVE_GUI (DISPLAY=${DISPLAY:-none})  xdotool=$HAVE_XDO"
	[ "$USE_VG" = 1 ]  || info "sem valgrind: testes de leak/fd viram verificacao de exit code apenas"
	[ "$HAVE_GUI" = 1 ] || info "sem GUI: testes que abrem janela serao SKIP"
}

cleanup() {
	chmod 644 "$WORK"/semperm* 2>/dev/null
	[ -n "$XVFB_PID" ] && kill "$XVFB_PID" 2>/dev/null
}
trap cleanup EXIT INT TERM

find_bin() {
	local c
	for c in "$BIN" ./cub3D ./cub3d; do
		[ -n "$c" ] && [ -x "$c" ] && { BIN=$c; ABS_BIN="$ROOT/${BIN#./}"; return 0; }
	done
	return 1
}

# ----------------------------------------------------------------------------
#  Sessao grafica (xdotool): abre sob valgrind, executa acoes, fecha, confere
#  acoes: esc | sleep:N | hold:TECLA:SEGUNDOS | down:TECLA
# ----------------------------------------------------------------------------
gui_session() {
	local name=$1 map=$2 a k dur i pid wid=""
	shift 2
	need_gui "$name" || return
	[ "$HAVE_XDO" = 1 ] || { skip "$name" "sem xdotool"; return; }
	[ "$USE_VG" = 1 ]   || { skip "$name" "sem valgrind"; return; }
	: >"$T/vg.log"
	valgrind -q "${VG_OPTS[@]}" --log-file="$T/vg.log" "$BIN" "$map" >"$T/out" 2>"$T/err" &
	pid=$!
	for i in $(seq 1 120); do
		wid=$(xdotool search --name '^Cub3D$' 2>/dev/null | head -1)
		[ -n "$wid" ] && break
		kill -0 "$pid" 2>/dev/null || break
		sleep 0.5
	done
	if [ -z "$wid" ]; then
		kill -9 "$pid" 2>/dev/null; wait "$pid" 2>/dev/null
		fail "$name" "a janela 'Cub3D' nao abriu"; return
	fi
	sleep 1
	for a in "$@"; do
		case $a in
			esc)     xdotool windowfocus "$wid" 2>/dev/null; xdotool key Escape 2>/dev/null ;;
			sleep:*) sleep "${a#sleep:}" ;;
			down:*)  xdotool windowfocus "$wid" 2>/dev/null; xdotool keydown "${a#down:}" 2>/dev/null ;;
			hold:*)  k=${a#hold:}; dur=${k#*:}; k=${k%%:*}
			         xdotool windowfocus "$wid" 2>/dev/null
			         xdotool keydown "$k" 2>/dev/null; sleep "$dur"; xdotool keyup "$k" 2>/dev/null ;;
		esac
	done
	for i in $(seq 1 80); do kill -0 "$pid" 2>/dev/null || break; sleep 0.5; done
	if kill -0 "$pid" 2>/dev/null; then
		kill -9 "$pid" 2>/dev/null; wait "$pid" 2>/dev/null
		fail "$name" "o programa nao fechou"; return
	fi
	wait "$pid"; RC=$?
	xdotool keyup w a s d Left Right 2>/dev/null
	local why=""
	[ "$RC" -eq 0 ] || why="$(rc_label $RC)"
	[ "$(fd_leaks)" -eq 0 ] || why="${why:+$why; }fd aberto sem close"
	if [ -n "$why" ]; then
		fail "$name" "$why; $(head -c 200 "$T/vg.log" | tr '\n' ' ')"
	else
		ok "$name"
	fi
}

# ============================================================================
#  BLOCOS
# ============================================================================
b1() {
	if [ "${SKIP_BUILD:-0}" = 1 ]; then skip "bloco de build" "SKIP_BUILD=1"; return; fi
	local n h rc
	make fclean >/dev/null 2>&1
	make >"$T/make.log" 2>&1; rc=$?
	if [ $rc -eq 0 ]; then ok "make fclean && make compila"
	else fail "make falhou" "$(tail -3 "$T/make.log" | tr '\n' ' ')"; return; fi
	if grep -qi 'warning' "$T/make.log"; then
		fail "make emitiu warnings" "$(grep -i warning "$T/make.log" | head -2 | tr '\n' ' ')"
	else ok "sem warnings"; fi
	n=$(make -n 2>&1 | grep -c -E '(^|[ /])(cc|gcc|clang) ')
	[ "$n" -eq 0 ] && ok "make repetido nao recompila nem relinka" \
		|| fail "make repetido executa $n comandos de compilacao (relink?)"
	h=$(find . -name cub3d.h -not -path './minilibx*' -not -path "./$T/*" | head -1)
	if [ -n "$h" ]; then
		touch "$h"
		n=$(make -n 2>&1 | grep -c -E '(^|[ /])(cc|gcc|clang) ')
		[ "$n" -gt 0 ] && ok "alterar $h reconstroi" \
			|| warn "alterar $h NAO reconstroi (o Makefile nao rastreia headers: use make re)"
		make >/dev/null 2>&1
	fi
	make re >"$T/make.log" 2>&1 && ok "make re" || fail "make re falhou"
	make clean >/dev/null 2>&1 && make clean >/dev/null 2>&1 && ok "make clean (duas vezes seguidas)" \
		|| fail "make clean falhou"
	make fclean >/dev/null 2>&1 && make fclean >/dev/null 2>&1 && ok "make fclean (duas vezes seguidas)" \
		|| fail "make fclean falhou"
	make >/dev/null 2>&1 || { fail "rebuild apos fclean falhou"; return; }
	if [ -x ./cub3D ]; then ok "binario se chama cub3D"
	else warn "binario nao se chama 'cub3D' (o subject pede esse nome)"; fi
}

b2() {
	local g="$FIX/good/base.cub" f
	expect_error "sem argumento"
	run; [ ! -s "$T/out" ] && ok "sem argumento: nada em stdout" || fail "sem argumento: escreveu em stdout"
	expect_error "dois argumentos" "$g" "$g"
	expect_error "tres argumentos" a b c
	expect_error "argumento vazio" ""
	expect_error "arg 'ab' (menor que 4 chars)" ab
	expect_error "arg 'a'" a
	expect_error "arg '.cub' (so a extensao)" .cub
	expect_error "arg 'cub'" cub
	for f in mapa.txt mapa.cub.txt mapacub mapa.cub. mapa.CUB "mapa.cub " ab; do
		cp "$g" "$WORK/$f"
		expect_error "extensao invalida: '$f' (arquivo valido)" "$WORK/$f"
	done
	expect_error "arg '-h'" -h
	expect_error "arg '--help'" --help
	cp "$g" "$WORK/mapa.cub.cub"
	expect_alive "mapa.cub.cub (termina em .cub)" "$WORK/mapa.cub.cub"
	expect_alive "caminho relativo com ./" "./$g"
	expect_alive "caminho absoluto" "$ROOT/$g"
	expect_alive "caminho com .." "$FIX/../$(basename "$FIX")/good/base.cub"
	cp "$g" "$WORK/meu mapa.cub"
	expect_alive "caminho com espaco" "$WORK/meu mapa.cub"
	run; [ -s "$T/err" ] && ok "mensagem de uso vai para stderr" || fail "mensagem de uso nao foi para stderr"
}

b3() {
	local g="$FIX/good/base.cub" abs
	expect_error "arquivo inexistente" nao_existe.cub
	mkdir -p "$WORK/dir.cub"
	TMO=10 run "$WORK/dir.cub"; chk_error "diretorio chamado dir.cub"
	cp "$g" "$WORK/semperm.cub"; chmod 000 "$WORK/semperm.cub"
	if [ "$(id -u)" = 0 ]; then skip "arquivo sem permissao" "rodando como root"
	else expect_error "arquivo sem permissao" "$WORK/semperm.cub"; fi
	chmod 644 "$WORK/semperm.cub"
	ln -sf "$ROOT/$g" "$WORK/link.cub"
	expect_alive "symlink para mapa valido" "$WORK/link.cub"
	expect_error "arquivo vazio" "$FIX/bad/arq_vazio.cub"
	expect_error "so linhas vazias e espacos" "$FIX/bad/arq_so_branco.cub"
	expect_error "lixo binario aleatorio" "$FIX/bad/arq_random.cub"
	expect_safe  "byte NUL no mapa (sem crash)" "$FIX/bad/arq_nul.cub"
	expect_safe  "BOM UTF-8 (sem crash)" "$FIX/bad/arq_bom.cub"
	expect_safe  "CRLF (sem crash)" "$FIX/bad/arq_crlf.cub"
	expect_alive "arquivo sem \\n no final" "$FIX/good/arq_sem_nl_final.cub"
	TMO=40 run "$FIX/bad/arq_linha_enorme.cub"; chk_error "linha de 200 mil caracteres"
	abs="$ROOT/$g"
	( cd /tmp && timeout "$TMO" "$ABS_BIN" "$abs" >"$ROOT/$T/out" 2>"$ROOT/$T/err" ); RC=$?
	: >"$T/vg.log"
	chk_error "rodar de outra pasta (texturas relativas: erro limpo)"
}

b4() {
	local f
	for f in "$FIX"/bad/cfg_*.cub; do expect_error "$(basename "$f" .cub)" "$f"; done
	for f in "$FIX"/good/cfg_*.cub; do expect_alive "aceita: $(basename "$f" .cub)" "$f"; done
}

b5() {
	local f
	for f in "$FIX"/bad/cor_*.cub; do expect_error "$(basename "$f" .cub)" "$f"; done
	for f in "$FIX"/good/cor_*.cub; do expect_alive "valida: $(basename "$f" .cub)" "$f"; done
	manual "Cores: abra $FIX/manual/cores_vermelho_azul.cub -> chao VERMELHO, teto AZUL (se inverter, a ordem dos bytes esta errada)"
	manual "Cores: F 220,100,0 deve virar 14443520 (0xDC6400); C 225,30,0 deve virar 14753280 (0xE11E00)"
}

b6() {
	local f
	for f in "$FIX"/bad/tex_*.cub; do
		[ "$(basename "$f")" = tex_sem_permissao.cub ] && [ "$(id -u)" = 0 ] \
			&& { skip "tex_sem_permissao" "root"; continue; }
		expect_error "$(basename "$f" .cub)" "$f"
	done
	for f in "$FIX"/good/tex_*.cub; do expect_alive "$(basename "$f" .cub)" "$f"; done
}

b7() {
	local f
	for f in "$FIX"/bad/map_*.cub; do expect_error "$(basename "$f" .cub)" "$f"; done
	for f in "$FIX"/good/map_*.cub; do expect_alive "valido: $(basename "$f" .cub)" "$f"; done
}

b8() {
	manual "Parser (precisa dos prints de debug): minimap.cub -> largura 20, altura 20, spawn N em row 18/col 17, view_direction 78 ('N')"
	manual "Parser: todas as linhas do layout com o mesmo tamanho; spawn virou '0' no layout; row=y, col=x"
	manual "Parser: caminhos das texturas sem \\n nem espaco sobrando (imprima entre colchetes [...])"
	manual "Ponte: game.map.map == config.map.map_layout (mesmo ponteiro) e game->cfg != NULL"
	info "Bloco 8 depende dos prints temporarios: veja a secao de checagem manual no fim"
}

b9() {
	local f d
	for f in "$FIX"/bad/load_*.cub; do expect_error_gui "$(basename "$f" .cub)" "$f"; done
	for f in "$FIX"/good/load_*.cub; do expect_alive "$(basename "$f" .cub)" "$f"; done
	manual "Texturas de tamanhos diferentes (good/load_tamanhos_diferentes): nenhuma parede distorcida; 1x1 sem divisao por zero"
}

b10() {
	local d
	for d in N S E W; do expect_alive "spawn $d abre sem crash" "$FIX/good/spawn_$d.cub"; done
	expect_alive "spawn encostado em parede" "$FIX/good/spawn_encostado.cub"
	manual "Spawn: N olha para o topo do mapa, S para a base, E para a direita, W para a esquerda ($FIX/manual/spawn_*.cub)"
	manual "Spawn: pos = (col+0.5, row+0.5); dir=(0,-1) p/ N; dir e cam perpendiculares; lado direito da tela = leste em N"
	manual "Spawn: olhando exatamente p/ N/S/E/W nao pode haver coluna preta no centro da tela"
}

b11() {
	manual "Render: $FIX/manual/arena.cub tem 4 texturas de cor solida: parede norte VERMELHA, sul VERDE, oeste AZUL, leste AMARELA"
	manual "Render: $FIX/manual/espelhamento.cub -> textura metade vermelha/azul: o vermelho tem que ficar a ESQUERDA em todas as 4 faces"
	manual "Render: sem faixas pretas nas quinas, sem curvatura (olho de peixe) em parede reta, teto/chao divididos no meio"
	manual "Render: $FIX/manual/corredor_longo.cub (120 de comprimento > RAY_MAX_DIST=50): sem coluna faltando nem crash"
	manual "Render: encostar numa parede (distancia ~0): sem crash nem textura 'explodindo'"
}

b12() {
	manual "Movimento: W frente, S tras, A esquerda, D direita (relativos a visao, tambem depois de girar)"
	manual "Movimento: seta direita gira p/ direita, esquerda p/ esquerda; as duas juntas = parado; W+S e A+D = parado"
	manual "Movimento: DIAGONAL (W+D) deve andar a mesma distancia por segundo que W sozinho (hoje forward/strafe nao sao normalizados: ~41% mais rapido)"
	manual "Movimento: gire 1 minuto seguido e confira que |dir| continua 1 (dir_x^2 + dir_y^2 ~ 1)"
	manual "Movimento: CapsLock ligado ainda anda; 5 teclas ao mesmo tempo nao trava"
	manual "Movimento: segure W, clique em outra janela, solte W, volte: o jogador NAO pode continuar andando (tecla presa)"
	manual "Movimento: velocidade independente do FPS (compare a distancia em 3 s normal e sob valgrind)"
}

b13() {
	manual "Colisao: andar de frente contra cada parede externa; deslizar na diagonal ao longo da parede"
	manual "Colisao: $FIX/manual/quina_diagonal.cub -> nao passa pela folga diagonal entre as duas paredes"
	manual "Colisao: corredor de 1 celula (good/map_corredor) passa de ponta a ponta sem prender; sala minima nao escapa"
	manual "Colisao: girar encostado na parede nao entra na parede"
	manual "Colisao (tunelamento): segure W, 'kill -STOP <pid>', espere 2 s, 'kill -CONT <pid>': nao pode atravessar parede (limite o delta_time, ex. 0.05 s)"
	manual "Colisao: primeiro frame com delta_time normal (last_frame_time setado no data_init)"
}

b14() {
	local d
	for d in 50x50 100x5 5x100 200x200 500x500 120x6; do
		ALIVE_T=8 expect_alive "mapa $d abre" "$FIX/good/big_$d.cub"
	done
	if [ -f "$FIX/good/big_1000x1000.cub" ]; then
		ALIVE_T=20 expect_alive "mapa 1000x1000 abre" "$FIX/good/big_1000x1000.cub"
	else
		info "mapa 1000x1000 desativado (BIGMAP=1 para incluir)"
	fi
	mk good/enorme_sem_obstaculo "$(gen_room 150 8)"
	expect_alive "sala 150x8 (maior que RAY_MAX_DIST)" "$FIX/good/enorme_sem_obstaculo.cub"
	if [ "$HAVE_GUI" = 1 ]; then
		local t0=$SECONDS
		TMO=60 run -n "$FIX/good/big_500x500.cub"
		info "500x500: ficou vivo ate o timeout=$([ $RC -eq 124 ] && echo sim || echo nao) (tempo total $((SECONDS - t0))s)"
	fi
}

b15() {
	local g="$FIX/good/base.cub" p1 p2
	ENVP=(env -u DISPLAY)
	expect_error "sem DISPLAY: erro limpo" "$g"
	[ "$(grep -c '^Error' "$T/err")" -le 1 ] && ok "sem DISPLAY: mensagem 'Error' aparece uma vez" \
		|| fail "sem DISPLAY: 'Error' repetido ($(grep -c '^Error' "$T/err")x) (cub_connection e cub_init imprimem os dois?)"
	ENVP=(env DISPLAY=:9999)
	expect_error "DISPLAY inexistente (:9999)" "$g"
	ENVP=()
	if [ "$HAVE_GUI" = 1 ]; then
		timeout 4 "$BIN" "$g" >/dev/null 2>&1 & p1=$!
		timeout 4 "$BIN" "$g" >/dev/null 2>&1 & p2=$!
		wait "$p1"; local r1=$?; wait "$p2"; local r2=$?
		[ "$r1" -eq 124 ] && [ "$r2" -eq 124 ] && ok "duas instancias simultaneas" \
			|| fail "duas instancias simultaneas" "exit $r1 / $r2"
	else
		skip "duas instancias simultaneas" "sem GUI"
	fi
	manual "mlx: minimizar/restaurar, mover a janela e redimensionar durante o jogo nao trava nem crasha"
}

b16() {
	local g="$FIX/good/base.cub"
	gui_session "ESC logo na abertura: zero leaks"            "$g" esc
	gui_session "ESC apertado 3 vezes seguidas (double free?)" "$g" esc esc esc
	gui_session "andar e girar, depois ESC: zero leaks"       "$g" hold:w:1.5 hold:Right:1 hold:a:1 hold:s:1 hold:Left:1 esc
	gui_session "ESC com W pressionado"                       "$g" down:w sleep:1 esc
	gui_session "mapa irregular: andar e fechar"              "$FIX/good/map_irregular.cub" hold:w:1 hold:d:1 esc
	gui_session "mapa grande: andar e fechar"                 "$FIX/good/big_200x200.cub" hold:w:1.5 hold:Right:1 esc
	# memoria nao cresce
	if need_gui "memoria estavel durante o jogo (RSS)"; then
		"$BIN" "$g" >/dev/null 2>&1 & local pid=$! first last limit
		sleep 3
		first=$(ps -o rss= -p "$pid" 2>/dev/null | tr -d ' ')
		sleep "${RSS_SECS:-20}"
		last=$(ps -o rss= -p "$pid" 2>/dev/null | tr -d ' ')
		kill "$pid" 2>/dev/null; wait "$pid" 2>/dev/null
		if [ -z "$first" ] || [ -z "$last" ]; then
			fail "memoria estavel durante o jogo (RSS)" "o processo morreu durante a medicao"
		else
			limit=$((first + first / 10 + 2048))
			[ "$last" -le "$limit" ] && ok "memoria estavel durante o jogo (RSS ${first}KB -> ${last}KB)" \
				|| fail "memoria cresce durante o jogo" "RSS ${first}KB -> ${last}KB (limite ${limit}KB)"
		fi
	fi
	manual "Saida: fechar com o X da janela (o script nao consegue simular o botao do gerenciador de janelas): valgrind deve mostrar 'All heap blocks were freed'"
	manual "Saida: Ctrl+C no terminal mata o processo (leaks nesse caso sao esperados)"
}

b17() {
	local f i rc bad=0 g="$FIX/good/base.cub"
	# todos os mapas invalidos: nenhum segfault, nenhum exit 0, nenhum leak
	printf '  tabela dos mapas invalidos (exit | primeira linha da mensagem):\n'
	for f in "$FIX"/bad/*.cub; do
		run "$f"
		printf '    %-34s exit=%-3s %s\n' "$(basename "$f")" "$RC" "$(sed -n 2p "$T/err" | head -c 60)"
		case $RC in 139|134|42|0|124) bad=$((bad + 1)) ;; esac
	done
	[ "$bad" -eq 0 ] && ok "nenhum mapa invalido causou segfault, abort, leak ou foi aceito" \
		|| fail "$bad mapa(s) invalidos com comportamento ruim (veja a tabela acima)"
	# repetibilidade
	run -n "$FIX/bad/map_aberto_dir.cub"; cp "$T/err" "$T/err1"
	run -n "$FIX/bad/map_aberto_dir.cub"
	cmp -s "$T/err" "$T/err1" && ok "mesma entrada, mesma saida (duas execucoes)" \
		|| fail "saida diferente entre duas execucoes iguais"
	# 500 execucoes seguidas
	rc=0
	for i in $(seq 1 500); do
		"$BIN" "$FIX/bad/map_sem_spawn.cub" >/dev/null 2>&1; rc=$?
		[ "$rc" -eq 1 ] || break
	done
	[ "$rc" -eq 1 ] && ok "500 execucoes seguidas de mapa invalido (sempre exit 1)" \
		|| fail "execucao $i de 500 terminou com exit $rc"
	# valor de config gigante
	( H_NO="NO $(rep a 100000)"; mk bad/stress_valor_gigante )
	expect_error "valor de config com 100 mil caracteres" "$FIX/bad/stress_valor_gigante.cub"
	# 10 mil linhas vazias entre config e mapa
	{ hdr; yes '' | head -n 10000; printf '%s\n' "$MAP"; } >"$FIX/good/stress_10k_vazias.cub"
	expect_alive "10 mil linhas vazias antes do mapa" "$FIX/good/stress_10k_vazias.cub"
	# mapa com 10 mil linhas
	mk good/stress_10k_linhas "$(gen_room 5 10000)"
	ALIVE_T=10 expect_alive "mapa com 10 mil linhas (append_row copia o vetor a cada linha)" "$FIX/good/stress_10k_linhas.cub"
	# valgrind --track-origins
	if [ "$HAVE_VG" = 1 ]; then
		valgrind -q --track-origins=yes --leak-check=full --log-file="$T/vg.log" \
			"$BIN" "$FIX/bad/map_aberto_dir.cub" >/dev/null 2>&1
		grep -q -i 'uninitialised' "$T/vg.log" \
			&& fail "valor nao inicializado (track-origins)" "$(grep -i -m1 -A3 uninitialised "$T/vg.log" | tr '\n' ' ')" \
			|| ok "sem valor nao inicializado (--track-origins=yes)"
	else
		skip "track-origins" "sem valgrind"
	fi
}

b18() {
	local hits files out
	hits=$(grep -rIn --include='*.c' --include='*.h' -E 'debug_print|test_ray|put_player_half' . \
		--exclude-dir=minilibx-linux --exclude-dir=libft --exclude-dir="$T" --exclude-dir=objs 2>/dev/null)
	if [ -z "$hits" ]; then ok "sem codigo de debug (debug_print*, test_ray, put_player_half)"
	else fail "codigo de debug ainda presente" "$(echo "$hits" | head -3 | tr '\n' ' ')"; fi
	hits=$(grep -rIn --include='*.c' -E '^\s*printf\(' . \
		--exclude-dir=minilibx-linux --exclude-dir=libft --exclude-dir="$T" --exclude-dir=objs 2>/dev/null | head -5)
	[ -z "$hits" ] && ok "sem printf solto nos .c" || warn "printf encontrado (confira se e legitimo): $(echo "$hits" | head -2 | tr '\n' ' ')"
	hits=$(grep -rIn --include='*.c' -E '^\s*//\s*(printf|mlx_|game_|if|while)' . \
		--exclude-dir=minilibx-linux --exclude-dir=libft --exclude-dir="$T" --exclude-dir=objs 2>/dev/null | wc -l)
	[ "$hits" -le 5 ] && ok "poucos trechos comentados" || warn "$hits linhas de codigo comentado"
	if command -v norminette >/dev/null 2>&1; then
		files=$(find . \( -name '*.c' -o -name '*.h' \) -not -path './minilibx*' -not -path './libft/*' \
			-not -path "./$T/*" -not -path './objs/*')
		# shellcheck disable=SC2086
		out=$(norminette $files 2>&1 | grep -v ': OK!')
		[ -z "$out" ] && ok "norminette limpa" || fail "norminette reportou erros" "$(echo "$out" | head -3 | tr '\n' ' ')"
	else
		skip "norminette" "nao instalada"
	fi
	if command -v nm >/dev/null 2>&1; then
		out=$(nm -u "$BIN" 2>/dev/null | awk '{print $2}' | sed 's/@.*//' \
			| grep -v -E '^(_|X|mlx_|open$|close$|read$|write$|printf$|malloc$|free$|perror$|strerror$|exit$|gettimeofday$|sin$|cos$|tan$|sqrt$|fabs$|floor$|ceil$|pow$|round$)' | sort -u | tr '\n' ' ')
		info "simbolos externos fora da lista permitida (revise: libs estaticas do mlx/libft aparecem aqui): ${out:-nenhum}"
	fi
	manual "Regressao: depois de qualquer mudanca no parser rode os blocos 2 a 8; no close_handler/clear_* o bloco 16; no player_init o bloco 10"
	manual "Final: make fclean && make do zero + ./run_tests.sh completo"
}

# ============================================================================
#  MAIN
# ============================================================================
usage() { sed -n '2,24p' "$0" | sed 's/^# \{0,1\}//'; }

SELECTED=()
for a in "$@"; do
	case $a in
		-l) for b in "${ALL_BLOCKS[@]}"; do printf '%2s  %s\n' "$b" "${BNAME[$b]}"; done; exit 0 ;;
		-h|--help) usage; exit 0 ;;
		[0-9]|[0-9][0-9]) SELECTED+=("$a") ;;
		*) echo "argumento desconhecido: $a (use -h)"; exit 2 ;;
	esac
done
[ ${#SELECTED[@]} -eq 0 ] && SELECTED=("${ALL_BLOCKS[@]}")

[ -f Makefile ] || { echo "rode na raiz do projeto (onde esta o Makefile)"; exit 2; }
mkdir -p "$T"
: >"$T/out"; : >"$T/err"; : >"$T/vg.log"
setup_env
gen_fixtures

for b in "${SELECTED[@]}"; do
	CUR=$b
	printf '\n%s== Bloco %s: %s ==%s\n' "$C" "$b" "${BNAME[$b]}" "$N"
	if [ "$b" != 1 ] && ! find_bin; then
		skip "bloco $b" "binario nao encontrado: compile antes (make) ou rode o bloco 1"
		continue
	fi
	"b$b"
	[ "$b" = 1 ] && find_bin
done

printf '\n%s============================ RESUMO ============================%s\n' "$C" "$N"
printf '  %sOK: %d%s   %sFAIL: %d%s   %sSKIP: %d%s   %sWARN: %d%s\n' \
	"$G" "$PASS" "$N" "$R" "$FAIL" "$N" "$Y" "$SKIP" "$N" "$Y" "$WARN" "$N"
if [ ${#FAILED[@]} -gt 0 ]; then
	printf '\n%sFalhas:%s\n' "$R" "$N"
	printf '  - %s\n' "${FAILED[@]}"
	printf '\n  logs de cada falha: %s/\n' "$LOGS"
fi
if [ ${#MANUAL[@]} -gt 0 ]; then
	printf '\n%sChecagem MANUAL (o script nao consegue verificar):%s\n' "$Y" "$N"
	printf '  [ ] %s\n' "${MANUAL[@]}"
	printf '\n  mapas para jogar na mao: %s/manual/\n' "$FIX"
fi
[ "$FAIL" -eq 0 ]
