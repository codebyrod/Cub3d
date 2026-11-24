
#!/bin/bash

set -e

BIN="/tmp/cub3d_dda_tests"

trap 'rm -f "$BIN"' EXIT

echo "======================================"
echo "       COMPILANDO TESTES DDA"
echo "======================================"

cc -Wall -Wextra -Werror -g \
	-fsanitize=address,undefined \
	-I. -I./includes \
	tests/test_dda.c \
	ray.c \
	ray_utils.c \
	-lm \
	-o "$BIN"

echo ""
echo "======================================"
echo "          EXECUTANDO TESTES"
echo "======================================"

"$BIN"
STATUS=$?

rm -f "$BIN"

exit "$STATUS"
