# sudoku-c

<p style="display: flex;aling-itens: center">Testado em:   
<img src="https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black" height="20px"></p>

Desenvolvi um jogo de Sudoku em linguagem C que gera partidas de forma aleatória, apresentando telas de menu e incorporando uma funcionalidade de salvamento em arquivo.

> #### Tela Principal
>
> ![Home](imagem.png)

> #### Tela do Jogo
>
> ![Sudoko](imagem2.png)

Esse jogo é dependente da biblioteca ncurses e para fazer a compilação é necessario usar p seguinte no terminal:

`gcc sudoku.c -o sudoku -lncurses`

e para execulta use no terminal:

`./sudoku`

**OBS:** É extremamente importante que o termina usado use Unicode.
