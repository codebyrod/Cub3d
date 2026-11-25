#!/usr/bin/env bash
BIN=${BIN:-}
for c in ./cub3D ./cub3d; do [ -z "$BIN" ] && [ -x "$c" ] && BIN=$c; done
[ -x "$BIN" ] || { echo "binario nao encontrado (use BIN=./seu_binario)"; exit 2; }

chk() {   # arquivo largura altura row col dir
	local out
	out=$(CUB_DEBUG=1 timeout 10 "$BIN" "$1" 2>&1)
	for kv in "width=$2" "height=$3" "spawn_row=$4" "spawn_col=$5" "spawn_dir=$6" \
		rows_equal=1 rows_match_height=1 spawn_cell=48 same_ptr=1 cfg_ok=1; do
		if ! echo "$out" | grep -qx "$kv"; then
			echo "FAIL $1: esperado $kv"
			echo "$out" | head -4 | sed 's/^/       saida: /'
			return
		fi
	done
	[ "$(echo "$out" | grep -cE '^(north|south|west|east)=\[[^] ]+\]$')" -eq 4 ] \
		|| { echo "FAIL $1: caminho de textura com espaco ou quebra de linha"; return; }
	echo "OK   $1"
}

M=tests_cub3d/maps/good
chk maps/minimap.cub     20 20 18 17 78
chk $M/base.cub           6  4  2  4 78
chk $M/spawn_S.cub        6  4  2  4 83
chk $M/spawn_E.cub        6  4  2  4 69
chk $M/spawn_W.cub        6  4  2  4 87
chk $M/map_irregular.cub  8  4  2  1 78
chk $M/map_indentado.cub  6  3  1  3 78
chk $M/map_minimo_3x3.cub 3  3  1  1 78