*This project was created as part of the 42 curriculum by rosousa-.*

---

# Fract-ol

[🇧🇷 Versão em Português](#portugues) | [🇺🇸 English Version](#english)

## <a id="portugues"></a>🇧🇷 Versão em Português

Aqui começa o seu texto em português.

---

*Este projeto foi criado como parte do currículo da 42 por rosousa-.*

---

## 📝 Descrição

O **Fract-ol** é um projeto de exploração gráfica focado na renderização matemática de conjuntos de fractais. O objetivo principal é criar um software capaz de gerar imagens psicodélicas e infinitas baseadas em fórmulas matemáticas de números complexos, permitindo ao usuário interagir e explorar esses mundos matemáticos.

Este projeto serve como uma introdução à programação gráfica utilizando a biblioteca **MiniLibX**, abordando conceitos fundamentais como:
- Manipulação de pixels e buffers de imagem.
- Aritmética de números complexos.
- Otimização de cálculos matemáticos intensivos.
- Gerenciamento de eventos (teclado e mouse).

Os fractais implementados incluem o clássico **Conjunto de Mandelbrot** e o dinâmico **Conjunto de Julia**.

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

O programa deve ser executado com parâmetros específicos dependendo do fractal desejado:


**1. Conjunto de Mandelbrot**:

Bash
```bash
./fractol Mandelbrot
```

**2. Conjunto de Julia:**

O conjunto de Julia requer coordenadas iniciais para a "semente" complexa ($C$). Valores diferentes geram formas diferentes.

Bash
```bash
./fractol julia -0.8 0.156
```
```bash
./fractol julia 0.285 0.01
```

### Controles

Uma vez que a janela esteja aberta:
- **Roda do Mouse:** Zoom in / Zoom out (seguindo a posição do cursor).
- **Setas:** Mover a câmera (Pan).
- **ESC** ou **X da janela:** Fechar o programa limpo (sem leaks).

## 📚 Recursos e Referências
Este projeto foi construído com base em conceitos matemáticos sólidos e otimizações de computação. Abaixo estão os tópicos e referências utilizados:

### Teoria Matemática
- **Números Complexos:** Compreensão do plano complexo onde $Z = x + iy$.
- **A Fórmula Fractal:** Implementação da iteração $Z_{n+1} = Z_{n}^2 + C$.
- **Algoritmo de Tempo de Fuga:** O uso do módulo quadrado ($x^2 + y^2 > 4$) para evitar o uso custoso da função sqrt (raiz quadrada).
- **Conjuntos Racionais vs. Irracionais:** Entendimento de por que a precisão de double é necessária para representar as bordas infinitas do fractal.

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

**Fract-ol** is a graphical exploration project focused on the mathematical rendering of fractal sets. The main objective is to create software capable of generating infinite, psychedelic images based on complex number mathematical formulas, allowing the user to interact with and explore these mathematical worlds.

This project serves as an introduction to graphical programming using the **MiniLibX** library, covering fundamental concepts such as:

- Pixel and image buffer manipulation.
- Complex number arithmetic.
- Otimization of intensive mathematical calculations.
- Event management (keyboard and mouse).

The implemented fractals include the classic **Mandelbrot Set** and the dynamic **Julia Set**.

---

## 🛠️ Instructions

Prerequisites
The project was developed in C and uses the MiniLibX graphics library. Ensure you have gcc, make, and X11 dependencies installed (if on Linux).

### Compilation
To compile the project, you must run make inside the MiniLibx folder. After that, run the following command at the root of the repository:

Bash
```bash
make
```

### Execution
The program must be executed with specific parameters depending on the desired fractal:

**1. Mandelbrot Set:**

Bash
```bash
./fractol Mandelbrot
```

**2. Julia Set:**
The Julia set requires initial coordinates for the complex "seed" ($C$). Different values generate different shapes.

Bash
```bash
./fractol julia -0.8 0.156
```

Bash
```bash
./fractol julia 0.285 0.01
```

### Controls
Once the window is open:

- **Mouse Wheel:** Zoom in / Zoom out (following the cursor position).
- **Arrows:** Move the camera (Pan).
- **ESC** or **Window X**: Close the program cleanly (no leaks).

## 📚 Resources and References

This project was built upon solid mathematical concepts and computational optimizations. Below are the topics and references used:

### Mathematical Theory
- **Complex Numbers:** Understanding the complex plane where $Z = x + iy$.
- **The Fractal Formula:** Implementation of the iteration $Z_{n+1} = Z_{n}^2 + C$.
- **Escape Time Algorithm:** Using the squared modulus ($x^2 + y^2 > 4$) to avoid the costly usage of the sqrt (square root) function.
- **Rational vs. Irrational Sets:** Understanding why double precision is necessary to represent the infinite edges of the fractal.

### Programming Logic
- **Mapping:** Converting screen coordinates (pixels 0-800) to the complex plane (e.g., -2.0 to +2.0) using linear interpolation.
- **Algebraic Form:** Chose to use $(x^2 - y^2) + 2xyi$ instead of the trigonometric/polar form to maximize CPU performance.
- **Pass by Reference:** Correction of critical bugs in handling complex structs by passing pointers to render functions.

### External References

- **Equaciona Com Paulo Pereira:** Set of classes on complex numbers taught via the YouTube channel.
- **Beginner's Guide To The Desktop:** Series of videos explaining basic desktop components:
	- Class 1: Introduction
	- Class 2: X11, Display Manager and Windows Manager
- **fractol: render beautiful fractals in C (mandelbrot and julia set):** Video from the YouTube channel "Oceano" where he demystifies the project structure and offers a step-by-step guide on how to realize it.
- **Introduction to the minilibX: a simple X-Window programming API in C:** Video from the YouTube channel "Oceano" explaining how the minilibX library works.
- **Draw a Pollock painting with the minilibX:** Video from the YouTube channel "Oceano" explaining how the minilibX library works.
- **The Mandelbrot Fractal Explained!:** Video from the YouTube channel "The Art of Code" explaining the Mandelbrot set.
- **How Imaginary Numbers Were Invented:** Video from the YouTube channel "Veritasium em Português" explaining the origin of imaginary numbers.
- **MinilibX Documentation:** Official documentation.
- **harm-smits.github.io/42docs:** Complementary documentation for the minilibx library.
- **Wikipedia:** Articles on "Mandelbrot set" and "Julia set" for interesting constants.

## 🤖 AI Usage
### 0. AIs
For this project, AI tools from Alphabet were primarily used, specifically NotebookLM and the Gemini chatbot, due to their "guided study" feature. In this regard, the AIs were utilized as pedagogical tutoring tools and, in rare instances, for debugging, assisting with the following specific tasks:

### 1. Explanation of Mathematical Concepts:
- Helped visualize the difference between the Mandelbrot iteration ($Z$ starts at 0, $C$ changes) and Julia ($Z$ is the pixel, $C$ is constant).
- Clarified why sqrt should be avoided in render loops (comparison with squared radius).

### 2. Code Debugging:
- Correction of pointer logic in the handle_pixel function (pass by value vs. reference error).
- Diagnosis of Valgrind errors related to uninitialized memory in mlx_new_window.

### 3. Optimization and Best Practices:
- Suggestion to replace string comparisons (ft_strcmp) inside the render loop with integer flags (int type).
- Explanation on the correct usage of the static keyword in .c versus .h files.