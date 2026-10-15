
# Planejamento Cub3d

> **Metodologia:** Bottom-up & Scaffolding. Cada sprint entrega um incremento demonstrável. Cada camada é testada isoladamente antes de servir de fundação para a próxima.
> **Regra de Ouro do Debug:** *Nunca depure três abstrações simultâneas.* Se o pixel não aparece, verifique: MLX → imagem → data → offset → pixel. Se a parede está errada: rayDir → DDA → hit → distance → projection. Se a textura falha: side → wallX → texX → texY → texture buffer.
> **Regra de Ouro da Dupla:** A engine consome **dados**. O parser produz **dados**. A arquitetura deve permitir trocar `mock map` por `parser map` sem que o raycaster saiba absolutamente nada sobre parsing.

---

## Visão Arquitetônica: A Pilha de Abstração

Internalize a pirâmide que você vai construir:

```
[Semana 5-6] Integração & Parser (.cub dinâmico) + Norma + Defesa
[Semana 4]   Ilusão 3D + Mapeamento UV + Texturas
[Semana 3]   Raycasting Matemático (DDA, FOV, Vetores)
[Semana 2]   Mundo 2D Mockado + Física de Colisão + Input Contínuo
[Semana 1]   Buffer de Imagem + Contrato de Dados + Build System
```

### Mapa de Dependências

```text
                 ┌──────────────┐
                 │   miniLibX   │
                 └──────┬───────┘
                        ↓
                 ┌──────────────┐
                 │   imagem     │
                 └──────┬───────┘
                        ↓
                 ┌──────────────┐
                 │    buffer    │  ← strict aliasing, endianness, size_line
                 └──────┬───────┘
                        ↓
                 ┌──────────────┐
                 │   mapa 2D    │  ← mockado → parser (contrato via ponteiros)
                 └──────┬───────┘
                        ↓
              ┌─────────┴────────────────────────────────────────────────────┐
              ↓                                                              ↓
          jogador                                           colisão (AABB simplificado, eixos separados)
              ↓
      câmera (dir + plane, vetores normalizados)
              ↓
      raio (um por coluna da tela)
              ↓
      DDA (grid traversal, deltaDist, sideDist, step)
              ↓
      ponto de impacto + side (X/Y → face N/S/L/O)
              ↓
      distância perpendicular (perpWallDist, correção fisheye)
              ↓
      projeção vertical (lineHeight, drawStart/End, clamping)
              ↓
      wallX (fração do impacto dentro do tile 1×1)
              ↓
      texX → texY (step, nearest-neighbor, espelhamento S/O)
              ↓
      textura (XPM, 4 faces, data_addr, size_line da textura)
              ↓
      renderização (coluna por coluna, teto/chão, double buffering)
              ↓
      integração (parser, RGB bitshift, bússola de spawn)
              ↓
      cleanup universal (ordem reversa, estados parciais, NULL-check)
              ↓
      Norminette + Valgrind + GDB + Franken-map + Simulação de Defesa
```

> **Pergunta inicial (Momento Sócrates 0):** Se você não controla o *byte* no endereço `0`, como pode confiar no *pixel* na coordenada `(x, y)`?

---

## O Contrato de Dados (Definir no Dia 1, não no Dia 36)

Antes de escrever código, você e o Fernando devem concordar:

```c
t_game
 ├── mlx, win, img          // sua responsabilidade
 ├── data_addr, bpp, size_line, endian  // buffer de render
 ├── char **map             // parser aloca, engine lê (somente leitura)
 ├── int map_width, map_height
 ├── double posX, posY, dirX, dirY, planeX, planeY  // sua responsabilidade
 ├── void *tex[4]           // NO, SO, WE, EA — você carrega, parser entrega paths
 ├── int tex_w, tex_h, tex_bpp, tex_sl, tex_end  // metadados das texturas
 ├── int floor_color, ceiling_color  // parser entrega R,G,B; você converte
 └── int keys[6]            // estado de teclas (W,A,S,D, ←, →)
```

**Regras de Ownership:**
- Quem aloca `map`? → Fernando. Quem libera? → Fernando (ou cleanup universal combinado).
- Quem aloca `tex[]`? → Você (via `mlx_xpm_file_to_image`). Quem libera? → Você.
- Quem aloca `img`? → Você. Quem libera? → Você.
- O renderer **nunca** altera `map`. O parser **nunca** altera `dir`/`plane`.
- Use **forward declarations** nos headers para evitar que o parser dependa de `mlx.h` indiretamente.

---

# Sprint 1 — Memória, MLX e Framebuffer (Dias 1–7)
**Objetivo:** Dominar MLX + framebuffer. Entregar: janela + buffer + mapa 2D mockado renderizado.

---

## Dia 1 — Autópsia da miniLibX e Makefile

### Estude
- [ ] X11
- [ ] display server
- [ ] processo cliente
- [ ] janela
- [ ] imagem
- [ ] framebuffer
- [ ] `mlx_init`
- [ ] `mlx_new_window`
- [ ] `mlx_new_image`
- [ ] `mlx_get_data_addr`
- [ ] `mlx_put_image_to_window`
- [ ] `mlx_loop`
- [ ] `mlx_hook`
- [ ] Como a miniLibX abstrai o X11 (ou Xext) no Linux
- [ ] O que é um *display connection*
- [ ] O que são *implicit rules* em um Makefile
- [ ] Como elas causam relinks silenciosos
- [ ] Diferença entre linkar biblioteca estática (`.a`) e incluir sub-makefile
- [ ] Quais flags de link (`-lXext -lX11 -lm`) o Makefile interno já assume
- [ ] O que acontece se você omitir alguma no seu Makefile

### Prática
- [ ] Descompacte `minilibx-linux.tgz` e leia o Makefile interno antes de escrever o seu.
- [ ] Abra `mlx.h`, o Makefile da MLX e a implementação da MLX.
- [ ] Siga o caminho: `mlx_init()` → connection/display → window → image.
- [ ] Configure o Makefile raiz com `all`, `clean`, `fclean`, `re`, `bonus`.
- [ ] Aplique `-Wall -Wextra -Werror`.
- [ ] Garanta compilação incremental sem relinks.
- [ ] Teste: `make` → `make` → `touch render.c` → `make` → `make clean` → `make`.
- [ ] Observe o comportamento dos timestamps.

### Perguntas Socráticas
1. O que exatamente é retornado por `mlx_init()`? Quem é dono da memória associada a esse retorno?
2. O que acontece se você destruir uma imagem antes de destruí-la da janela?
3. Uma `void *` retornada pela MLX representa necessariamente um buffer de pixels?
4. Por que a MLX não simplesmente entrega um `int *`?
5. Se o Makefile da MLX já compila a lib, por que seu Makefile raiz não pode simplesmente ignorá-lo e linkar direto?
6. Se você rodar `make` duas vezes seguidas e o segundo comando recompilar algo, quem é o culpado: o timestamp do `.c` ou a regra do `.a`?
7. Se Fernando alterar o parser amanhã, como seu Makefile garante que apenas os objetos modificados sejam recompilados sem que você precise entender o código dele?

### Testes — Como sei que deu certo?
- [ ] `make` compila sem erros e sem warnings.
- [ ] Rodar `make` duas vezes: a segunda deve dizer "Nothing to be done".
- [ ] `touch` em um `.c` e `make` recompila apenas esse objeto.
- [ ] `make clean` remove todos os `.o` e o binário.
- [ ] `make re` faz clean + all em sequência.
- [ ] O binário linka corretamente com a MLX (teste com `ldd ./cub3D` ou rodando).

---

## Dia 2 — Esqueleto de Diretórios + Git Flow

### Estude
- [ ] targets
- [ ] prerequisites
- [ ] timestamps
- [ ] `.o`
- [ ] linking
- [ ] `ar`
- [ ] `ranlib`
- [ ] `make`
- [ ] dependências
- [ ] relink
- [ ] Como estruturar `includes/`, `srcs/`, `libft/`, `srcs_parser/`
- [ ] Como a separação de pastas afeta o include path (`-I`)
- [ ] O que quebra se Fernando renomear uma pasta sem avisar
- [ ] Git: `git status`
- [ ] Git: `git add`
- [ ] Git: `git commit`
- [ ] Git: `git log`
- [ ] Git: `git diff`
- [ ] Git: `git branch`
- [ ] Git: `git switch`
- [ ] Git: `git merge`
- [ ] Git: `git stash`
- [ ] Git: `git checkout -b`
- [ ] Git: `git rebase`
- [ ] Git: `git pull --rebase`
- [ ] Git: `git push`
- [ ] Git: `git log --oneline --graph`
- [ ] Git: `git add -p`
- [ ] Git: `git merge --no-ff`

### Prática
- [ ] Estruture os diretórios: `includes/`, `srcs/`, `libft/`, `srcs_parser/`.
- [ ] Adicione `.gitignore` no Dia 1 (nunca no Dia 30): ignore `.o`, binários, `*.a`.
- [ ] Inicialize repositório Git.
- [ ] Crie branch `feature/window-buffer`.
- [ ] Faça commits pequenos e atômicos.
- [ ] Integre com a branch principal.
- [ ] Teste `git rebase main` antes de abrir PR.
- [ ] Nunca commite `.o`/binários.
- [ ] Trabalhe em branch separada da de Fernando.

### Perguntas Socráticas
1. Se o Makefile da MLX já compila a lib, por que o seu Makefile raiz não pode simplesmente ignorá-lo e linkar direto? O que muda no `re` se você não respeitar essa hierarquia de dependência?
2. Por que faz sentido que `t_game` seja alocada uma única vez (stack ou heap?) e passada por ponteiro em toda a engine, em vez de ser global?
3. Como você planeja evitar conflitos com o código do Fernando? Qual estratégia de branching minimiza dores de cabeça na integração?
4. Como a separação `includes/`, `srcs/`, `libft/`, `srcs_parser/` afeta o include path (`-I`) do seu Makefile? O que quebra se Fernando renomear uma pasta sem avisar?

