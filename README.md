*This project was created as part of the 42 curriculum by febranda e rosousa-.*

---

# Cub3d

[🇧🇷 Versão em Português](#portugues) | [🇺🇸 English Version](#english)

## <a id="portugues"></a>🇧🇷 Versão em Português

Aqui começa o seu texto em português.

---

*Este projeto foi criado como parte do currículo da 42 por febranda e rosousa-.*

---

## 📝 Descrição

[..em construção]

---

## 🛠️ Instruções

### Pré-requisitos
O projeto foi desenvolvido em **C** e utiliza a biblioteca gráfica `MiniLibX`. Certifique-se de ter `gcc`, `make` e as dependências da X11 instaladas (se estiver no Linux).

### Compilação
Para compilar o projeto, é necessário executar `make` na pasta da `MiniLibx`. Após isso execute o seguinte comando na raiz do repositório:

```bash
make
```

### Execução

[..em construção]


```

### Controles

Uma vez que a janela esteja aberta:
- **Setas:** 
- **ESC** ou **X da janela:** Fechar o programa limpo (sem leaks).

## 📚 Recursos e Referências
Este projeto foi construído com base em conceitos matemáticos sólidos e otimizações de computação. Abaixo estão os tópicos e referências utilizados:

### Teoria Matemática


### Lógica de Programação
- **Mapeamento (Mapping):** Conversão de coordenadas de tela (pixels 0-800) para o plano complexo (ex: -2.0 a +2.0) usando interpolação linear.
- **Forma Algébrica:** Optou-se por usar $(x^2 - y^2) + 2xyi$ em vez da forma trigonométrica/polar para maximizar a performance da CPU.
- **Passagem por Referência:** Correção de bugs críticos na manipulação de structs complexas passando ponteiros para funções de renderização.

### Referências Externas
- **Equaciona Com Paulo Pereira:** Conjunto de aulas sobre números complexos ministradas através do canal do youtube.
- **Beginner's Guide To The Desktop:** série de vídeos explicando componentes básicos do desktop:
	- Aula 1: Introduction
	- Aula 2: X11, Display Manager and Windows Manager
- **fractol: render beautiful fractals in C (mandelbrot and julia set)**. Video do canal do Youtube "Oceano" onde o mesmo descomplexifica a estrutura do projeto e oferece um passo a passo de como realizá-lo.
- **Introduction to the minilibX : a simple X-Window programming API in C:** Vídeo do canal do Youtube "Oceano" explicando o funcionamento da biblioteca minilibX.
- **Draw a Pollock painting with the minilibX:** Vídeo do canal do Youtube "Oceano" explicando o funcionamento da biblioteca minilibX.
- **The Mandelbrot Fractal Explained!:** Vídeo do canal do Youtube "The Art of Code" explicando o conjunto Mandelbrot.
- **Como os Números Imaginários Foram Inventados:** Video do canal do Youtube "Veritassium em Português" explicando a origem dos números imaginários.
- **MinilibX Documentation:** Documentação oficial.
- **harm-smits.github.io/42docs:** Documentação complementar da biblioteca minilibx.
- **Wikipedia:** Artigos sobre "Mandelbrot set" e "Julia set" para constantes interessantes.


## 🤖 Uso de Inteligência Artificial

### 0. IA's
Para este projeto foram usadas sobretudo as IA's da empresa Alphabet, especificamente a NotebookLM e o chatbot Gemini por conta do seu recurso "estudo guiado". Nesse sentido as IA's foram utilizadas como ferramenta de tutoria pedagógica e debugging em raros casos, auxiliando nas seguintes tarefas específicas:

### 1. Explicação de Conceitos Matemáticos:
- Ajudou a visualizar a diferença entre a iteração do Mandelbrot ($Z$ começa em 0, $C$ muda) e Julia ($Z$ é o pixel, $C$ é constante).
- Clarificou por que sqrt deve ser evitado em loops de renderização (comparação com raio ao quadrado).

### 2. Depuração de Código (Debugging):
- Correção de lógica de ponteiros na função handle_pixel (erro de passagem por valor vs. referência).
- Diagnóstico de erros do Valgrind relacionados a memória não inicializada na mlx_new_window.

### 3. Otimização e Boas Práticas:
- Sugestão de substituir comparações de strings (ft_strcmp) dentro do loop de renderização por flags inteiras (int type).
- Explicação sobre o uso correto da palavra-chave static em arquivos .c versus .h.

### 4. README.md
- A IA também auxiliou na criação deste README.md, sobretudo na geração das fontes utilizadas.
---

## <a id="english"></a>English Version 🇺🇸
[🇧🇷 Versão em Português](#portugues)

Here starts your text in English.

---
*This project was created as part of the 42 curriculum by rosousa-.*

---
## 📝 Description

[..em construção]