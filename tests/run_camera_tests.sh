
#!/bin/bash

# Interrompe o script se algum comando falhar.
set -e

# Descobre a raiz do projeto a partir da localização deste script.
ROOT="$(cd "$(dirname "$0")/.." && pwd)"

# Identifica onde estão os arquivos do projeto.
if [ -f "$ROOT/src/data_init.c" ]; then
	SRC_DIR="$ROOT/src"
else
	SRC_DIR="$ROOT"
fi

# Define o executável temporário.
BIN="/tmp/cub3d_camera_tests"

# Remove o executável temporário ao finalizar.
trap 'rm -f "$BIN"' EXIT

echo "Compilando testes da camera..."

cc -Wall -Wextra -Werror -g \
	-fsanitize=address,undefined \
	-I"$ROOT" \
	-I"$ROOT/includes" \
	"$ROOT/tests/test_camera.c" \
	"$SRC_DIR/data_init.c" \
	"$SRC_DIR/camera.c" \
	-lm -o "$BIN"

echo ""
echo "Executando testes..."
echo ""

"$BIN"