### Testes — Como sei que deu certo?
- [ ] `git log --oneline` mostra commits atômicos e mensagens claras.
- [ ] `git branch` mostra sua branch separada da main.
- [ ] `git status` não mostra arquivos não rastreados que deveriam estar no `.gitignore`.
- [ ] Ao alterar um `.c` e rodar `make`, apenas esse arquivo é recompilado.
- [ ] O projeto compila com as flags `-Wall -Wextra -Werror` sem erros.

---

## Dia 3 — O Buffer de Imagem + Strict Aliasing

### Estude
- [ ] ponteiro
- [ ] `void *`
- [ ] cast
- [ ] `char *`
- [ ] `int *`
- [ ] bytes
- [ ] endereço
- [ ] `bpp` (bits_per_pixel)
- [ ] `size_line`
- [ ] endianess
- [ ] strict aliasing
- [ ] O que a miniLibX retorna quando você chama a função de criação de imagem
- [ ] O que são `bpp`, `size_line` e `endian`
- [ ] Aritmética de ponteiros em C: `*(addr + offset)` vs `addr[offset]`
- [ ] O que significa *stride* (ou pitch) em um framebuffer

### Prática
- [ ] Defina `t_game` com ponteiros MLX, janela, imagem, buffer.
- [ ] Discuta com Fernando os campos que ele preencherá vs. os seus.
- [ ] Crie uma imagem pequena (ex: 8×8) para testar.
- [ ] Extraia `data_addr` com `mlx_get_data_addr`.
- [ ] Deduzir a fórmula do offset: `y * size_line + x * (bpp / 8)`.
- [ ] Desenhe fisicamente a memória:

```text
memória:

+----+----+----+----+----+----+
| p0 | p1 | p2 | p3 | p4 | p5 |
+----+----+----+----+----+----+
```

- [ ] Depois imagine uma imagem 3×2. Onde está `(0,0)`, `(1,0)`, `(2,0)`, `(0,1)`, `(1,1)`, `(2,1)`?
- [ ] Pinte um único pixel no centro da janela.

### Perguntas Socráticas
1. Se a imagem é conceitualmente uma matriz `[y][x]`, por que a memória não precisa ser uma matriz?
2. Se `data` aponta para o primeiro byte da imagem, qual é a diferença conceitual entre `data + 1` e `(int *)data + 1`?
3. Por que usar `size_line` em vez de `largura * (bpp / 8)`? O que acontece se você assumir que são iguais?
4. Se `bpp` for 32, você está manipulando 4 bytes por pixel. Por que, dependendo do `endian`, a ordem dos bytes RGBA muda, e como isso afeta seu cálculo se você escrever um `int` de uma vez só?
5. Se `data` aponta para uma região de memória pertencente à MLX, o que aconteceria se você chamasse `free(data)`? Por que "ter um ponteiro para uma memória" não significa "ser dono daquela memória"?
6. Se você armazenar `data_addr` como `int *` na sua struct para escrever pixels mais rápido, qual é o risco de *aliasing* se a MLX esperava que você escrevesse byte a byte? E se o `bpp` fosse 24 em vez de 32, por que `int *` se tornaria uma sentença de morte para o seu buffer?

### Testes — Como sei que deu certo?
- [ ] O pixel no centro da janela aparece na cor correta.
- [ ] Alterar `x` e `y` na fórmula move o pixel para a posição esperada.
- [ ] Se você pintar `(0,0)`, `(width-1,0)`, `(0,height-1)`, `(width-1,height-1)`, os 4 cantos aparecem corretos.
- [ ] O Valgrind não acusa `invalid write` ao pintar um pixel.
- [ ] `mlx_put_image_to_window` exibe a imagem na janela.

---

## Dia 4 — Escrever Pixels + Double Buffering

### Estude
- [ ] aritmética de ponteiros
- [ ] `sizeof`
- [ ] offset linear
- [ ] stride
- [ ] row-major order
- [ ] Double buffering conceitual
- [ ] Por que desenhar pixel por pixel diretamente na janela pode ser ruim
- [ ] Diferença entre "renderizar na janela" e "renderizar em memória → apresentar imagem"

### Prática
- [ ] Crie uma imagem pequena, por exemplo: 8 × 8.
- [ ] Pinte: um pixel, uma linha, uma coluna, uma diagonal, um quadrado.
- [ ] Pinte metade do buffer com uma cor (teto) e a outra metade com outra (chão).
- [ ] Empurre a imagem para a janela de uma só vez no final.
- [ ] Teste o conceito de double buffering:

```text
buffer
   ↓
renderizar tudo
   ↓
imagem pronta
   ↓
MLX coloca na janela
```

### Perguntas Socráticas
1. Você precisa deduzir a relação:

```
(x, y)
   ↓
posição linear
   ↓
endereço
```

   Não memorize uma fórmula. Derive-a.
2. Se `size_line` for maior que `width * bytes_per_pixel`, o que isso significa? Por que usar somente `width * bpp` pode ser uma abstração perigosa?
3. Se você fizer isso pixel a pixel com `mlx_pixel_put`, qual o custo assintótico de syscalls por frame? Por que o buffer de imagem é uma solução de complexidade diferente?

### Testes — Como sei que deu certo?
- [ ] A linha horizontal aparece reta, sem desvios.
- [ ] A diagonal vai de um canto ao outro sem buracos.
- [ ] O quadrado preenchido tem as bordas alinhadas.
- [ ] Teto e chão ocupam exatamente metade da tela cada.
- [ ] Não há flicker ao atualizar a imagem.
- [ ] Rode `valgrind --leak-check=summary`: nenhum leak após fechar a janela.

---

## Dia 5 — O Contrato de Dados (`t_game`)

### Estude
- [ ] O que é um *opaque pointer*
- [ ] Por que a MLX retorna `void *` para a conexão e para a janela
- [ ] Struct padding e alinhamento de memória em arquiteturas x86_64
- [ ] Padrão de projeto *Repository* (aplicado a uma struct central)
- [ ] Forward declarations para evitar que o parser dependa de `mlx.h` indiretamente
- [ ] Quem é o dono da memória da imagem
- [ ] O que acontece se você criar uma nova imagem a cada frame
- [ ] Quando a imagem anterior deve ser destruída

### Prática
- [ ] Refatore o código para usar uma struct `t_app` (ou `t_game`) contendo ponteiros para `mlx`, janela, imagem e buffer.
- [ ] Garanta que todas as funções recebam ou acessem essa struct de forma organizada.
- [ ] Desenhe no papel os campos que você precisa vs. os campos que Fernando vai preencher.
- [ ] Defina quais campos pertencem exclusivamente à sua metade (buffer, vetores de câmera, texturas carregadas) e quais são "só leitura" para você (mapa, cores, paths).
- [ ] Pense em como você impede que sua engine tente renderizar antes que Fernando tenha preenchido o mapa.

### Perguntas Socráticas
1. Se `t_game` contém ponteiros para MLX, janela, imagem e mapa, quem deve ser responsável por liberar cada recurso? E o que acontece se duas funções diferentes assumirem que são "donas" do mesmo ponteiro?
2. Se a janela é um recurso do sistema operacional e a struct é um recurso seu, quem deve ser responsável por `free` em quem: a engine libera a janela, ou a janela libera a engine?
3. Se você e Fernando precisam compartilhar uma struct, e ele incluir `<stdio.h>` no parser e você não quiser incluir no engine, como evitar que os headers dele poluam os seus?
4. Por que é perigoso armazenar a matriz do mapa como `char **` na mesma struct que guarda ponteiros da MLX?

### Testes — Como sei que deu certo?
- [ ] A struct compila sem erros.
- [ ] Todas as funções recebem `t_game *` (ou `t_app *`), não usam variáveis globais.
- [ ] O programa ainda abre a janela e pinta teto/chão usando a struct.
- [ ] Não há memory leak ao fechar (teste com Valgrind).
- [ ] Se você tentar acessar um campo da struct antes de inicializá-lo, o compilador avisa (use `-Wall -Wextra -Werror`).

---

## Dia 6 — Primeiro Mapa Mockado

### Estude
- [ ] arrays 2D
- [ ] índice `[linha][coluna]`
- [ ] tile
- [ ] coordenadas mundo
- [ ] coordenadas tela
- [ ] Como representar um mapa 2D em C: array de strings, array de inteiros, struct com dimensões
- [ ] Prós e contras de cada representação
- [ ] Tile-based rendering: world space vs screen space
- [ ] O que é *blitting* e por que você está fazendo isso manualmente

### Prática
- [ ] Declare uma matriz fixa `1`/`0` só para teste.
- [ ] Use: `0 = espaço`, `1 = parede`.
- [ ] Desenhe quadrados 64×64 (ou outro TILE_SIZE configurável).
- [ ] Itere a matriz 2D com laços, multiplicando índices `[x][y]` pelo tile size.
- [ ] Mapeie: `map[y][x]` → `pixel_x = x * TILE`, `pixel_y = y * TILE`.
- [ ] Generalize a função de "desenhar quadrado preenchido" para que o `tile_size` seja uma constante única usada em todo o projeto (não um número mágico espalhado).

### Perguntas Socráticas
1. Se seu tile tem 64px e seu mapa é uma matriz `char [10][10]`, qual a fórmula para converter um índice de matriz `(row, col)` para coordenadas de tela `(x, y)`? E o inverso?
2. Você deve conseguir transformar `map[y][x]` em `pixel_x = x * TILE`, `pixel_y = y * TILE` sem confundir `x/y` do mapa com `x/y` da tela.
3. Por que usar `char` para representar 0 e 1 é suficiente agora, mas será insuficiente quando Fernando entregar o parser?
4. Se você desenhar o chão e depois as paredes iterando a matriz uma vez, o que acontece se o jogador estiver sobrepondo visualmente uma parede? Qual princípio de ordenação de renderização você está violando?

### Testes — Como sei que deu certo?
- [ ] O mapa aparece visualmente como uma grade de quadrados.
- [ ] Paredes (`1`) têm cor diferente do chão (`0`).
- [ ] A grade está alinhada (linhas retas, colunas retas).
- [ ] Alterar a matriz no código altera o mapa visual corretamente.
- [ ] O tile size é uma constante `#define`, não um número mágico espalhado.
o for usar checkboxes em tabelas, a sinta
---

