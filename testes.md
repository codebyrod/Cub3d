# Bateria de testes: Cub3D

A lista tem **19 blocos**, do build até o stress. Cada item traz o caso e o resultado esperado. Os blocos 1 a 8 só exercitam o parser e fecham antes de abrir janela, então dá para automatizar. O resto precisa de olho e teclado.

**Critério de erro limpo** (vale para todo teste de erro):
- `Error` na primeira linha e a mensagem na segunda, em **stderr**
- código de saída `1`
- sem segfault e sem leak no valgrind
- nenhuma janela abre

---

## Bloco 0: Preparação

```bash
mkdir -p maps/good maps/bad textures
HDR='NO ./textures/north.xpm
SO ./textures/south.xpm
WE ./textures/west.xpm
EA ./textures/east.xpm
F 220,100,0
C 225,30,0
'
MAP='111111
100001
1000N1
111111'
mk() { printf '%s\n%s\n' "$HDR" "$2" > "maps/$1.cub"; }   # mk nome "mapa"
mk good/base "$MAP"
```

**Texturas de teste** (cor sólida, uma por face, para provar qual textura cai em qual parede):

```bash
mk_xpm() {  # uso: mk_xpm nome RRGGBB tamanho
  local n=$3 row i
  row=$(printf 'a%.0s' $(seq "$n"))
  {
    echo '/* XPM */'; echo "static char *$1[] = {"
    echo "\"$n $n 1 1\","; echo "\"a c #$2\","
    for i in $(seq $((n-1))); do echo "\"$row\","; done
    echo "\"$row\""; echo '};'
  } > "textures/t_$1.xpm"
}
mk_xpm n FF0000 64; mk_xpm s 00FF00 64; mk_xpm w 0000FF 64; mk_xpm e FFFF00 64
```

**Runner dos mapas inválidos:**

```bash
run_bad() {
  for f in maps/bad/*.cub; do
    printf '%-38s ' "$f"
    valgrind -q --leak-check=full --show-leak-kinds=all --errors-for-leak-kinds=all \
      --error-exitcode=42 ./cub3d "$f" 2>/tmp/err.txt >/dev/null
    echo "exit=$? | $(head -c 90 /tmp/err.txt | tr '\n' '|')"
  done
}
```

Leitura do `exit`: **1** é o esperado, **42** significa que o valgrind achou erro ou leak, **139** é segfault e **0** significa que o programa aceitou um mapa que deveria recusar.

---

## Bloco 1: Build

- [ ] `make fclean && make` sem warning (`-Wall -Wextra -Werror`)
- [ ] `make` de novo sem mudanças: não recompila nada
- [ ] Alterar um `.h` e rodar `make`: reconstrói o que depende dele (se não reconstruir, o Makefile não rastreia headers)
- [ ] `make re` funciona e não faz relink desnecessário
- [ ] `make clean`, `make fclean` limpam sem erro, e rodar duas vezes seguidas também não dá erro
- [ ] O binário se chama `cub3D` (como o subject pede) e não só `cub3d`
- [ ] Nenhuma função proibida: `nm cub3d | grep " U "`
- [ ] `norminette` limpa em todos os `.c` e `.h`

## Bloco 2: Argumentos e nome do arquivo

- [ ] Sem argumento → mensagem de uso
- [ ] Dois ou mais argumentos → mensagem de uso
- [ ] Argumento vazio `./cub3d ""` → mensagem de uso
- [ ] `./cub3d ab` e `./cub3d a` → erro de extensão, sem leitura fora da string
- [ ] `./cub3d .cub` e `./cub3d cub` → recusa
- [ ] `./cub3d mapa.CUB` (maiúscula) → recusa
- [ ] `./cub3d mapa.cub.txt`, `./cub3d mapacub`, `./cub3d mapa.cub.` → recusa
- [ ] `./cub3d "mapa.cub "` (espaço no fim) → recusa
- [ ] `./cub3d mapa.cub.cub` existente e válido → aceita (termina em `.cub`)
- [ ] `./cub3d ./maps/good/base.cub`, caminho absoluto e caminho com `..` → todos funcionam
- [ ] Caminho com espaço: `cp maps/good/base.cub "maps/good/meu mapa.cub"` → funciona
- [ ] Argumento começando com hífen: `./cub3d -h`, `./cub3d --help` → recusa pela extensão
- [ ] Erro vai para stderr: `./cub3d 2>/dev/null` não imprime nada, e `./cub3d >/dev/null` ainda mostra a mensagem

