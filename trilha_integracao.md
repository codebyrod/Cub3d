# Trilha de integração parser + execução (Cub3D)

Ordem de trabalho do mais imediato ao mais distante, com um teste de sucesso em cada etapa para validar antes de seguir.

> **Versão atualizada após a leitura do código do parser.** Principais mudanças em relação à versão anterior:
> - O parser já preenche o mapa com espaços até `width`, então a ponte **não aloca nem copia nada** (sem `pad_row`, `build_map` ou `free_game_map`).
> - O `ft_bzero` do `config` já acontece dentro do `parse`.
> - Dois bugs do parser entraram na Etapa 1: `sufix_verification` (leitura fora da string) e o formato do `parse_error`.
> - Texturas inexistentes são barradas pelo parser; o caminho de falha do `textures_init` precisa de outro teste.


---

## Etapa 1: Entender o estado atual e corrigir o parser nas bordas

**Objetivo:** confirmar que a `main` e o parser se comportam como esperado, antes de mexer em qualquer coisa.

- [x] Rodar `./cub3d` sem argumento, depois com dois argumentos: ambos devem dar a mensagem de uso
- [x] Rodar `./cub3d arquivo_que_nao_existe.cub`: o parser deve recusar com erro limpo e código de saída 1
- [x] Criar `cp minimap.cub minimap.txt` e rodar `./cub3d minimap.txt`: deve recusar pela extensão
- [x] Rodar `./cub3d ab` e `./cub3d .cub`: hoje `sufix_verification` lê fora da string (`&map[len - 4]` com `len < 4`). **Corrigir:**

```c
int	sufix_verification(char *map)
{
	int	len;

	len = ft_strlen(map);
	if (len <= 4)
		return (1);
	return (ft_strncmp(&map[len - 4], ".cub", 5));
}
```

- [ ] `parse_error` imprime `Error: msg` numa linha só. O subject pede `Error` seguido da mensagem. **Corrigir:**

```c
void	parse_error(char *msg)
{
	ft_putendl_fd("Error", 2);
	ft_putendl_fd(msg, 2);
}
```

- [x] ~~Conferir se `config` é zerado antes do `parse`~~: já é, o `parse` faz `ft_bzero(game_config, sizeof(t_data_pars))` na primeira linha
- [ ] Rodar `./cub3d minimap.cub` e **anotar o que acontece** (janela, crash, tela de uma cor só)


## Etapa 2: Provar que o parser entrega o que o jogo precisa

**Objetivo:** antes de ligar as pontas, ver os dados que saem do parser.

- [x] Criar `debug_print_config` (temporária) e chamá-la **depois** do `parse` (se vier antes, imprime lixo e dá segfault no `free_config`)
- [x] Imprimir: `map.width`, `map.height`, as 4 texturas entre colchetes, `floor_color`, `ceil_color`, `player_data.row/col/view_direction` e o layout linha a linha
- [x] Comparar com o `minimap.cub`:
  - largura **20**, altura **20**
  - spawn **N** em **row 18, col 17** (`row` é y, `col` é x, ambos base 0)
  - `F 220,100,0` deve virar `14443520` (`0xDC6400`)
  - `C 225,30,0` deve virar `14753280` (`0xE11E00`)
- [x] Conferir nos colchetes que as texturas não têm espaço ou `\n` sobrando


## Etapa 3: Header do jogo conhecer o tipo do parser

**Objetivo:** o `t_game` poder guardar o ponteiro da config.

- [x] No `cub3d.h`, incluir o `parser.h` e adicionar o campo `t_data_pars *cfg;` no `t_game`
- [x] Adicionar os protótipos `config_to_game` e `clear_textures`

## Etapa 4: A ponte (`config_to_game`)

**Objetivo:** apontar o jogo para os dados do parser.

O `setup_map_size` do parser já completa todas as linhas com espaço até `width`, então a ponte não precisa copiar o mapa. O layout pertence à config e quem libera é o `free_config`.

- [x] Criar `config_bridge.c` com **apenas** `config_to_game`:


  O spawn vira `'0'` porque o `is_wall_at` trata tudo que não é `'0'` como parede. A direção continua em `cfg->player_data.view_direction`. A função não aloca, então não falha (pode virar `void`).

- [x] Adicionar o arquivo ao Makefile
- [x] Descomentar `config_to_game` na `main`

## Etapa 5: Texturas e cores vindo da config

**Objetivo:** o jogo parar de usar valores fixos.

- [x] Em `textures_init`, trocar os caminhos fixos por `game->cfg->textures.north/south/west/east`
- [x] Em `data_init`, apagar as duas linhas que fixam cor do teto e do chão (elas sobrescrevem as da ponte)
- [x] **Teste:** editar o `.cub` para `F 255,0,0` e `C 0,0,255`. Chão vermelho e teto azul confirmam que a config chega na tela

## Etapa 6: Jogador nasce onde o mapa manda

**Objetivo:** posição e direção vindas do parser.

- [x] Apagar `pos_init_player` e o protótipo
- [x] Reescrever `player_init` lendo `row`, `col` e `view_direction` de `game->cfg->player_data`, com `+ 0.5` e a direção por letra: N=(0,-1), S=(0,1), E=(1,0), W=(-1,0)
- [x] Manter `init_camera_plane` no final do `player_init`

## Etapa 7: Jogar de verdade

**Objetivo:** movimento e colisão no mapa real.

- [x] Andar com WASD, girar com as setas, bater nas paredes, andar colado nelas

## Etapa 8: Saída e memória

**Objetivo:** fechar sem vazar nada.

- [x] Criar `clear_textures` em `clear.c` (destroy das 4 imagens se `img_ptr != NULL`)
- [x] Reescrever `close_handler`: texturas, imagem, janela, conexão, `free_config(game->cfg)`, `exit`. **Não** há `free_game_map`: o mapa pertence à config
- [ ] Fazer o ESC chamar `close_handler`


## Etapa 9: Corrigir os bugs menores

- [ ] `main`: o caminho de erro do `cub_init` só precisa de `free_config` (já está certo, o mapa é da config)
- [ ] `cub_init`: conferir se o retorno em sucesso e em erro está coerente com o `if (cub_init(...))` da `main`
- [ ] `my_pixel_put`: trocar `>` por `>=` nos limites e remover a gravação duplicada
- [ ] `err_init_cub`: conferir o retorno de `ft_strscmp`


## Etapa 11: Limpeza final

- [ ] Remover `printf` de debug, `debug_print_config`, `test_ray`, trechos comentados e funções de teste (`put_player_half`)
- [ ] Conferir a norminette
- [ ] `make fclean && make`, e rodar tudo de novo do zero

---

Se travarem em qualquer etapa, anotem a saída exata do que aconteceu (mensagem, trecho do valgrind ou o print da Etapa 2) para diagnosticar.