## Dia 7 — Primeiro Incremento + Fuga Limpa

### Prática
- [ ] Execute `./cub3D` e visualize:

```text
┌────────────────────────┐
│ ██████████████████████ │
│ █                    █ │
│ █    █████           █ │
│ █    █               █ │
│ █         █████      █ │
│ █                    █ │
│ ██████████████████████ │
└────────────────────────┘
```

- [ ] Tudo renderizado **através do buffer de imagem**. Nada de raycasting ainda.
- [ ] Configure a escuta de eventos específicos do X11 (como `DestroyNotify`).
- [ ] Mapeie ESC e "X" vermelho para uma **única** função `close_game()`.
- [ ] A função de saída deve passar pelo GDB em modo TUI sem acusar vazamento ou segfault.

### Perguntas Socráticas
1. Por que um único `close_game()` chamado pelos dois hooks é mais seguro do que duas funções de saída separadas? O que pode dar errado se cada uma liberar memória de forma diferente?
2. O que exatamente as funções de inicialização alocam, e por que isso importa quando você for caçar leaks no Valgrind mais tarde?

### Critério de Aprovação do Sprint 1
Você consegue explicar, sem olhar documentação:
1. Quem aloca o quê?
2. Qual ponteiro representa o buffer?
3. Como `(x,y)` vira endereço?
4. O que `size_line` representa?
5. Quando a imagem é enviada para a janela?
6. Quem destrói cada recurso?

### Testes — Como sei que deu certo?
- [ ] O mapa 2D aparece completo na janela.
- [ ] Apertar ESC fecha a janela imediatamente sem segfault.
- [ ] Clicar no "X" vermelho fecha a janela imediatamente sem segfault.
- [ ] Valgrind não reporta `definitely lost` após fechar por ESC.
- [ ] Valgrind não reporta `definitely lost` após fechar pelo "X".
- [ ] GDB: `break close_game`, `run`, `continue`, fechar janela, o breakpoint é atingido.

**🔥 Momento Sócrates Sprint 1:** Se `data` aponta para uma região de memória pertencente à MLX, o que aconteceria se você chamasse `free(data)`? Por que "ter um ponteiro para uma memória" não significa "ser dono daquela memória"?

---

# Sprint 2 — Jogador, Input, Física e Câmera (Dias 8–14)
**Objetivo:** Transformar o mapa em um pequeno **mundo interativo**. Entregar: mini-engine 2D funcional (player + WASD + colisão + rotação).

---

## Dia 8 — Sistema de Coordenadas + Agente

### Estude
- [ ] coordenadas cartesianas
- [ ] ponto vs vetor
- [ ] coordenadas discretas
- [ ] coordenadas contínuas
- [ ] Como adicionar variáveis de posição X e Y usando ponto flutuante, não inteiros

### Prática
- [ ] Adicione `posX`, `posY` como `double` na struct.
- [ ] Desenhar jogador como um pequeno quadrado diferente sobre o mapa.
- [ ] Separe mentalmente três espaços diferentes:

```text
Mapa:
[3][5]

Mundo:
(5.37, 3.82)

Tela:
(343, 245)
```

### Perguntas Socráticas
1. Se o jogador está em `(5.37, 3.82)`, qual célula do mapa representa sua posição? E se ele estiver em `(5.99, 3.99)`?
2. Por que a posição do jogador deveria ser `float`/`double` desde o primeiro dia, mesmo desenhando em um grid de inteiros? O que quebraria matematicamente mais tarde se você usasse `int`?

### Testes — Como sei que deu certo?
- [ ] O jogador aparece na posição correta do mapa.
- [ ] Alterar `posX`/`posY` no código move o jogador visualmente.
- [ ] A posição em float mapeia corretamente para a célula do mapa (teste com valores como `5.99` → célula `5`, não `6`).

---

## Dia 9 — Input Contínuo (Key Press / Key Release)

### Estude
- [ ] event loop
- [ ] key press
- [ ] key release
- [ ] estado
- [ ] polling
- [ ] callback
- [ ] Event-driven vs polling em sistemas de janelas
- [ ] Diferença entre reagir a um evento de tecla isolado e manter um array de estados de tecla lido a cada frame pelo `mlx_loop_hook`
- [ ] Por que a segunda abordagem evita "solavancos"

### Prática
- [ ] Configure a MLX para registrar quando uma tecla foi pressionada (estado = 1) e quando foi solta (estado = 0).
- [ ] Implemente um sistema de flags para teclas (WASD, setas). Atualize as variáveis de forma independente.
- [ ] O `mlx_loop_hook` lê o estado e atualiza a cada frame.

### Perguntas Socráticas
1. Por que `key_press` sozinho produz movimento "aos solavancos"? Por que guardar estado resolve isso?
2. O que acontece se `W = 1` e `D = 1` simultaneamente?
3. Qual é a diferença estrutural entre reagir a um evento de tecla isolado e manter um array de estados de tecla lido a cada frame pelo `mlx_loop_hook`?

### Testes — Como sei que deu certo?
- [ ] Segurar W move o jogador para cima continuamente, sem solavancos.
- [ ] Segurar A + W move o jogador na diagonal.
- [ ] Soltar W para o movimento imediatamente.
- [ ] Pressionar duas teclas simultaneamente resulta em movimento diagonal.
- [ ] Não há flicker ao manter teclas pressionadas.

---

## Dia 10 — Loop do Jogo + Movimento

### Estude
- [ ] game loop
- [ ] update
- [ ] render
- [ ] frame
- [ ] estado
- [ ] O que é um *game loop* e por que ele precisa de um timestep

### Prática
- [ ] Configure o `mlx_loop_hook` para que, a cada ciclo contínuo do programa, ele limpe o buffer, atualize a posição do personagem baseado nas teclas pressionadas e redesenhe tudo na tela sem piscar (flicker).
- [ ] Modelo mental:

```text
evento
   ↓
estado
   ↓
update
   ↓
render
   ↓
próximo frame
```

- [ ] Comece com: `posição + velocidade = nova posição`.
- [ ] Depois pense em W/A/S/D como vetores.

### Perguntas Socráticas
1. Se você colocar a lógica de movimento dentro do callback de `KeyPress`, o que acontece quando o usuário segura a tecla?
2. Por que redesenhar diretamente na janela a cada frame (sem buffer intermediário) tende a piscar, e como sua solução do Sprint 1 já resolve isso?

### Testes — Como sei que deu certo?
- [ ] O jogador se move suavemente, frame a frame.
- [ ] Não há flicker (piscar) ao mover.
- [ ] A velocidade é constante (não depende de quantas vezes o evento de tecla foi disparado).
- [ ] O buffer é limpo a cada frame antes de redesenhar.

---

## Dia 11 — Movimento Diagonal + Vetores

### Estude
- [ ] vetor 2D
- [ ] magnitude
- [ ] normalização
- [ ] velocidade
- [ ] Movimentação com rotação: alterar o ângulo do jogador com setas esquerda/direita
- [ ] Atualizar o vetor de direção e o plano da câmera

### Prática
- [ ] W/A/S/D como vetores.
- [ ] Corrigir velocidade diagonal: `vx = velocidade`, `vy = velocidade` pode produzir velocidade diagonal maior que a horizontal.
- [ ] Normalize o vetor de movimento diagonal.

### Pergunta Socrática
1. Se o jogador andar simultaneamente em X e Y, por que `vx = velocidade`, `vy = velocidade` pode produzir velocidade diagonal maior que a velocidade horizontal?

### Testes — Como sei que deu certo?
- [ ] Andar para cima (W) e andar para a direita (D) têm a mesma velocidade perceptível.
- [ ] Andar na diagonal (W+D) não é visivelmente mais rápido que andar em linha reta.
- [ ] Use um cronômetro: percorrer 10 células na diagonal deve levar o mesmo tempo que 10 células em linha reta (aproximadamente).

---

## Dia 12 — Colisão Numérica

### Estude
- [ ] AABB (Axis-Aligned Bounding Box) simplificado para grid
- [ ] Casting de `float` para `int` em C: truncamento vs arredondamento
- [ ] O que é *epsilon* e por que a colisão em ponto flutuante precisa dele
- [ ] colisão por eixo
- [ ] sliding
- [ ] player radius

### Prática
- [ ] Dada a posição em pixels/float do jogador, deduza a fórmula que converte isso em índices `[linha][coluna]` da matriz.
- [ ] Antes de aplicar o movimento, checar se o destino é parede.
- [ ] Mova X e Y separadamente para permitir deslizar ao longo de paredes.
- [ ] Defina um raio para o jogador (ex.: 0.2 células) e implemente colisão considerando esse raio, não apenas o ponto central.

### Perguntas Socráticas
1. A posição do jogador é `(float x, float y)`. O mapa é `int map[row][col]`. Qual a operação matemática exata para descobrir em qual célula da matriz o jogador está? Por que `x / tile_size` funciona, mas `(x + epsilon) / tile_size` pode ser mais seguro?
2. Por que é interessante tratar X e Y separadamente? O que acontece se o passo do jogador for maior que uma célula (tunelamento)?
3. Se o jogador está em `(1.0f, 1.0f)` e o tile size é 64, ele está exatamente na borda de quatro células. Para qual célula sua fórmula de índice o mapeia? Isso é deterministicamente seguro?
4. Se você checar apenas o centro do jogador, o que acontece se o jogador tiver um raio (hitbox) maior que zero?

### Testes — Como sei que deu certo?
- [ ] O jogador não atravessa paredes.
- [ ] Ao andar em direção a uma parede em ângulo, o jogador desliza ao longo dela (não trava completamente).
- [ ] O jogador não fica preso em cantos.
- [ ] Teste com velocidade alta: o jogador não "tunela" através de paredes finas.
- [ ] GDB: breakpoint na função de colisão. Inspecione `map[y][x]` quando o jogador tenta se mover para uma parede.

---

## Dia 13 — Vetor de Direção + Plano da Câmera (FOV)