## Bloco 3: O arquivo em si

- [ ] Arquivo inexistente: `./cub3d nao_existe.cub` → `Could not open the map file`
- [ ] Diretório com nome `.cub`: `mkdir maps/bad/dir.cub` → erro limpo, sem loop infinito
- [ ] Sem permissão: `cp maps/good/base.cub maps/bad/sem_perm.cub && chmod 000 maps/bad/sem_perm.cub` → erro limpo (não vale como root)
- [ ] Symlink para um mapa válido → funciona
- [ ] Arquivo vazio: `: > maps/bad/vazio.cub` → `Missing or incomplete configuration`
- [ ] Só linhas vazias e espaços: `printf '\n\n  \n\t\n' > maps/bad/so_branco.cub` → idem
- [ ] Lixo binário: `head -c 4096 /dev/urandom > maps/bad/random.cub` → erro limpo, sem crash
- [ ] Arquivo com bytes NUL no meio → sem crash
- [ ] BOM UTF-8 no começo: `printf '\xEF\xBB\xBF' | cat - maps/good/base.cub > maps/bad/bom.cub` → erro limpo (decidam se aceitam ou recusam)
- [ ] CRLF: `sed 's/$/\r/' maps/good/base.cub > maps/bad/crlf.cub` → comportamento definido e sem crash (a config tolera `\r`, o mapa não)
- [ ] Arquivo sem `\n` no final: `printf '%s\n%s' "$HDR" "$MAP" > maps/good/sem_nl_final.cub` → funciona
- [ ] Linha gigante (200 mil caracteres): `{ printf '%s\n' "$HDR"; head -c 200000 /dev/zero | tr '\0' 1; echo; } > maps/bad/linha_enorme.cub` → erro limpo, sem crash
- [ ] Rodar de outra pasta (`cd /tmp && .../cub3d .../base.cub`) → falha nas texturas relativas (esperado, informativo)

## Bloco 4: Identificadores e formato das linhas de config

Geração:
```bash
B=maps/good/base.cub
sed '/^NO/d' $B > maps/bad/falta_NO.cub
sed '/^SO/d' $B > maps/bad/falta_SO.cub
sed '/^WE/d' $B > maps/bad/falta_WE.cub
sed '/^EA/d' $B > maps/bad/falta_EA.cub
sed '/^F /d' $B > maps/bad/falta_F.cub
sed '/^C /d' $B > maps/bad/falta_C.cub
sed 's|^SO.*|NO ./textures/north.xpm|' $B > maps/bad/NO_duplicado.cub
sed 's|^C .*|F 1,2,3|' $B > maps/bad/F_duplicado.cub
sed 's/^NO/no/' $B > maps/bad/id_minusculo.cub
sed 's/^NO/NOO/' $B > maps/bad/id_NOO.cub
sed 's/^NO/XX/' $B > maps/bad/id_desconhecido.cub
sed '1i # comentario' $B > maps/bad/comentario.cub
sed 's|^NO.*|NO|' $B > maps/bad/NO_sem_valor.cub
sed 's|^NO.*|NO    |' $B > maps/bad/NO_so_espacos.cub
```

- [ ] `falta_*` (6 casos) → `Missing or incomplete configuration`
- [ ] `NO_duplicado`, `F_duplicado` → `Duplicate ... configuration`
- [ ] `id_minusculo`, `id_NOO`, `id_desconhecido`, `comentario` → `Unknown identifier...`
- [ ] `NO_sem_valor`, `NO_so_espacos` → `Invalid line (configuration expected)`
- [ ] **Duplicata depois de completar a config:** com os 6 identificadores presentes, um sétimo `NO ...` antes do mapa vira linha de mapa → `Invalid character in the map`. Confirmem que a mensagem faz sentido
- [ ] **Formatos aceitos:** TAB entre id e valor, espaços antes do id, espaços no fim da linha, vários espaços entre id e valor
- [ ] **Ordem livre:** `F` e `C` antes das texturas; texturas em ordem `EA, WE, SO, NO`
- [ ] Linhas vazias entre as linhas de config → aceito
- [ ] Valor com lixo no fim (`NO ./textures/north.xpm extra`) → `Could not open a texture file`

