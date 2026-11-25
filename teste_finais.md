## Etapa 1: Entender o estado atual e corrigir o parser nas bordas

**Objetivo:** confirmar que a `main` e o parser se comportam como esperado, antes de mexer em qualquer coisa.

**Teste de sucesso:** todos os casos de argumento inválido dão `Error` em uma linha e a mensagem na seguinte, em stderr, sem crash. Vocês sabem explicar por que o resultado do `minimap.cub` é o que é.

## Etapa 2: Provar que o parser entrega o que o jogo precisa

**Objetivo:** antes de ligar as pontas, ver os dados que saem do parser.

**Teste de sucesso:** os números batem com o arquivo. Depois, apagar os prints e o `return` antecipado da `main`.

## Etapa 3: Header do jogo conhecer o tipo do parser

**Objetivo:** o `t_game` poder guardar o ponteiro da config.

**Teste de sucesso:** `make` compila sem erro.

## Etapa 4: A ponte (`config_to_game`)

**Objetivo:** apontar o jogo para os dados do parser.

- [ ] **Teste isolado:** colocar um `return` logo depois da ponte, imprimir `game.map.map` linha a linha e conferir que o spawn virou `0` e todas as linhas têm o mesmo tamanho
- [ ] Rodar esse teste sob `valgrind` (um `free_config` no fim deve deixar zero leaks)

**Teste de sucesso:** o mapa impresso é igual ao `.cub`, exceto o `N`, que virou `0`. Valgrind limpo.

## Etapa 5: Texturas e cores vindo da config

**Objetivo:** o jogo parar de usar valores fixos.

- [ ] **Teste (parser):** apontar uma textura para um caminho inexistente. O parser barra com `Could not open a texture file`
- [ ] **Teste (`textures_init`):** apontar uma textura para um arquivo legível que **não é xpm** (por exemplo `maps/minimap.cub`). O parser aceita e o `mlx_xpm_file_to_image` falha
- [ ] **Corrigir o que esse teste expõe:** quando `textures_init` falha, o `cub_init` retorna `TEXTURE_ERROR` sem limpar, e conexão, janela e imagem vazam. Chamar `end_connection(game)` (ou equivalente) nesse ramo

## Etapa 6: Jogador nasce onde o mapa manda

**Objetivo:** posição e direção vindas do parser.

- [ ] **Teste:** abrir o `minimap.cub`. O jogador deve nascer no canto inferior direito, olhando para o norte
- [ ] **Teste dos 4 spawns:** trocar o `N` por `S`, `E` e `W` e conferir a direção e que cada face da parede mostra a textura certa

## Etapa 7: Jogar de verdade

**Objetivo:** movimento e colisão no mapa real.

- [ ] **Teste com mapa irregular:** `.cub` com linhas de tamanhos diferentes. Sem segfault, graças ao padding do parser
- [ ] **Teste de mapa grande:** um mapa maior que 40x40. Confirmar que a checagem antiga de tamanho da janela foi realmente removida

## Etapa 8: Saída e memória

**Objetivo:** fechar sem vazar nada.

- [ ] **Teste:** fechar com ESC e com o X da janela, sob `valgrind --leak-check=full --show-leak-kinds=all`. Meta: zero leaks

## Etapa 9: Corrigir os bugs menores



## Etapa 10: Teste de estresse do parser

O parser já valida extensão, duplicatas, RGB de 0 a 255, spawn único, mapa fechado e linha vazia dentro do mapa. Esta etapa só confere que cada caso funciona.

- [ ] Mapa aberto, sem spawn, dois spawns, cor fora de 0 a 255, identificador duplicado ou faltando, arquivo vazio, mapa no meio da config
- [ ] Cada caso deve dar `Error` seguido de mensagem em stderr, sem crash e sem leak