### Estude
- [ ] vetor direção
- [ ] vetor unitário
- [ ] trigonometria
- [ ] `sin`
- [ ] `cos`
- [ ] radianos
- [ ] Vetores unitários em R²
- [ ] Matriz de rotação 2D: `[[cos θ, -sin θ], [sin θ, cos θ]]`
- [ ] O que é um *camera plane* em raycasting e por que ele não é um ângulo, mas um segmento de reta
- [ ] O plano da câmera não é "mais uma direção"
- [ ] Ele deve representar a largura visual da câmera

### Prática
- [ ] Adicione `dirX`, `dirY` como vetor normalizado.
- [ ] Adicione `planeX`, `planeY` perpendiculares à direção.
- [ ] Derive geometricamente o plano da câmera para um FOV de 66°.

### Perguntas Socráticas
1. Se `dir = (1, 0)`, qual direção representa? Se você rotacionar 90°? O comprimento de `(dirX, dirY)` deve ser quanto? Por quê?
2. Se `dirX` e `dirY` representam para onde o jogador olha, por que eles precisam ser normalizados? O que acontece com o FOV se o vetor de direção tiver magnitude 2?
3. O plano da câmera é perpendicular à direção. Se `dir = (0, -1)` (olhando para Norte), qual deve ser o vetor `plane` para um FOV de 66°? Como você deriva isso geometricamente sem decorar fórmulas?
4. Qual é a relação entre o comprimento do vetor-plano e o campo de visão final em graus?

### Testes — Como sei que deu certo?
- [ ] `sqrt(dirX*dirX + dirY*dirY)` é aproximadamente 1.0.
- [ ] O produto escalar `dirX*planeX + dirY*planeY` é aproximadamente 0 (perpendicular).
- [ ] Alterar `dir` e `plane` manualmente no código altera a "direção da câmera" corretamente.

---

## Dia 14 — Rotação + Entregável Sprint 2

### Estude
- [ ] matriz de rotação 2D
- [ ] radianos
- [ ] seno/cosseno
- [ ] rotação de vetores
- [ ] Não memorize a matriz. Derive-a geometricamente.

### Prática
- [ ] Aplique a mesma matriz de rotação tanto em `dir` quanto em `plane`.
- [ ] Implemente rotação contínua com as setas esquerda/direita.
- [ ] Garanta que o raycasting use a direção atualizada corretamente.

### Perguntas Socráticas
1. Quando o jogador gira, você aplica a mesma matriz de rotação tanto em `dir` quanto em `plane`. Por que rotacionar o plano é tão importante quanto rotacionar a direção?
2. O que aconteceria visualmente se você rotacionasse apenas a direção?

### Entregável do Sprint 2
Você deve ter:

```text
MAPA 2D
   ↓
PLAYER
   ↓
WASD
   ↓
MOVIMENTO
   ↓
COLISÃO
   ↓
ROTAÇÃO
```

### Testes — Como sei que deu certo?
- [ ] O jogador se move com WASD sobre o mapa mockado.
- [ ] Colide corretamente com paredes (não atravessa).
- [ ] Desliza ao longo de paredes em ângulo.
- [ ] Roda a câmera com as setas esquerda/direita.
- [ ] Sem flicker.
- [ ] Sem leaks reportados (Valgrind).
- [ ] GDB: breakpoint em `close_game`, `run`, fechar janela, `bt` mostra pilha de chamadas correta.

**🔥 Momento Sócrates Sprint 2:** Se você decidir, por otimização prematura, armazenar a posição do jogador como `int` em vez de `float`, fazendo a conversão apenas na hora de desenhar, a colisão funciona perfeitamente com inteiros. No entanto, no Sprint 3, quando tentar rotacionar a câmera com senos e cossenos, o compilador trunca seus ângulos. Por que a decisão de usar `int` para posição no Sprint 2 destruiu a arquitetura do Sprint 3? Qual invariante de precisão você violou?

---

# Sprint 3 — Raycasting e DDA (Dias 15–21)
**Objetivo:** Coração matemático do cub3D. Entregar: raycaster 2D funcional, 800 raios por frame, distância perpendicular, lado da colisão.

---

## Dia 15 — Geometria do Raio

### Estude
- [ ] ray
- [ ] origin
- [ ] direction
- [ ] parametric equation
- [ ] intersection
- [ ] Algoritmo DDA (Digital Differential Analyzer) para percorrer uma grade 2D
- [ ] Conceitos de raio, direção, deltaDist, sideDist

### Prática
- [ ] Implemente uma função que, dada uma posição e um ângulo, retorne a distância até a parede mais próxima e o lado atingido (N/S/E/W).
- [ ] Mentalmente: `raio = origem + direção × distância`.

### Pergunta Socrática
1. Se você conhece posição do jogador e direção do jogador, como representa matematicamente qualquer ponto sobre o raio?

### Testes — Como sei que deu certo?
- [ ] A função retorna uma distância positiva para qualquer direção.
- [ ] Para uma parede diretamente à frente, a distância é aproximadamente a distância euclidiana esperada.
- [ ] Para direções sem parede (mapa aberto), o raio não entra em loop infinito (proteja com limite de passos).

---

## Dia 16 — Camera Plane + FOV Completo

### Estude
- [ ] FOV
- [ ] camera plane
- [ ] perpendicularidade
- [ ] produto escalar
- [ ] Mapeamento de coordenadas de tela para espaço da câmera: `cameraX = 2 * x / width - 1`
- [ ] Por que disparar um raio por coluna de pixels e não por linha
- [ ] O que é *perpendicular distance* e por que a distância euclidiana pura cria distorção

### Prática
- [ ] Para uma janela de largura W, lance W raios uniformemente distribuídos no FOV.
- [ ] Armazene as distâncias em um array.
- [ ] `rayDir = dir + plane * cameraX`.

### Perguntas Socráticas
1. Se sua tela tem 800 colunas, por que `cameraX` varia de -1 a +1? O que significa -1 geometricamente em relação ao plano da câmera?
2. Se `cameraX = 0`, o raio aponta para onde? Se `cameraX = -1`, o que isso representa no mundo?
3. Por que usar a coordenada normalizada da câmera é melhor do que simplesmente incrementar um ângulo?

### Testes — Como sei que deu certo?
- [ ] O array de distâncias tem exatamente `width` elementos.
- [ ] A distância na coluna central (`x = width/2`) é a menor quando o jogador olha reto para uma parede.
- [ ] As distâncias nas bordas da tela são maiores que no centro (quando olhando para uma parede frontal).

---

## Dia 17 — Um Único Raio + DeltaDist

### Estude
- [ ] grid traversal
- [ ] distância até linha vertical
- [ ] distância até linha horizontal
- [ ] `deltaDist`
- [ ] Por que `deltaDistX` é essencialmente o inverso da direção do raio
- [ ] O que acontece matematicamente (e no seu código) quando `rayDirX` é exatamente `0`
- [ ] Como evitar divisão por zero sem "chutar" um valor qualquer

### Prática
- [ ] Faça **um raio** antes de pensar em 800.
- [ ] Calcule `deltaDistX = fabs(1 / rayDirX)`, `deltaDistY = fabs(1 / rayDirY)`.
- [ ] Determine `step` (+1 ou -1) baseado no sinal de `rayDirX`/`rayDirY`.

### Perguntas Socráticas
1. Se o raio aponta para `(1, 0.5)`, ele cruza uma linha vertical a cada 1 unidade em X. Quanto ele precisa viajar em X para cruzar uma linha vertical? E quanto em Y para cruzar uma horizontal? Por que `deltaDistX = |1 / rayDirX|`?
2. Imagine:

```text
┌───┬───┬───┐
│   │   │   │
├───┼───┼───┤
│ P ───────►│
├───┼───┼───┤
```

   Qual fronteira o raio alcançará primeiro? A vertical ou horizontal? Como saber **sem desenhar todos os pontos do raio**?

### Testes — Como sei que deu certo?
- [ ] Para `rayDirX = 0`, o código não dá segfault (teste divisão por zero).
- [ ] `stepX` é +1 quando `rayDirX > 0`, -1 quando `rayDirX < 0`.
- [ ] `deltaDistX` e `deltaDistY` são sempre positivos.
- [ ] Para um raio olhando para (1, 0), `deltaDistY` é infinito (ou muito grande), e o loop só incrementa X.

---

## Dia 18 — DDA Completo

### Estude
- [ ] `deltaDist`
- [ ] `step`
- [ ] `sideDist`
- [ ] grid traversal
- [ ] O loop de salto do DDA

### Prática
- [ ] Estruture um laço `while` que compara `sideDistX` com `sideDistY`.
- [ ] Incremente a menor delas até que as coordenadas batam em uma célula cujo valor seja 1 (parede).
- [ ] A estrutura conceitual:

```text
calcular delta
      ↓
determinar step
      ↓
determinar distância inicial
      ↓
comparar X/Y
      ↓
avançar menor distância
      ↓
verificar parede
      ↓
repetir
```

### Perguntas Socráticas
1. Como o sinal de `rayDirX`/`rayDirY` determina se `step` é `+1` ou `-1`?
2. Por que o cálculo do `sideDist` inicial depende da posição fracionária do jogador dentro da célula atual, e não só da posição inteira?
3. Por que o DDA não precisa recalcular a interseção geométrica completa a cada célula?

### Testes — Como sei que deu certo?
- [ ] O loop termina ao encontrar uma parede (não loop infinito).
- [ ] `mapX`/`mapY` nunca saem dos bounds do mapa mockado.
- [ ] O número de iterações é proporcional ao tamanho do mapa, não à resolução.
- [ ] GDB: `watch mapX` ou `watch mapY` durante o loop para verificar incrementos corretos.

---

## Dia 19 — Side (Lado da Colisão)

### Estude
- [ ] Como uma única variável booleana (`side`), combinada com o sinal de `rayDirX`/`rayDirY`, é suficiente para determinar qual das 4 texturas (N, S, L, O) usar mais tarde
- [ ] Tente deduzir a tabela lógica sozinho antes de pesquisar