## Bloco 5: Cores

Para cada caso, troque a linha `F` (e depois a `C`) e rode.

- [ ] Inválidos, todos com `Invalid color`: `256,0,0` · `0,256,0` · `0,0,256` · `-1,0,0` · `1,2` · `1,2,3,4` · `1,,3` · `,1,2` · `a,b,c` · `1.5,2,3` · `1;2;3` · `220,100,0,` · `99999999999,0,0` · `1,2,3abc`
- [ ] Linha só com `F` e sem valor → `Invalid line`
- [ ] **Válidos:** `0,0,0` · `255,255,255` · `220, 100, 0` · `220 ,100 , 0` · `010,020,030` (zeros à esquerda) · `000,000,000`
- [ ] Teto igual ao chão → válido
- [ ] **Teste visual da ordem de bytes:** `F 255,0,0` e `C 0,0,255` → chão vermelho e teto azul. Depois `F 0,255,0` → chão verde
- [ ] Imprimir `floor_color` para `220,100,0` → `14443520` (`0xDC6400`)

## Bloco 6: Caminhos de textura (no parser)

- [ ] Arquivo inexistente → `Could not open a texture file`
- [ ] **Diretório** como textura (`NO ./textures`) → recusado
- [ ] Arquivo vazio (`: > textures/vazio.xpm`) → o parser aceita, o `textures_init` recusa (ver Bloco 9)
- [ ] Sem permissão de leitura → recusado
- [ ] Caminho absoluto → funciona
- [ ] Caminho com espaço (`./textures/minha pedra.xpm`) → funciona
- [ ] Caminho com `..` (`./textures/../textures/north.xpm`) → funciona
- [ ] Mesma textura nas 4 faces → funciona e não vaza
- [ ] Uma só falha entre as quatro: testar cada uma (NO, SO, WE, EA) → erro limpo

## Bloco 7: Mapa, caracteres e formato

Geração (função `mk` do Bloco 0):
```bash
mk bad/sem_spawn     $'111111\n100001\n100001\n111111'
mk bad/dois_spawns   $'111111\n1N00S1\n100001\n111111'
mk bad/aberto_dir    $'111111\n100000\n1000N1\n111111'
mk bad/aberto_topo   $'110111\n100001\n1000N1\n111111'
mk bad/aberto_esq    $'111111\n000001\n1000N1\n111111'
mk bad/aberto_base   $'111111\n100001\n1000N1\n111011'
mk bad/buraco_meio   $'111111\n100 01\n1000N1\n111111'
mk bad/spawn_borda   $'1N1111\n100001\n100001\n111111'
mk bad/char_invalido $'111111\n100201\n1000N1\n111111'
mk bad/minusculo     $'111111\n100001\n1000n1\n111111'
mk bad/tab_no_mapa   $'111111\n10\t001\n1000N1\n111111'
mk bad/linha_vazia   $'111111\n100001\n\n1000N1\n111111'
mk bad/linha_espacos $'111111\n100001\n   \n1000N1\n111111'
mk bad/texto_apos    $'111111\n100001\n1000N1\n111111\n\nlixo'
mk bad/mapa_no_fim_aberto $'111111\n100001\n1000N0'
printf '%s' "$HDR" > maps/bad/sem_mapa.cub
printf '%s\n%s\n' "$MAP" "$HDR" > maps/bad/mapa_antes_config.cub
```

