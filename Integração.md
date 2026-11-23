# Checklist de integração parser + execução (Cub3D)

Guia de tarefas para juntar o parser e o motor de execução em um único programa.

## Fase 1: Headers (compilar sem conflito)

- [x] No `parser.h`, renomear `s_player`/`t_player` para `s_spawn`/`t_spawn`
- [x] No `parser.h`, renomear `s_map`/`t_map` para `s_pmap`/`t_pmap`
- [x] No `parser.h`, atualizar `t_data` (campos `map` e `player_data` com os tipos novos) e o protótipo de `setup_map_size`
- [x] Rodar `grep -rn "t_map\|t_player" <pasta_do_parser>` e corrigir todas as ocorrências
- [x] No `cub3d.h`, adicionar `# include "parser.h"` abaixo do `includes/includes.h`
- [x] No `cub3d.h`, adicionar o campo `t_data *cfg;` no `t_game`
- [x] No `cub3d.h`, adicionar os protótipos `config_to_game`, `free_game_map` e `clear_textures`
- [x] No `cub3d.h`, remover o protótipo de `pos_init_player`
- [x] Verificar se a libft define `t_bool` (conflito com o `t_bool` do `cub3d.h`)

## Fase 2: Makefile

- [x] Adicionar os `.c` do parser à lista de fontes
- [ ] Adicionar os novos arquivos (`config_bridge.c`)
- [x] Garantir `-I` da libft e do `minilibx-linux`, e linkar libft, mlx, `-lXext -lX11 -lm`
- [x] Confirmar que só existe **uma** `main` em todos os fontes (apagar a do parser e a antiga da execução)
- [x] Fazer `make fclean && make` sempre que mexer em header

## Fase 3: Ponte parser para jogo (`config_bridge.c`)

- [ ] Criar `pad_row`: copia a linha e completa com espaços até `map.width`
- [ ] Criar `build_map`: aloca `char **` terminado em NULL com as linhas preenchidas
- [ ] Em `build_map`, trocar a letra do spawn por `'0'` em `map[row][col]`
- [ ] Criar `config_to_game`: copia largura, altura, `floor_color`, `ceil_color` e seta `game->cfg`
- [ ] Criar `free_game_map` (libera linhas e o vetor)
- [ ] Confirmar com o parser: `row` é y e `col` é x
- [ ] Confirmar com o parser: as cores saem como `(r << 16) | (g << 8) | b`

## Fase 4: Main unificada

- [ ] Uma única `main(int ac, char **av)` que valida os argumentos
- [ ] `ft_bzero(&game, sizeof(game))` antes de qualquer coisa
- [ ] Chamar `parse(av[1], &config)` e sair com 1 se falhar
- [ ] Chamar `config_to_game` e liberar a config se falhar
- [ ] Chamar `cub_init` com a condição **corrigida**: `if (cub_init(&game))`
- [ ] No caminho de erro, chamar `free_game_map` e `free_config`
- [ ] Remover o mapa hardcoded e a checagem `map * TILE_SIZE > WIDTH`

## Fase 5: Adaptar a execução

### textures.c

- [ ] Em `textures_init`, trocar os caminhos fixos por `game->cfg->textures.north/south/west/east`

### data\_init.c

- [ ] Apagar `pos_init_player`
- [ ] Apagar as duas linhas que fixam `celling_color` e `floor_color`
- [ ] Criar `set_spawn_dir` (N=(0,-1), S=(0,1), E=(1,0), W=(-1,0))
- [ ] Reescrever `player_init` lendo `player_data.row/col/view_direction` e usando `+ 0.5`
- [ ] Manter `init_camera_plane` no final do `player_init`

## Fase 6: Limpeza e saída

- [ ] Criar `clear_textures` em `clear.c` (destroy das 4 imagens se `img_ptr != NULL`)
- [ ] Reescrever `close_handler`: texturas, imagem, janela, conexão, mapa, `free_config`, `exit`
- [ ] No `handle_press`, chamar `close_handler` quando `keycode == XK_Escape`
- [ ] Lembrar que o `exit` no `close_handler` é o caminho normal de saída, então o `free_config` do fim da `main` não roda

## Fase 7: Bugs pequenos que apareceram

- [ ] `my_pixel_put`: trocar `x > WIDTH || y > HEIGHT` por `>=` e apagar a gravação duplicada
- [ ] `err_init_cub`: conferir o retorno de `ft_strscmp` (se retorna 0 quando iguais, a lógica está invertida)
- [ ] Remover `printf` de debug e `test_ray` antes da entrega

## Fase 8: Validações do parser (o subject exige)

- [ ] Extensão `.cub`
- [ ] Identificadores `NO SO WE EA F C`, sem duplicatas e sem faltar nenhum
- [ ] RGB com 3 valores entre 0 e 255
- [ ] Mapa como última parte do arquivo
- [ ] Exatamente um spawn (`N/S/E/W`)
- [ ] Mapa fechado por paredes (nenhum `0` ou spawn encostando em espaço, borda ou fora da linha)
- [ ] Arquivo `.cub` sem `\n` no final funcionando
- [ ] Mensagem `Error\n` seguida de texto em stderr

## Fase 9: Testes

- [ ] `./cub3D minimap.cub` abre, com o jogador olhando para o norte
- [ ] Mapa com linhas de tamanhos diferentes: sem segfault
- [ ] Mapas inválidos (aberto, sem spawn, dois spawns, cor errada, textura inexistente): erro limpo, sem crash
- [ ] Sem argumento, ou com mais de um: mensagem de uso
- [ ] `valgrind --leak-check=full --show-leak-kinds=all` fechando com ESC e com o X da janela: zero leaks
- [ ] Testar os quatro spawns (`N`, `S`, `E`, `W`) e conferir que as texturas aparecem nas faces certas

## Divisão sugerida entre vocês

| Quem | Tarefas |
| --- | --- |
| Parser | Fase 1 (lado do `parser.h`), Fase 8, confirmar `row/col` e cores |
| Execução | Fase 1 (lado do `cub3d.h`), Fases 3 a 7 |
| Juntos | Fase 2 (Makefile) e Fase 9 (testes) |

Comecem pela Fase 1 e pelo Makefile, que são o que mais bloqueia o resto. Quando compilar, mandem os erros que aparecerem.