### Prática
- [ ] Registre `side` (0 = X, 1 = Y) no momento da colisão.
- [ ] Isso posteriormente determinará a face da parede.

### Perguntas Socráticas
1. Se o raio atravessou uma fronteira vertical, o que isso diz sobre a face da parede atingida?
2. Se o raio atravessou uma fronteira horizontal, o que isso diz sobre a face?
3. Como `side` combinada com o sinal de `rayDirX`/`rayDirY` determina N/S vs L/O?

### Testes — Como sei que deu certo?
- [ ] Ao bater em uma parede à direita, `side` indica face correta.
- [ ] Ao bater em uma parede acima, `side` indica face correta.
- [ ] Imprima `side` e a posição do hit para 4 direções cardinais: verifique se a lógica está consistente.

---

## Dia 20 — 800 Raios + Debug Visual

### Estude
- [ ] normalized device coordinates
- [ ] camera coordinate
- [ ] FOV
- [ ] ray direction
- [ ] Varredura da tela inteira

### Prática
- [ ] Loop `for (x = 0; x < width; x++)`.
- [ ] Mapeie `0..799` para `-1..+1`.
- [ ] **Opcional mas vital:** Desenhe as linhas dos raios no seu minimapa 2D. Cada raio deve terminar exatamente na célula atingida.
- [ ] **Bloco Pareto — GDB Avançado (Claude):**
  ```bash
  watch deltaDistX
  break dda.c:42
  info locals
  display sideDistX
  ```
  Use `watch` para pegar o exato momento em que `deltaDist` vira `inf` ou `nan`.

### Perguntas Socráticas
1. Por que usar a coordenada normalizada da câmera é melhor do que simplesmente incrementar um ângulo?
2. Em que condição exata (mapa, ângulo) você consegue forçar uma divisão por zero no cálculo de `deltaDist`? Reproduza esse caso de propósito antes que o avaliador o encontre por você.

### Testes — Como sei que deu certo?
- [ ] 800 raios são lançados por frame sem crash.
- [ ] Cada raio retorna distância e lado da colisão.
- [ ] No minimapa 2D (se implementado), os raios terminam exatamente nas células de parede.
- [ ] GDB: `watch` em `deltaDistX` não dispara alarme falso (não é `nan`/`inf` indevidamente).

---

## Dia 21 — Distância Perpendicular + Entregável Sprint 3

### Estude
- [ ] fisheye effect
- [ ] perpendicular distance
- [ ] projeção
- [ ] produto escalar
- [ ] A distância real do raio gera paredes arredondadas
- [ ] Qual cálculo final, baseado no lado da colisão (X ou Y), encontra a distância projetada na câmera (`perpWallDist`)

### Prática
- [ ] Calcule `perpWallDist` baseado no `side`.
- [ ] Corrija fisheye.

### Pergunta Socrática
1. Por que a distância que o raio percorreu não necessariamente representa a distância visual da parede?
2. Se a distância dobra, por que a altura projetada deve diminuir aproximadamente pela metade?

### Entregável do Sprint 3
Você possui:

```text
PLAYER
   ↓
CAMERA
   ↓
RAY
   ↓
DDA
   ↓
WALL HIT
```

### Testes — Como sei que deu certo?
- [ ] Paredes frontais aparecem retas (sem curvatura de fisheye).
- [ ] Paredes perpendiculares ao jogador têm distância consistente em toda a extensão.
- [ ] O array de distâncias por coluna está preenchido para todas as colunas.
- [ ] Valgrind não reporta `invalid read/write` durante o DDA.

**🔥 Momento Sócrates Sprint 3:** Dentro do loop DDA, você declara `double deltaDistX = fabs(1 / rayDirX)`. Em um canto perfeitamente alinhado, `rayDirX` pode ser zero. Em C, `1.0 / 0.0` resulta em `inf` (IEEE 754), e `fabs(inf)` é `inf`. O DDA ainda funciona porque a comparação `sideDistX < sideDistY` nunca será verdadeira para o eixo X. Mas e se `rayDirX` não fosse exatamente zero, mas um valor denormalizado próximo de zero devido a precisão de ponto flutuante? Como o uso de `float` vs `double` nesse cálculo específico pode silenciosamente corromper seu loop após milhares de iterações?

---

# Sprint 4 — Projeção 3D (Dias 22–28)
**Objetivo:** A grande ilusão. Entregar: Wolfenstein-like sem textura, com teto/chão coloridos e clamping seguro.

---

## Dia 22 — Distância Perpendicular e Altura Projetada

### Estude
- [ ] fisheye effect
- [ ] perpendicular distance
- [ ] projeção
- [ ] produto escalar
- [ ] perspectiva
- [ ] projeção
- [ ] inverse proportionality
- [ ] Por que a distância *direta* do jogador até a parede (não a perpendicular) produz o efeito de curvatura nas bordas da tela
- [ ] O que exatamente `perpWallDist` corrige que a distância euclidiana simples não corrige

### Prática
- [ ] `lineHeight = int(h / perpWallDist)`.
- [ ] Parede perto → grande. Parede longe → pequena.

### Perguntas Socráticas
1. Se `perpWallDist` é 1, a parede ocupa a altura inteira da tela. Se for 0.5, qual a altura? E se for 10?
2. Por que a relação é de proporção inversa e não linear?
3. Se a distância dobra, por que a altura projetada deve diminuir aproximadamente pela metade?

### Testes — Como sei que deu certo?
- [ ] Uma parede a 1 unidade de distância ocupa a altura total da tela.
- [ ] Uma parede a 2 unidades ocupa metade da altura.
- [ ] Uma parede a 4 unidades ocupa um quarto da altura.
- [ ] As proporções são consistentes em toda a largura da tela.

---

## Dia 23 — Clamping (drawStart / drawEnd)

### Estude
- [ ] clipping
- [ ] bounds checking
- [ ] integer limits
- [ ] Se `lineHeight` for maior que a altura da tela, seu `drawStart` pode virar negativo
- [ ] Por que escrever esse valor diretamente em um índice de array sem clamping é um convite a segfault

### Prática
- [ ] Calcule `drawStart` e `drawEnd`.
- [ ] Limite-os dentro dos bounds do buffer.
- [ ] Verifique antes de escrever.

### Perguntas Socráticas
1. Quando você encosta o "nariz" na parede, `perpWallDist` tende a zero e `lineHeight` explode. Como você garante que não escreva em `buffer[-1]` ou `buffer[HEIGHT + 1]`?
2. Por que fazer `for (y = drawStart; y <= drawEnd; y++)` sem antes garantir que `drawStart` e `drawEnd` estão dentro do buffer pode ser perigoso?
3. O que impediria um `drawEnd` incorreto de escrever além da memória alocada? Qual camada da sua arquitetura deveria ser responsável por garantir essa invariável?

### Testes — Como sei que deu certo?
- [ ] Encostar o nariz na parede não causa segfault.
- [ ] `drawStart` nunca é negativo; `drawEnd` nunca excede `HEIGHT - 1`.
- [ ] Teste com parede a distância 0.1: a parede preenche a tela inteira sem crash.
- [ ] Teste com parede muito distante: `lineHeight` não zera (mínimo de 1 pixel).

---

## Dia 24 — Teto e Chão

### Prática
- [ ] Cada coluna terá: teto → parede → chão.
- [ ] Preencha os pixels acima de `drawStart` com a cor do teto.
- [ ] Preencha os pixels abaixo de `drawEnd` com a cor do chão.
- [ ] Use cores hexadecimais configuráveis para teste.

### Pergunta Socrática
1. Por que preencher a coluna inteira pode ser mais simples conceitualmente do que tentar desenhar primeiro apenas a parede?
2. Se você preencher teto e chão antes de desenhar a parede, qual a vantagem? E se fizer depois?

### Testes — Como sei que deu certo?
- [ ] A tela mostra teto (cor A), parede (cor B), chão (cor C).
- [ ] As cores não "vazam" uma sobre a outra.
- [ ] A proporção teto/parede/chão muda conforme a distância da parede.
- [ ] Não há linhas horizontais de cor errada entre as seções.

---

## Dia 25 — Pipeline por Coluna + Performance

### Estude
- [ ] complexidade
- [ ] frame
- [ ] número de raios
- [ ] número de passos DDA
- [ ] Pipeline por coluna

### Prática
- [ ] Enxergue o esqueleto mental do motor:

```text
for cada coluna X:

    construir raio
        ↓
    executar DDA
        ↓
    obter distância
        ↓
    calcular altura
        ↓
    calcular limites
        ↓
    desenhar coluna
```

### Pergunta Socrática
1. Se sua tela possui 1280 colunas e cada raio atravessa 20 células, aproximadamente quantos passos DDA você executa por frame? E por segundo a 60 FPS?

### Testes — Como sei que deu certo?
- [ ] O loop por coluna não recalcula coisas que poderiam ser pré-calculadas (ex: `cameraX` pode ser pré-calculado?).
- [ ] O tempo de frame é consistente (não varia wildamente).
- [ ] Use `clock()` ou `gettimeofday` para medir: o loop de 1280 colunas deve levar menos de 16ms (para 60 FPS).

---


## Dia 25 — Pipeline por Coluna + Performance

### Estude
- [ ] complexidade
- [ ] frame
- [ ] número de raios
- [ ] número de passos DDA
- [ ] Pipeline por coluna
- [ ] Otimizações de raycasting: pré-cálculo de vetores de direção
- [ ] Tabelas de seno/cosseno
- [ ] Redução de operações por raio
- [ ] Como reduzir o número de raios lançados sem perder qualidade visual

### Prática
- [ ] Enxergue o esqueleto mental do motor:

```text
for cada coluna X:

    construir raio
        ↓
    executar DDA
        ↓
    obter distância
        ↓
    calcular altura
        ↓
    calcular limites
        ↓
    desenhar coluna
```

- [ ] Avalie onde seu raycasting gasta mais tempo (use `clock()` ou `gettimeofday`).
- [ ] Identifique pontos de otimização.
- [ ] Implemente uma otimização simples: renderize em metade da largura e estique a imagem final, se necessário. Compare desempenho.