- [ ] `sem_spawn`, `dois_spawns` (e três ou quatro) → `exactly one player start`
- [ ] Os seis `aberto_*`, `buraco_meio`, `spawn_borda` → `Map is not closed by walls`
- [ ] `char_invalido`, `minusculo`, `tab_no_mapa` → `Invalid character in the map`
- [ ] `linha_vazia`, `linha_espacos`, `texto_apos` → `Empty line or extra content inside the map`
- [ ] `sem_mapa` → `Map is missing`
- [ ] `mapa_antes_config` → `Invalid line (configuration expected)`
- [ ] Região aberta **inalcançável** (um `0` isolado numa sala sem acesso, aberta para fora) → o parser recusa. Registrem a decisão
- [ ] **Válidos que devem passar:**
  - Mapa mínimo 3x3 com spawn no centro
  - Linhas de tamanhos diferentes, todas fechadas por `1`
  - Linhas com espaços à esquerda (mapa "recuado")
  - Espaço vazio **dentro** do mapa cercado de `1`
  - Linhas em branco depois do mapa
  - Mapa sem `\n` final
  - Cada um dos 4 spawns
  - Spawn encostado em parede
  - Corredor de 1 célula
  - Mapa com várias salas conectadas

## Bloco 8: Saída do parser (Etapas 2 e 4)

Com `debug_print_config` e depois do `config_to_game`:

- [ ] `minimap.cub` → largura 20, altura 20, spawn N em `row 18, col 17`, cores `14443520` e `14753280`
- [ ] **Todas as linhas do layout têm o mesmo tamanho** (maior linha do arquivo)
- [ ] Mapa irregular: a largura é a **maior** linha e as menores foram preenchidas com espaço
- [ ] O spawn virou `'0'` no layout e `view_direction` continua `'N'` (`78`)
- [ ] Os 4 spawns: `view_direction` = `N`, `S`, `E`, `W`
- [ ] `row` é y e `col` é x. Num mapa assimétrico, o spawn nasce onde o arquivo mostra
- [ ] Caminhos das texturas sem `\n` nem espaço sobrando (use os colchetes `[...]`)
- [ ] Mapa 1 linha por 1 coluna é recusado (fechado exige no mínimo o spawn cercado)
- [ ] Valgrind sob o teste isolado: zero leaks

## Bloco 9: Carga das texturas no jogo

- [ ] Textura de outro caminho (`NO ./textures/t_n.xpm`) → aparece na parede norte
- [ ] **Arquivo que não é xpm** (`NO ./maps/good/base.cub`) → `Texture error`, código 1, sem leak
- [ ] Arquivo vazio como textura → mesmo resultado
- [ ] Xpm truncado (`head -c 200 textures/north.xpm > textures/trunc.xpm`) → erro limpo, sem crash
- [ ] **Falha parcial:** NO e SO válidas, WE inválida → o cleanup libera as duas que carregaram (valgrind)
- [ ] Falha na primeira, na última, e em cada uma das quatro
- [ ] Texturas de **tamanhos diferentes** entre faces (64x64, 128x128, 32x32) → nenhuma parede distorcida nem crash
- [ ] Textura **não quadrada** (64x128) → sem leitura fora da imagem
- [ ] Textura 1x1 → não divide por zero
- [ ] Textura grande (1024x1024) → renderiza sem lentidão absurda

## Bloco 10: Spawn e primeiro frame

- [ ] `N` → olha para o topo do mapa, `S` → base, `E` → direita, `W` → esquerda
- [ ] **Confirmar com as texturas coloridas** (`t_n`, `t_s`, `t_w`, `t_e`): spawn `N` mostra a parede que vem **de frente** com a cor da face voltada para você (a parede ao norte exibe a textura norte)
- [ ] Posição no centro da célula (`+ 0.5`): imprimir `pos` → `(17.50, 18.50)` no `minimap.cub`
- [ ] Spawn colado em parede (a 1 célula) → primeiro frame sem artefato
- [ ] **Coluna central**: olhando exatamente para N/S/E/W, o raio do centro tem componente zero. Sem coluna preta, sem faixa estranha
- [ ] `dir` e `cam` perpendiculares: `dir·cam ≈ 0` em cada spawn
- [ ] Lado direito da tela correto nos 4 spawns (spawn `N`: direita = leste)

## Bloco 11: Render

