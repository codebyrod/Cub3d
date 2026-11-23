# Trilha de integração parser + execução (Cub3D)

Ordem de trabalho do mais imediato ao mais distante, com um teste de sucesso em cada etapa para validar antes de seguir.

---

## Etapa 1: Entender o estado atual

**Objetivo:** confirmar que a `main` e o parser se comportam como esperado, antes de mexer em qualquer coisa.

- [x] Rodar `./cub3d` sem argumento, depois com dois argumentos: ambos devem dar a mensagem de uso
- [x] Rodar `./cub3d arquivo_que_nao_existe.cub` e `./cub3d minimap.txt`: o parser deve recusar com erro limpo
- [x] Rodar `./cub3d minimap.cub` e **anotar o que acontece** (janela, crash, tela de uma cor só)
- [x] Conferir se `config` é zerado antes do `parse`. Hoje só o `game` leva `ft_bzero`. Se o parser depende das flags começarem em 0, falta `ft_bzero(&config, sizeof(config))`

**Teste de sucesso:** vocês sabem explicar por que o resultado do `minimap.cub` é o que é.

## Etapa 2: Provar que o parser entrega o que o jogo precisa

**Objetivo:** antes de ligar as pontas, ver os dados que saem do parser.

- [x] Depois do `parse`, imprimir temporariamente: `map.width`, `map.height`, as 4 texturas, `floor_color`, `ceil_color`, `player_data.row/col/view_direction`
- [x] Comparar com o `minimap.cub`:
  - largura **20**, altura **20**
  - spawn **N** em **row 18, col 17**
  - `F 220,100,0` deve virar `14443520` (`0xDC6400`)
  - `C 225,30,0` deve virar `14753280` (`0xE11E00`)
- [x] Se `row/col` vierem trocados, ou a cor vier em outra ordem de bytes, resolver **agora**, no parser

**Teste de sucesso:** os números batem com o arquivo. Depois, apagar os prints.

## Etapa 3: Header do jogo conhecer o tipo do parser

**Objetivo:** o `t_game` poder guardar o ponteiro da config.

- [x] No `cub3d.h`, incluir o `parser.h` e adicionar o campo `t_data_pars *cfg;` no `t_game`
- [ ] Adicionar os protótipos `config_to_game`, `free_game_map` e `clear_textures`

**Teste de sucesso:** `make` compila sem erro.

## Etapa 4: A ponte (`config_to_game`)

**Objetivo:** transformar o mapa do parser no mapa que o raycaster lê.

- [ ] Criar `config_bridge.c` com `pad_row`, `build_map`, `config_to_game` e `free_game_map`, adaptando os tipos para o sufixo `_pars`
- [ ] Adicionar o arquivo ao Makefile
- [ ] Descomentar `config_to_game` e `game.cfg = &config` na `main`, tratando a falha da ponte com `free_config` e retorno 1
- [ ] **Teste isolado:** colocar um `return` logo depois da ponte, imprimir o mapa linha a linha e conferir que o spawn virou `0` e todas as linhas têm o mesmo tamanho
- [ ] Rodar esse teste sob `valgrind` (deve liberar `game.map.map` e `config`)

**Teste de sucesso:** o mapa impresso é igual ao `.cub`, exceto o `N`, que virou `0`. Valgrind limpo.

## Etapa 5: Texturas e cores vindo da config

**Objetivo:** o jogo parar de usar valores fixos.

- [ ] Em `textures_init`, trocar os caminhos fixos por `game->cfg->textures`
- [ ] Em `data_init`, apagar as duas linhas que fixam cor do teto e do chão (elas sobrescrevem as da ponte)
- [ ] **Teste:** editar o `.cub` para `F 255,0,0` e `C 0,0,255`. Chão vermelho e teto azul confirmam que a config chega na tela
- [ ] **Teste:** apontar uma textura para um caminho inexistente. Deve sair erro limpo, sem crash

## Etapa 6: Jogador nasce onde o mapa manda

**Objetivo:** posição e direção vindas do parser.

- [ ] Apagar `pos_init_player` e o protótipo
- [ ] Reescrever `player_init` lendo `row`, `col` e `view_direction`, com `+ 0.5` e a direção por letra: N=(0,-1), S=(0,1), E=(1,0), W=(-1,0)
- [ ] **Teste:** abrir o `minimap.cub`. O jogador deve nascer no canto inferior direito, olhando para o norte
- [ ] **Teste dos 4 spawns:** trocar o `N` por `S`, `E` e `W` e conferir a direção e que cada face da parede mostra a textura certa

## Etapa 7: Jogar de verdade

**Objetivo:** movimento e colisão no mapa real.

- [ ] Andar com WASD, girar com as setas, bater nas paredes, andar colado nelas
- [ ] **Teste com mapa irregular:** `.cub` com linhas de tamanhos diferentes. Sem segfault, graças ao padding
- [ ] **Teste de mapa grande:** um mapa maior que 40x40. Confirmar que a checagem antiga de tamanho da janela foi realmente removida

## Etapa 8: Saída e memória

**Objetivo:** fechar sem vazar nada.

- [ ] Reescrever `close_handler`: texturas, imagem, janela, conexão, `free_game_map`, `free_config(game->cfg)`, `exit`
- [ ] Criar `clear_textures` em `clear.c`
- [ ] Fazer o ESC chamar `close_handler`
- [ ] **Teste:** fechar com ESC e com o X da janela, sob `valgrind --leak-check=full --show-leak-kinds=all`. Meta: zero leaks

## Etapa 9: Corrigir os bugs menores

- [ ] `main`: no caminho de erro do `cub_init`, liberar também `game.map.map` (hoje só libera a config)
- [ ] `cub_init`: conferir se o retorno em sucesso e em erro está coerente com o `if (cub_init(...))` da `main`
- [ ] `my_pixel_put`: trocar `>` por `>=` nos limites e remover a gravação duplicada
- [ ] `err_init_cub`: conferir o retorno de `ft_strscmp`

## Etapa 10: Teste de estresse do parser

- [ ] Mapa aberto, sem spawn, dois spawns, cor fora de 0 a 255, identificador duplicado ou faltando, arquivo vazio, mapa no meio da config
- [ ] Cada caso deve dar `Error` seguido de mensagem em stderr, sem crash e sem leak

## Etapa 11: Limpeza final

- [ ] Remover `printf` de debug, `test_ray`, trechos comentados e funções de teste (`put_player_half`)
- [ ] Conferir a norminette
- [ ] `make fclean && make`, e rodar tudo de novo do zero

---

Se travarem em qualquer etapa, anotem a saída exata do que aconteceu (mensagem, trecho do valgrind ou o print da Etapa 2) para diagnosticar.