### Pergunta Socrática
1. Se sua tela possui 1280 colunas e cada raio atravessa 20 células, aproximadamente quantos passos DDA você executa por frame? E por segundo a 60 FPS?
2. Vale a pena pré-calcular algo para cada coluna da tela? O que pode ser reutilizado entre frames?
3. Como essa otimização afeta a memória? Você precisa de um buffer intermediário?

### Testes — Como sei que deu certo?
- [ ] O loop por coluna não recalcula coisas que poderiam ser pré-calculadas (ex: `cameraX` pode ser pré-calculado?).
- [ ] O tempo de frame é consistente (não varia wildamente).
- [ ] Use `clock()` ou `gettimeofday` para medir: o loop de 1280 colunas deve levar menos de 16ms (para 60 FPS).
- [ ] Renderizar em meia-resolução e esticar: o FPS aumenta perceptivelmente?
- [ ] A qualidade visual em meia-resolução é aceitável?

---

## Dia 26 — Z-Buffer + Coordenada de Impacto

### Estude
- [ ] Buffer de profundidade (z-buffer) e sua utilidade para texturização e sprites
- [ ] Onde armazenar a coordenada de impacto: array separado? struct por coluna?
- [ ] Como armazenar a coordenada exata de onde o raio atingiu a parede (não apenas a distância)
- [ ] Isso será necessário para texturização e futuramente para sprites/portas (z-buffer)

### Prática
- [ ] Além do array de distâncias, armazene também a coordenada exata de onde o raio atingiu a parede.
- [ ] Armazene `wallX` (fração do impacto dentro do tile 1×1).

### Pergunta Socrática
1. Onde armazenar essa coordenada de impacto? Um array separado? Uma struct por coluna? Como isso afeta a legibilidade?
2. Se você criar uma struct para representar cada raio (com distância, lado, coordenada de impacto), e armazenar um array dessas structs alocado dinamicamente, como garantir que todos os campos sejam inicializados? O que acontece se você esquecer de inicializar um ponteiro dentro dessa struct?

### Testes — Como sei que deu certo?
- [ ] A coordenada de impacto é um valor `double` entre 0 e 1 (fração dentro do tile).
- [ ] Para uma parede frontal, o impacto varia de 0 a 1 conforme a posição X do raio.
- [ ] O valor é consistente ao se mover paralelamente à parede.
- [ ] Não há memory leak ao alocar/desalocar o array de structs (se usar).

---

## Dia 27 — Sombreamento por Distância

### Estude
- [ ] Desenho de paredes com gradiente de cor baseado na distância
- [ ] Como calcular uma cor interpolada
- [ ] Como evitar cálculos de cor por pixel se a altura da parede for grande
- [ ] Existe alguma otimização de preenchimento?

### Prática
- [ ] Aplique um sombreamento às paredes: quanto mais longe, mais escuro.
- [ ] Não use textura ainda.
- [ ] Preencha faixas verticais vs. desenhar pixel a pixel.

### Pergunta Socrática
1. O que é mais custoso: calcular o deslocamento no buffer a cada pixel ou pré-calcular ponteiros para o início de cada linha? Como isso se relaciona com `size_line`?

### Testes — Como sei que deu certo?
- [ ] Paredes próximas são claras; paredes distantes são escuras.
- [ ] O gradiente é suave, não em degraus visíveis.
- [ ] A mesma parede, vista de diferentes distâncias, muda de tom consistentemente.
- [ ] O sombreamento não reduz o FPS drasticamente.

---

## Dia 28 — Primeiro FPS Renderer + Entregável Sprint 4

### Prática
- [ ] Você deve conseguir olhar para:

```text
██████████████████████
████      ████████████
███        ███████████
██          ██████████
██          ██████████
███        ███████████
████      ████████████
██████████████████████
```

- [ ] E perceber: **isso já é uma engine 3D, mesmo sem textura.**
- [ ] **Bloco Pareto — Valgrind (Claude):**
  ```bash
  valgrind --leak-check=summary ./cub3D
  ```
  Rode todo dia. Reserve `--leak-check=full --show-leak-kinds=all` para antes de cada entrega semanal.

### Testes — Como sei que deu certo?
- [ ] O jogador navega em tempo real sem travamentos.
- [ ] Paredes próximas são altas; distantes são baixas.
- [ ] Não há fisheye visível.
- [ ] Teto e chão estão preenchidos com cores distintas.
- [ ] Valgrind `--leak-check=summary` não reporta leaks após fechar.
- [ ] O FPS é estável (teste com `clock()` ou `gettimeofday`).

**🔥 Momento Sócrates Sprint 4:** Você está escrevendo pixels dentro de um buffer que possui largura, altura, `bpp` e `size_line`. O que impediria um `drawEnd` incorreto de escrever além da memória alocada? Qual camada da sua arquitetura deveria ser responsável por garantir essa invariável?

---

# Sprint 5 — Texturização (Dias 29–35)
**Objetivo:** Adicionar texturas nas 4 faces. Entregar: engine 3D texturizada com UV mapping correto.

---

## Dia 29 — Imagens MLX + Carregamento

### Estude
- [ ] XPM
- [ ] `mlx_xpm_file_to_image`
- [ ] image buffer
- [ ] texture metadata
- [ ] Formato XPM e como a miniLibX carrega texturas
- [ ] Diferença entre textura e imagem de janela
- [ ] Quem deve ser o dono da textura carregada
- [ ] Como evitar carregar a mesma textura múltiplas vezes
- [ ] O ciclo de vida de uma imagem na MLX: carga, uso, destruição

### Prática
- [ ] Carregar 4 texturas hardcoded (caminhos fixos).
- [ ] Extrair `data_addr`, `width`, `height`, `bpp`, `size_line`, `endian` de cada uma.
- [ ] Carregue uma textura simples (um quadrado colorido) e exiba-a como um sprite estático na tela para entender o fluxo.
- [ ] Estruture a carga das 4 texturas obrigatórias sem causar vazamentos na alocação.

### Pergunta Socrática
1. Qual a diferença entre "imagem MLX" e "dados de pixels da imagem"?
2. Se a terceira textura falhar depois que as duas primeiras já foram carregadas, quem destrói as duas primeiras?
3. Se você carregar 4 texturas de 64×64, cada uma com 32bpp, quantos bytes de memória você está alocando implicitamente? Por que isso importa quando o avaliador rodar Valgrind?
4. A função da MLX que carrega imagens retorna `void *`. Se ela retornar `NULL`, como sua engine deve se comportar? Você pode simplesmente continuar e crashar no primeiro frame?
5. Se cada textura tem seu próprio `bpp`, `size_line` e `endian`, por que você pode (ou não pode) assumir que todos esses valores serão idênticos aos do seu buffer de tela?

### Testes — Como sei que deu certo?
- [ ] As 4 texturas carregam sem erro.
- [ ] Cada textura tem `data_addr` válido (não NULL).
- [ ] As dimensões extraídas correspondem às dimensões reais dos arquivos XPM.
- [ ] Se um arquivo XPM for inválido, o programa trata o erro (não segfault).
- [ ] Valgrind não reporta leaks após carregar e destruir as texturas.

---

## Dia 30 — `wallX` (Fração do Impacto)

### Estude
- [ ] ponto de impacto
- [ ] fractional coordinate
- [ ] modulo/fração
- [ ] coordenadas UV
- [ ] Como calcular o valor exato (em ponto flutuante) de onde a parede foi atingida dentro daquele bloco matemático 1×1 do mapa

### Prática
- [ ] Calcule `wallX` — onde dentro do tile 1×1 o raio bateu.
- [ ] Exemplo: se bateu no meio exato da parede, gera o valor 0.5.

### Pergunta Socrática
1. Usando a posição inicial do jogador, o comprimento do raio e a direção dele, como calcular o valor exato de onde a parede foi atingida dentro daquele bloco matemático 1×1 do mapa?
2. Se a parede foi atingida no lado Norte, a coordenada de impacto cresce para leste ou oeste? Como você pode padronizar isso para os quatro lados?

### Testes — Como sei que deu certo?
- [ ] `wallX` é sempre um valor entre 0.0 e 1.0.
- [ ] Para uma parede frontal, variar a posição do raio na horizontal varia `wallX` linearmente.
- [ ] O valor é consistente ao se mover paralelamente à parede.

---

## Dia 31 — `texX` + Espelhamento

### Estude
- [ ] `texX`
- [ ] posição proporcional
- [ ] coluna da textura
- [ ] Como mapear um ponto na parede para um pixel na textura
- [ ] Cálculo da coordenada X a partir da posição de impacto

### Prática
- [ ] `texX = int(wallX * texWidth)`.
- [ ] Corrigir espelhamento em faces Sul e Oeste.
- [ ] Monte a tabela antes de codificar:

| Side | Direção do raio | Textura | Condição de espelho |
|------|-----------------|---------|---------------------|
| X=0  | rayDirX > 0     | EA      | não                 |
| X=0  | rayDirX < 0     | WE      | sim                 |
| Y=1  | rayDirY > 0     | SO      | não                 |
| Y=1  | rayDirY < 0     | NO      | sim                 |

### Pergunta Socrática
1. Se a textura tem 64 pixels de largura e o raio bate em 25% da parede, qual região da textura deveria ser utilizada? E o que precisa acontecer se o resultado não for um inteiro?
2. Texturas aplicadas nos lados Sul e Oeste tendem a aparecer espelhadas por causa da direção do plano cartesiano. Qual condição simples deve ser aplicada ao cálculo do `texX` para inverter a leitura da textura caso o raio atinja essas faces específicas?

### Testes — Como sei que deu certo?
- [ ] `texX` está sempre entre 0 e `texWidth - 1`.
- [ ] Faces Sul e Oeste mostram a textura na orientação correta (não espelhada).
- [ ] Andar paralelo a uma parede faz a coluna da textura "deslizar" suavemente.

---

## Dia 32 — `texY` + Step de Textura