- [ ] Quatro paredes externas mostram **quatro texturas diferentes** nas faces certas
- [ ] **Sem espelhamento**: com uma textura que tem texto ou seta, o desenho lê certo nas 4 faces
- [ ] Sem faixas verticais pretas ou "costuras" nas quinas
- [ ] **Sem efeito olho de peixe**: parede reta aparece reta, sem curvar nas bordas
- [ ] Parede vizinha de profundidade diferente: a transição é limpa
- [ ] Teto e chão respeitam a metade da tela (em `y = HEIGHT/2`)
- [ ] Corredor longo: a parede distante diminui, sem salto
- [ ] Muito perto da parede (encostar): sem crash, sem textura "explodindo"
- [ ] Olhar para fora do mapa (porta aberta para espaços não é possível, então vale a quina): nenhuma coluna não desenhada
- [ ] Parede com textura de tamanho diferente em faces vizinhas: junção sem artefato

## Bloco 12: Movimento e rotação

- [ ] **W** anda para frente, **S** para trás, **A** para a esquerda, **D** para a direita (relativos à visão)
- [ ] Depois de girar 90°, W continua indo para onde você olha
- [ ] **Seta direita** gira para a direita, **seta esquerda** para a esquerda
- [ ] Esquerda + direita juntas → sem rotação
- [ ] W + S juntas → sem movimento
- [ ] A + D juntas → sem movimento
- [ ] **Diagonal (W + D)**: velocidade. Hoje `forward` e `strafe` não são normalizados, então a diagonal anda cerca de 41% mais rápido. Meçam a distância em 2 segundos de W e em 2 segundos de W+D, e corrijam se diferirem
- [ ] 360° de rotação volta ao ponto de partida, sem deriva (gire por 1 minuto e confira que `dir` ainda tem módulo 1: `dir_x² + dir_y² ≈ 1`)
- [ ] Mover e girar ao mesmo tempo (W + seta)
- [ ] Segurar 5 teclas ao mesmo tempo → sem travar
- [ ] CapsLock ligado: WASD maiúsculos funcionam
- [ ] **Velocidade independente do FPS**: comparem a distância em 3 segundos normal e sob `valgrind`
- [ ] **Perda de foco**: segure W, clique em outra janela e solte. Ao voltar, o jogador deve parar (se continuar andando, a tecla ficou "presa")
- [ ] ESC fecha na hora (Bloco 16)

## Bloco 13: Colisão

- [ ] Andar de frente contra cada uma das paredes externas: para sem atravessar
- [ ] **Deslizar**: andar na diagonal contra uma parede, o jogador desliza ao longo dela
- [ ] Encostar em uma quina interna, nos dois sentidos
- [ ] **Quina com folga diagonal** (duas paredes que se tocam só no canto): o jogador não passa
- [ ] Corredor de 1 célula de largura: passa de ponta a ponta, sem ficar preso
- [ ] Sala 1x1 com spawn no centro: não escapa, não crasha
- [ ] Girar encostado na parede: não "entra" na parede ao girar
- [ ] **Tunelamento** (salto de `delta_time`): segure W e congele o processo com `kill -STOP <pid>`, espere 2 segundos, `kill -CONT <pid>`. O jogador não pode atravessar uma parede fina. Se atravessar, limite o `delta_time` (por exemplo, máximo 0,05 s)
- [ ] Primeiro frame: `delta_time` não é gigante (o `last_frame_time` é setado no `data_init`)
- [ ] Andar até o fim do mapa em todas as direções e em todos os cantos

## Bloco 14: Mapas de formato extremo

- [ ] Maior que 40x40 (antiga trava da janela) → abre normalmente
- [ ] 100x5 (largo) e 5x100 (alto)
- [ ] 200x200 → o parse termina em tempo razoável e o FPS é aceitável
- [ ] Opcional: 1000x1000 → `time ./cub3d` mostra o parse; sem crash
- [ ] Mapa inteiro de `0` cercado por `1`, grande
- [ ] Muitas salas pequenas e corredores curtos
- [ ] Mapa com buracos de espaço internos (todos cercados): renderiza como parede
- [ ] Mapa onde a distância máxima de raio (`RAY_MAX_DIST = 50`) é menor que o mapa: sala de 100 células sem obstáculo → sem coluna faltando, sem crash

## Bloco 15: Ambiente do mlx

- [ ] **Sem display**: `env -u DISPLAY ./cub3d maps/good/base.cub` → erro limpo (`mlx_init` falha), código 1, sem leak
- [ ] `DISPLAY=:99 ./cub3d ...` (display inexistente) → idem
- [ ] Rodar duas instâncias ao mesmo tempo → ambas funcionam e fecham sem conflito
- [ ] Mensagem de erro de mlx sai uma só vez (hoje `cub_connection` e `cub_init` imprimem as duas)
- [ ] Minimizar e restaurar a janela → o jogo continua e redesenha
- [ ] Mover a janela de lugar durante o jogo → não trava de vez
- [ ] Redimensionar (se o WM permitir) → sem crash

## Bloco 16: Saída e memória

Para cada caso, rode `valgrind --leak-check=full --show-leak-kinds=all ./cub3d <mapa>` e confira `All heap blocks were freed`.

- [ ] Fechar com **ESC** → zero leaks, zero erros
- [ ] Fechar com o **X da janela** → zero leaks, zero erros
- [ ] Fechar **logo na abertura**, sem apertar nada
- [ ] Fechar **depois de andar e girar por um minuto**
- [ ] Apertar ESC várias vezes seguidas → sem double free
- [ ] ESC e X quase ao mesmo tempo → sem double free
- [ ] ESC segurado enquanto anda → fecha
- [ ] Ctrl+C no terminal → o processo morre (leaks aqui são esperados e não contam)
- [ ] Cada erro do Bloco 4 ao 9 → zero leaks
- [ ] Textura inválida (Bloco 9) → zero leaks, incluindo a conexão do mlx
- [ ] **Sem crescimento de memória durante o jogo:** deixe andando 60 segundos e rode `while sleep 2; do ps -o rss= -p $(pgrep -x cub3d); done`. O número deve estabilizar, sem subir sempre
- [ ] `valgrind` sem `Invalid read/write` em nenhum momento do jogo (andar até o fim do mapa, encostar em tudo)
- [ ] `valgrind --track-fds=yes`: nenhum arquivo aberto sobrando (o parser fecha o `fd` em todos os caminhos de erro)

## Bloco 17: Robustez do parser (stress)

- [ ] Rodar `run_bad` com **todos** os arquivos de `maps/bad/` → nenhum `exit=139`, nenhum `exit=42`, nenhum `exit=0`
- [ ] 500 execuções seguidas de um mapa inválido, em loop → sem crescimento de memória
- [ ] Config com 10 mil linhas vazias antes do mapa → funciona
- [ ] Mapa com 10 mil linhas → o parse termina (a função `append_row` copia o vetor a cada linha, então confirmem o tempo)
- [ ] Config com valor de 100 mil caracteres → erro limpo
- [ ] Muitos espaços no fim de cada linha do mapa → aceita (espaço é válido) e fecha corretamente
- [ ] Mesma entrada, duas execuções → mesmo resultado (sem dependência de memória não inicializada: `valgrind` com `--track-origins=yes` não mostra `uninitialised value`)

## Bloco 18: Regressão (antes de entregar)

- [ ] Rodar de novo **todos** os blocos 2 a 8 depois de qualquer mudança no parser
- [ ] Rodar o Bloco 16 depois de qualquer mudança no `close_handler` ou nos `clear_*`
- [ ] Rodar o Bloco 10 depois de qualquer mudança em `player_init`
- [ ] Remover `printf` de debug, `debug_print_config`, `debug_print_bridge`, `test_ray` e funções de teste (`put_player_half`, `put_tile` se não usadas)
- [ ] Nenhum trecho comentado grande sobrando
- [ ] `make fclean && make` do zero e a bateria rápida: `run_bad` e um mapa válido

---

Se algum item falhar, me mande o comando, a saída e (se for valgrind) o trecho do relatório, e eu aponto a causa. Se quiser, passo essa lista para um arquivo `.md`, para marcar os itens no repositório.