### Estude
- [ ] nearest-neighbor scaling
- [ ] interpolation conceitual
- [ ] texture coordinate
- [ ] texture step
- [ ] Amostragem de textura: como obter a cor de um pixel específico da textura a partir do buffer da imagem
- [ ] Como acessar pixels da textura sem violar os limites

### Prática
- [ ] `step = 1.0 * texHeight / lineHeight`.
- [ ] Loop de preenchimento copiando pixels da textura para o buffer.
- [ ] Desenhe uma parede texturizada completa, amostrando a coluna da textura e esticando-a verticalmente conforme a altura da parede.

### Pergunta Socrática
1. Se a parede possui 200 pixels na tela e a textura possui 64 pixels, quanto a coordenada Y da textura deve avançar por pixel da tela? Derive, não decore.
2. O buffer da textura tem o mesmo formato do buffer da janela? Como você pode acessar pixels da textura sem violar os limites?

### Testes — Como sei que deu certo?
- [ ] A parede texturizada aparece esticada corretamente (não comprimida nem distorcida).
- [ ] Texturas próximas mostram detalhes; distantes ficam pixeladas (nearest-neighbor).
- [ ] Não há segfault ao acessar pixels da textura.
- [ ] `texY` nunca sai dos bounds da textura.

---

## Dia 33 — As 4 Faces + Inversão de Espelho

### Estude
- [ ] Norte/Sul/Leste/Oeste
- [ ] Como associar cada lado (N/S/E/W) a uma textura diferente
- [ ] Como selecionar dinamicamente de qual dos 4 vetores de textura a cor será lida
- [ ] Onde armazenar as texturas: struct com quatro ponteiros? Array?
- [ ] Como isso se relaciona com a struct que o Fernando vai preencher

### Prática
- [ ] Selecione textura correta baseada em `side` + sinal de `rayDirX`/`rayDirY`.
- [ ] Use a tabela do Dia 31.

### Pergunta Socrática
1. Por que `side` sozinho não é suficiente para escolher entre quatro texturas? Que segunda informação você precisa?
2. Se você guardar quatro ponteiros para quatro buffers de textura dentro de uma struct, quem possui esses buffers? Se a struct for copiada por valor, você duplicou as texturas ou apenas duplicou os ponteiros? Que consequência isso tem para `free()`?

### Testes — Como sei que deu certo?
- [ ] Cada face da parede (N/S/E/W) mostra a textura correta.
- [ ] Não há espelhamento incorreto.
- [ ] Ao andar em um corredor, as paredes laterais mostram texturas diferentes conforme a orientação.

---

## Dia 34 — Profiling e Otimização (DeepSeek)

### Estude
- [ ] Otimizações de raycasting: pré-cálculo de vetores de direção
- [ ] Tabelas de seno/cosseno
- [ ] Redução de operações por raio
- [ ] Como reduzir o número de raios lançados sem perder qualidade visual
- [ ] Renderizar em resolução menor e escalar

### Prática
- [ ] Use `clock()` ou `gettimeofday` para medir tempo de frame.
- [ ] Identifique gargalos.
- [ ] Implemente uma otimização simples: renderize em metade da largura e estique a imagem final, se necessário. Compare desempenho.
- [ ] Refatore a função de desenho de paredes para preencher a faixa vertical de uma vez, se possível, ou minimizar chamadas de função por pixel.

### Pergunta Socrática
1. Vale a pena pré-calcular algo para cada coluna da tela? O que pode ser reutilizado entre frames?
2. Como essa otimização afeta a memória? Você precisa de um buffer intermediário?

### Testes — Como sei que deu certo?
- [ ] O FPS aumenta perceptivelmente com a otimização.
- [ ] A qualidade visual em meia-resolução é aceitável.
- [ ] Não há artefatos visuais na imagem esticada.
- [ ] O profiling identifica corretamente o gargalo (ex: DDA, desenho, etc.).

---

## Dia 35 — Incremento Completo + Entregável Sprint 5

### Prática
- [ ] Você deve possuir:

```text
Mapa mockado
      ↓
Raycasting
      ↓
DDA
      ↓
perpWallDist
      ↓
projeção
      ↓
wallX
      ↓
texX
      ↓
texY
      ↓
textura
```

- [ ] **Bloco Pareto — Valgrind (Claude):**
  ```bash
  valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./cub3D
  ```
  `--track-origins=yes` é especialmente útil para rastrear memória não inicializada.

### Testes — Como sei que deu certo?
- [ ] 4 texturas corretas, UV mapping funcionando.
- [ ] Sem espelhamento incorreto.
- [ ] Paredes próximas são detalhadas; distantes são pixeladas.
- [ ] Não há segfault ao carregar texturas inválidas.
- [ ] Valgrind `--leak-check=full` limpo.
- [ ] O FPS é estável.

**🔥 Momento Sócrates Sprint 5:** Você tem um ponteiro `int *texture` para os pixels de uma textura carregada. Para acessar o pixel `(texX, texY)`, você escreve `texture[texY * tex_width + texX]`. Durante um stress-test, `texY` está sendo calculado como negativo em alguns frames. O C não verifica bounds. Se `texY` for -1, em qual endereço de memória você está escrevendo/lendo? Como o Valgrind detectaria isso se o acesso fosse leitura, mas talvez não detectasse se fosse apenas cálculo de offset sem dereferenciação imediata?

---

# Sprint 6 — Integração, Memória, Norma e Avaliação (Dias 36–40)
**Objetivo:** Tornar o software defensável. Entregar: versão candidata à avaliação.

---

## Dia 36 — Contrato com o Fernando + Merge Real

### Estude
- [ ] Ponteiros como contrato: passagem por referência vs cópia de dados
- [ ] Bit shifting: `(R << 16) | (G << 8) | B`
- [ ] Inicialização condicional de vetores baseada em caracteres (`N`, `S`, `E`, `W`)
- [ ] Tabelas de lookup (Kimi) em vez de 4 `if`s
- [ ] Como conectar a struct gigante que o Fernando populou ao núcleo do seu motor de renderização sem copiar dados desnecessariamente, usando apenas ponteiros
- [ ] Conversão hexadecimal de cores

### Prática
- [ ] Definir interface `t_game` compartilhada.
- [ ] Remover `hardcoded_map`.
- [ ] Conectar `parser → game → renderer`.
- [ ] Conversão hexadecimal de cores: `(R << 16) | (G << 8) | B`. Use máscara `0xFF` se necessário.
- [ ] Bússola de spawn (`N/S/E/W`). Use **tabela de lookup** (Kimi) em vez de 4 `if`s:
  ```c
  // Exemplo conceitual
  t_camera cameras[4] = {
      {0, -1, 0.66, 0},  // N
      {0, 1, -0.66, 0},  // S
      {-1, 0, 0, 0.66},  // W
      {1, 0, 0, -0.66},  // E
  };
  ```

### Perguntas Socráticas
1. Fernando entrega `t_rgb {int r, g, b;}`. A MLX espera um `int` packed. Por que `(R << 16) | (G << 8) | B` funciona apenas em little-endian? O que muda se você precisar compatibilizar com big-endian?
2. O mapa tem `N` como posição inicial. Como você mapeia isso para `dirX, dirY, planeX, planeY` sem usar um `if` para cada letra? (Pense em tabelas de lookup).
3. Se você conectar seu motor à struct do Fernando usando ponteiros, o que acontece se ele der `free` na matriz do mapa antes de você chamar `mlx_loop_end`?
4. Se trocar a origem dos dados exige reescrever o raycaster, seu acoplamento está errado.

### Testes — Como sei que deu certo?
- [ ] O programa roda com o mapa do parser (não mais mockado).
- [ ] As cores do teto e chão do `.cub` aparecem corretamente.
- [ ] A posição inicial do jogador corresponde à letra N/S/E/W do mapa.
- [ ] A direção inicial está correta para cada bússola.
- [ ] Não há segfault ao trocar de mapa.

---

## Dia 37 — Substituição do Mock + Defesa das Texturas

### Estude
- [ ] Graceful degradation vs fail-fast
- [ ] Destruição de recursos em ordem inversa de alocação
- [ ] `mlx_destroy_image`, `mlx_destroy_window`, `mlx_destroy_display`
- [ ] Se o caminho de uma textura no `.cub` for inválido, em que exato ponto do seu pipeline você precisa capturar esse erro
- [ ] O que acontece se você só descobrir isso no meio do primeiro frame de desenho

### Prática
- [ ] Integrar texturas e cores do `.cub`.
- [ ] Se caminho de textura for inválido, capture o erro **antes** do primeiro frame.
- [ ] Se a textura Norte falha ao carregar, seu programa deve imprimir "Error\n" e sair. Mas e se a textura Sul já tinha sido carregada com sucesso antes? Como sua função de saída sabe o que já foi alocado sem causar double-free?

### Pergunta Socrática
1. Em que exato ponto do seu pipeline (antes do `mlx_loop_hook` começar) você precisa capturar erro de textura inválida?
2. Se a textura Norte falha ao carregar, seu programa deve imprimir "Error\n" e sair. Mas e se a textura Sul já tinha sido carregada com sucesso antes? Como sua função de saída sabe o que já foi alocado sem causar double-free?
3. Por que a ordem de destruição importa? O que acontece se você destruir a `mlx` antes de destruir as imagens que ela gerenciava?

### Testes — Como sei que deu certo?
- [ ] Mapa com textura inválida: programa imprime "Error" e sai limpo (sem segfault, sem leak).
- [ ] Mapa com textura válida: programa roda normalmente.
- [ ] Valgrind não reporta leaks quando uma textura falha e o programa sai.

---

## Dia 38 — Cleanup Universal + Estados Parciais (Claude)

### Estude
- [ ] Graceful degradation vs fail-fast
- [ ] Destruição de recursos em ordem inversa de alocação
- [ ] `mlx_destroy_image`, `mlx_destroy_window`, `mlx_destroy_display`
- [ ] Como projetar uma única função de encerramento universal (`close_game(struct *game)`) que saiba exatamente o que foi alocado até aquele milissegundo para liberar tudo na ordem reversa
- [ ] Como representar no estado da struct: "este recurso ainda não foi alocado"

### Prática
- [ ] Criar `close_game()` que sabe "o que já foi alocado até agora".
- [ ] Ordem reversa de destruição.
- [ ] Estratégia: Checar se ponteiro é `NULL` antes de liberar. Ou use flags por recurso.
- [ ] Cenário crítico:

```text
mlx_init       ✓
window         ✓
image          ✓
texture NO     ✓
texture SO     ✓
texture WE     ✗
texture EA     —
```

  O cleanup não pode assumir que tudo existe.

### Pergunta Socrática
1. Como representar no estado da struct: "este recurso ainda não foi alocado"?
2. Se você projetar `close_game(t_game *game)` que libera tudo, e ela é chamada tanto no fluxo normal de saída quanto no callback do botão "X" da janela, por que a ordem de liberação dentro de uma única função pode ser segura em um caminho de execução e fatal em outro? Como o conceito de *ownership* resolve esse dilema?
3. Se o usuário fecha no "X" vermelho, o callback recebe um ponteiro para sua struct. Esse ponteiro é o mesmo da `main`? Como você garante que não há data race entre o callback do X11 e o fluxo principal?

### Testes — Como sei que deu certo?
- [ ] Fechar no ESC: nenhum leak.
- [ ] Fechar no "X": nenhum leak.
- [ ] Forçar erro no meio da inicialização (ex: textura inválida): nenhum leak, nenhum double-free.
- [ ] GDB: `break close_game`, verificar que todos os recursos são liberados na ordem correta.

---

## Dia 39 — Valgrind + GDB + Norminette

### Estude
- [ ] Norminette: limites de linhas, funções, variáveis por função
- [ ] Structs de contexto para evitar passagem massiva de parâmetros
- [ ] Funções estáticas (`static`) para encapsulamento de arquivo
- [ ] Valgrind: `definitely lost`, `indirectly lost`, `still reachable`
- [ ] GDB TUI: inspeção de matrizes dinâmicas (`char **`) no momento do free
- [ ] Como verificar se `mlx_destroy_display` realmente libera o `void *mlx`
- [ ] Headers: include guards, forward declarations, dependências circulares
- [ ] O que a norma diz sobre comentários e por que menos é mais

### Prática
- [ ] Rode Valgrind em pelo menos 3 cenários diferentes (fechamento normal via ESC, fechamento via X vermelho, e um mapa com erro proposital).
  ```bash
  valgrind --leak-check=full --show-leak-kinds=all ./cub3D map.cub
  valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./cub3D maps/erro.cub
  ```
- [ ] GDB TUI. Breakpoint antes de `map[y][x]`. Pergunte: `map != NULL? y válido? map[y] != NULL? x válido?`
- [ ] Norminette. Funções ≤ 25 linhas, ≤ 5 variáveis locais, ≤ 4 parâmetros.
- [ ] Extrair lógica para structs de contexto e funções `static`.
- [ ] Se seu DDA tem 15 variáveis locais, como agrupá-las em structs **sem alocar dinamicamente**?
- [ ] Limpeza de headers: declare todas as funções externas permitidas (`open`, `read`, `malloc`, `free`, etc.) explicitamente no seu `.h`.
- [ ] Revisão de código: ler seu próprio código como se fosse de um estranho, 3 dias depois de escrevê-lo.

### Perguntas Socráticas
1. Se seu DDA tem 15 variáveis locais, como agrupá-las em structs sem alocar dinamicamente (evitando leaks)?
2. Por que declarar funções auxiliares como `static` é uma estratégia de encapsulamento e não apenas uma exigência da norma?
3. Se você quebrar o DDA em 5 funções, cada uma recebendo 3 ponteiros para structs, qual é o custo de performance dessa indireção? É mensurável ou prematura a otimização de inline manual?
4. Se você incluir `mlx.h` dentro do seu `cub3d.h`, e o parser de Fernando também incluir `cub3d.h`, o parser depende da MLX mesmo não usando gráficos. Como quebrar essa dependência usando forward declarations ou headers separados?
5. Por que a norma penaliza funções longas? Qual princípio de engenharia de software (além da 42) justifica isso?

### Testes — Como sei que deu certo?
- [ ] Valgrind: `--leak-check=full --show-leak-kinds=all` não reporta `definitely lost`.
- [ ] Valgrind: `--track-origins=yes` não reporta uso de memória não inicializada.
- [ ] Norminette: nenhuma função excede 25 linhas.
- [ ] Norminette: nenhuma função excede 5 variáveis locais.
- [ ] Norminette: nenhuma função excede 4 parâmetros.
- [ ] O projeto compila sem warnings com `-Wall -Wextra -Werror`.
- [ ] `make` não gera relinks.

---

## Dia 40 — Franken-Map + Simulação de Avaliação (Gemini + ChatGPT)

### Estude
- [ ] Fuzzing mental: o que acontece com mapa 3×3 cercado?
- [ ] Mapas com espaços em branco válidos dentro do labirinto
- [ ] Como explicar suas escolhas técnicas sob pressão
- [ ] Franken-map: teste espaços vazios gigantes e mapas mínimos 3×3

### Prática
- [ ] **Franken-Map (Gemini):** Teste mapas mínimos (3×3 cercado), espaços em branco gigantes, corredores invisíveis. O motor renderiza "breu" corretamente? Não dá segfault?
- [ ] **Simulação de defesa:**
  - "Por que você usa `perpWallDist` em vez da distância do raio?" → Responda matematicamente.
  - "Por que `size_line` existe se você já sabe a largura da imagem?" → Layout de memória.
  - "O que acontece se `mlx_xpm_file_to_image()` falhar?" → Caminho de erro.
  - "Quem libera o mapa?" → Responda sem hesitar.
  - "Se eu mudar a velocidade de rotação, onde você mexe?" → Variável exata, radianos por frame.
  - "Por que `double` em vez de `float`?" → Não é só precisão. É legibilidade, padrão da comunidade, e evitar denormals silenciosos.
- [ ] **Git tag:** `git tag v1.0-defesa` antes da avaliação.
- [ ] Faça uma compilação limpa, rode Valgrind uma última vez, e prepare o repositório para submissão.

### Perguntas Socráticas
1. Se o mapa tem uma linha de 1000 caracteres e outra de 5, como seu renderizador lida com o espaço vazio? Ele tenta acessar `map[y][x]` onde `x` está fora da linha curta?
2. Se um avaliador pedir para mudar a velocidade de rotação, qual variável exata ele deve tocar? A resposta está óbvia no código ou escondida em uma fórmula mágica?
3. Se o projeto exige que o programa "não trave", mas você encontrar um mapa que causa loop infinito no DDA, qual é a sua linha de defesa: pré-validação no parser ou guarda no loop da engine?
4. Depois de 40 dias, você projeta `close_game(t_game *game)` que libera tudo. Ela é chamada tanto no fluxo normal de saída quanto no callback do "X" da janela. No entanto, dentro do callback, você chama `free(game)` e depois tenta acessar `game->mlx` para destruir a janela. Por que a ordem de liberação dentro de uma única função pode ser segura em um caminho de execução e fatal em outro? Como o conceito de *ownership* resolve esse dilema?

### Testes — Como sei que deu certo?
- [ ] Mapa 3×3 cercado: não trava, renderiza corretamente.
- [ ] Mapa com espaços em branco gigantes: renderiza "breu" (teto/chão), não segfault.
- [ ] Mapa mínimo válido: roda sem erros.
- [ ] Mapa inválido: detecta erro antes de crashar.
- [ ] Valgrind limpo em todos os cenários.
- [ ] Norminette limpo.
- [ ] `git tag v1.0-defesa` criado.
- [ ] Você consegue responder todas as perguntas da simulação de defesa sem hesitar.

**🔥 Momento Sócrates Final:** Depois de 40 dias mexendo em ponteiros de imagem, texturas e mapas, qual foi o momento em que você mais teve certeza de estar liberando memória corretamente — e depois descobriu, via Valgrind, que estava errado? O que esse gap entre "achar que entendeu" e "provar que funciona" te ensina sobre como você vai revisar código em produção daqui a seis meses, numa fintech?

---

## Resumo Consolidado: Comandos Essenciais (Pareto 80/20)

### Git
```bash
git checkout -b feature/nome
git add -p
git commit -m "tipo: descrição"
git rebase main
git log --oneline --graph
git stash
git tag v1.0-defesa
```

### GDB
```bash
gdb -tui ./cub3D
break arquivo:linha
run
next / step
print variavel
backtrace
watch variavel
info locals
display variavel
continue
```

### Valgrind
```bash
valgrind --leak-check=summary ./cub3D              # diário
valgrind --leak-check=full --show-leak-kinds=all ./cub3D map.cub   # semanal
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./cub3D maps/erro.cub
```

---

## Checklist de Saída por Sprint

| Sprint | Entregável Funcional | Prova de Vida |
|--------|---------------------|---------------|
| 1 | Janela, pinta teto/chão, fecha limpo | `git log --oneline` atômico; explicar strict aliasing |
| 2 | Mapa 2D mockado, player move sem atravessar | GDB: breakpoint na colisão, inspeção de `map[y][x]` |
| 3 | 800 raios, DDA sólido, perpWallDist correto | GDB: `watch deltaDistX` durante canto alinhado |
| 4 | Paredes 3D coloridas, teto/chão, sem fisheye | Valgrind: nenhum `invalid write`; profiling opcional |
| 5 | 4 texturas corretas, UV mapping, sem espelho | Valgrind: `--leak-check=full` limpo |
| 6 | Parser integrado, Norminette limpo, zero leaks | Franken-map passa; simulação de defesa gravada |

---

> **Palavra final do mentor:**
> Não procure a resposta. Procure a pergunta certa. Se você conseguir explicar por que `size_line` existe, por que `double` é padrão, por que `free` na ordem errada mata o programa, e por que um callback de X11 não pode liberar a struct antes de destruir os recursos que ela aponta — o código será apenas a consequência natural do seu raciocínio. Agora, volte para o **Momento Sócrates da Sprint 1** e escreva a resposta. Não continue antes disso.
"""
