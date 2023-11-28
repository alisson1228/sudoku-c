#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <locale.h>

#define SIZE 9
#define INITG 30

typedef struct{
    int sudoku[SIZE][SIZE];
    int jogo[SIZE][SIZE];
    int erros;
} savejogo;

void printHome(int rows, int cols, int option, int continuar);

void printSudoku(int sudoku[SIZE][SIZE], float space, float ups, int ponteiro[2], int jogo[SIZE][SIZE], int erros);

void printInfo(float cols, float ups);

void printSave(int rows, int cols, int option);

void printVitoria(int rows, int cols, int option);

void printDerrota(int rows, int cols, int option);

int testnum(int sudoku[SIZE][SIZE], int row, int col, int num);

int testvit(int initp[INITG][2], int num);

int solveSudoku(int sudoku[SIZE][SIZE]);

void fillSudoku(int sudoku[SIZE][SIZE]);

void initposition(int initp[INITG][2]);

void initjogo(int sudoku[SIZE][SIZE], int jogo[SIZE][SIZE], int initp[INITG][2]);

void initponteiro(int ponteiro[2], int jogo[SIZE][SIZE]);

void controlgame(int jogo[SIZE][SIZE], int sudoku[SIZE][SIZE], int ponteiro[2], int *ch, int *start, int initp[INITG][2], int *erros, int *continuar, FILE *arquivo);

void controlhome(int jogo[SIZE][SIZE], int sudoku[SIZE][SIZE], int ponteiro[2], int ch, int *option, int *start, int *continuar, int initp[INITG][2]);

void controlsave(int jogo[SIZE][SIZE], int sudoku[SIZE][SIZE], int ponteiro[2], int ch, int *option, int *start, int initp[INITG][2], int *erros, int *continuar, FILE *arquivo);

void controlvitder(int jogo[SIZE][SIZE], int sudoku[SIZE][SIZE], int ponteiro[2], int ch, int *option, int *start, int initp[INITG][2], int *erros, int *continuar, FILE *arquivo);

void opensave(int jogo[SIZE][SIZE], int sudoku[SIZE][SIZE], int ponteiro[2], int initp[INITG][2], int *erros, int *continuar, FILE *arquivo);

int testevit(int sudoku[SIZE][SIZE], int jogo[SIZE][SIZE]);

int main() {
    setlocale(LC_ALL,"");

    FILE *arquivo;

    int sudoku[SIZE][SIZE] = {0};
    int jogo[SIZE][SIZE] = {0};
    int initp[INITG][2];
    int ponteiro[2];
    int ch;
    int rows, cols;
    int option = 0;
    int start = 0;
    int erros = 0;
    int continuar = 0;

    opensave(jogo, sudoku, ponteiro, initp, &erros, &continuar, arquivo);

    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();

    if (has_colors()) {
        start_color();

        init_color(13, 1000, 500, 10);
        init_color(12, 1000, 796, 858);

        init_pair(8, 12, COLOR_BLACK);
        init_pair(1, COLOR_RED, COLOR_BLACK);
        init_pair(2, COLOR_YELLOW, COLOR_BLACK);
        init_pair(3, COLOR_GREEN, COLOR_BLACK);
        init_pair(4, COLOR_MAGENTA, COLOR_BLACK);
        init_pair(5, COLOR_BLUE, COLOR_BLACK);
        init_pair(6, COLOR_CYAN, COLOR_BLACK);
        init_pair(7, 13, COLOR_BLACK);
    }

    do {
        getmaxyx(stdscr, rows, cols);

        if(cols > 85) {
            float space = (cols/2.0) - 19.0;
            float ups = (rows/2.0) - 10.0;

            if(start == 1) {
                printSudoku(jogo, space, ups, ponteiro, sudoku, erros);
            } else if(start == 0) {
                printHome(rows, cols, option, continuar);
            } else if(start == 2) {
                printInfo(cols, ups);
            } else if(start == 3) {
                printSave(rows, cols, option);
            } else if(start == 4) {
                printVitoria(rows, cols, option);
            } else {
                printDerrota(rows, cols, option);
            }

            ch = getch();

            if(start == 1) {
                controlgame(jogo, sudoku, ponteiro, &ch, &start, initp, &erros, &continuar, arquivo);
            } else if(start == 0) {
                controlhome(jogo, sudoku, ponteiro, ch, &option, &start, &continuar, initp);   
            } else if(start == 2) {
                if(ch == 27) {
                    start = 0;
                }
            } else if(start == 3) {
                controlsave(jogo, sudoku, ponteiro, ch, &option, &start, initp, &erros, &continuar, arquivo); 
            } else {
                controlvitder(jogo, sudoku, ponteiro, ch, &option, &start, initp, &erros, &continuar, arquivo);
            }

        } else {
            printw("TAMANHO INCOMPATIVEL DE TERMINAL AUMENTE!!\n\nMIN: 86x20 atual(%dx%d)", cols, rows);
            getch();
            ch = 'x';
        }

        clear();

    } while (ch != 'x');

    endwin();

    printf("O tamanho do terminal é %dx%d  [%d,%d]\n", cols, rows, ponteiro[0], ponteiro[1]);


    return 0;
}

void printHome(int rows, int cols, int option, int continuar) {
    for (int i = 0; i < (rows/2) - 10; i++) {
        printw("\n");
    }
    
    for (int i = 0; i < (cols/2) - 42; i++) {
        printw(" ");
    }

    printw(" ___________    ___     ___     _______       _________     ___   ____   ___     ___ \n");

    for (int i = 0; i < (cols/2) - 42; i++) {
        printw(" ");
    }

    printw("|           |  |   |   |   |  |   ___   \\    /   ___   \\   |   | /   /  |   |   |   |\n");
    
    for (int i = 0; i < (cols/2) - 42; i++) {
        printw(" ");
    }

    printw("|   |_______   |   |   |   |  |  |   |   \\  |   |   |   |  |   |/   /   |   |   |   |\n");
    
    for (int i = 0; i < (cols/2) - 42; i++) {
        printw(" ");
    }

    printw("|           |  |   |   |   |  |  |   |   |  |   |   |   |  |       <    |   |   |   |\n");
    
    for (int i = 0; i < (cols/2) - 42; i++) {
        printw(" ");
    }

    printw(" ________|  |  |   |___|   |  |  |___|   |  |   |___|   |  |   |\\   \\   |   |___|   |\n");
    
    for (int i = 0; i < (cols/2) - 42; i++) {
        printw(" ");
    }

    printw("|___________|  |___________|  |_________/    \\_________/   |___| \\___\\  |___________|\n");
    
    for (int i = 0; i < 5; i++) {
        printw("\n");
    }

    if(continuar == 0) {
        if(option == 0) {
            for (int i = 0; i < (cols/2) - 17; i++) {
                printw(" ");
            }

            printw("┌─────────────┐                   \n");

            for (int i = 0; i < (cols/2) - 17; i++) {
                printw(" ");
            }

            printw("│  J O G A R  │         I N F O   \n");
            
            for (int i = 0; i < (cols/2) - 17; i++) {
                printw(" ");
            }

            printw("└─────────────┘                   \n");
        } else {
            for (int i = 0; i < (cols/2) - 17; i++) {
                printw(" ");
            }

            printw("                     ┌───────────┐\n");
            
            for (int i = 0; i < (cols/2) - 17; i++) {
                printw(" ");
            }

            printw("   J O G A R         │  I N F O  │\n");
            
            for (int i = 0; i < (cols/2) - 17; i++) {
                printw(" ");
            }
            
            printw("                     └───────────┘\n");
        }
    } else {
        if(option == 0) {
            for (int i = 0; i < (cols/2) - 33; i++) {
                printw(" ");
            }

            printw("┌─────────────────────┐                                           \n");

            for (int i = 0; i < (cols/2) - 33; i++) {
                printw(" ");
            }

            printw("│  C O N T I N U A R  │       N O V O  J O G O          I N F O   \n");
            
            for (int i = 0; i < (cols/2) - 33; i++) {
                printw(" ");
            }

            printw("└─────────────────────┘                                           \n");
        } else if(option == 1) {
            for (int i = 0; i < (cols/2) - 33; i++) {
                printw(" ");
            }

            printw("                           ┌────────────────────┐                 \n");

            for (int i = 0; i < (cols/2) - 33; i++) {
                printw(" ");
            }

            printw("   C O N T I N U A R       │  N O V O  J O G O  │       I N F O   \n");
            
            for (int i = 0; i < (cols/2) - 33; i++) {
                printw(" ");
            }

            printw("                           └────────────────────┘                 \n");
        } else {
            for (int i = 0; i < (cols/2) - 33; i++) {
                printw(" ");
            }

            printw("                                                     ┌───────────┐\n");

            for (int i = 0; i < (cols/2) - 33; i++) {
                printw(" ");
            }

            printw("   C O N T I N U A R          N O V O  J O G O       │  I N F O  │\n");
            
            for (int i = 0; i < (cols/2) - 33; i++) {
                printw(" ");
            }

            printw("                                                     └───────────┘\n");
        }

    }

    printw("\n\n");

    for (int k = 0; k < (cols/2) - 42; k++) {
        printw(" ");
    }

    printw("Sair = X");

    refresh();
}

void printSudoku(int sudoku[SIZE][SIZE], float space, float ups, int ponteiro[2], int jogo[SIZE][SIZE], int erros) {
    for (int i = 0; i < ups; i++) {
        printw("\n");
    }
    
    
    for (int i = 0; i <SIZE; i++) {
        if (i == 3 || i == 6) {
            for (int k = 0; k < space; k++) {
                printw(" ");
            }
            

        //  printw("|===========|===========|===========|\n");
            printw("╠═══╪═══╪═══╬═══╪═══╪═══╬═══╪═══╪═══╣\n");
        }

        if (i == 0) {
            for (int k = 0; k < space; k++) {
                printw(" ");
            }

        //  printw("|===========|===========|===========|\n");
            printw("╔═══╤═══╤═══╦═══╤═══╤═══╦═══╤═══╤═══╗\n");
        }

        for (int k = 0; k < space; k++) {
            printw(" ");
        }
        
        printw("║");

        for (int j = 0; j <SIZE; j++) {

            if(j == 2 || j == 5 || j == 8) {
                if(sudoku[i][j] == 0) {
                    if(i == ponteiro[0] && j == ponteiro[1]) {
                        printw(" @ ║"); 
                    } else {
                        printw("   ║");
                    }
                } else {
                    if (sudoku[i][j] != jogo[i][j]) {
                        if(i == ponteiro[0] && j == ponteiro[1]) {
                            attron(COLOR_PAIR(2));
                        } else {
                            attron(COLOR_PAIR(1));
                        }
                    }

                    printw(" %d ", sudoku[i][j]); 

                    if (sudoku[i][j] != jogo[i][j]) {
                        if(i == ponteiro[0] && j == ponteiro[1]) {
                            attroff(COLOR_PAIR(2));
                        } else {
                            attroff(COLOR_PAIR(1));
                        }
                    }

                    printw("║");
                }
            } else {
                if(sudoku[i][j] == 0) {
                    if(i == ponteiro[0] && j == ponteiro[1]) {
                        printw(" @ │"); 
                    } else {
                        printw("   │"); 
                    }
                } else {
                    if (sudoku[i][j] != jogo[i][j]) {
                        if(i == ponteiro[0] && j == ponteiro[1]) {
                            attron(COLOR_PAIR(2));
                        } else {
                            attron(COLOR_PAIR(1));
                        }
                    }

                    printw(" %d ", sudoku[i][j]); 

                    if (sudoku[i][j] != jogo[i][j]) {
                        if(i == ponteiro[0] && j == ponteiro[1]) {
                            attroff(COLOR_PAIR(2));
                        } else {
                            attroff(COLOR_PAIR(1));
                        }
                    }

                    printw("│");
                }
            }
        }    
        printw("\n");

        if (i != 2 && i != 5 && i != 8) {
            for (int k = 0; k < space; k++) {
                printw(" ");
            }

        //  printw("|---+---+---|---+---+---|---+---+---|\n");
            printw("╟───┼───┼───╫───┼───┼───╫───┼───┼───╢\n");
        }

        if (i == 8) {
            for (int k = 0; k < space; k++) {
                printw(" ");
            }

        //  printw("|===========|===========|===========|\n");
            printw("╚═══╧═══╧═══╩═══╧═══╧═══╩═══╧═══╧═══╝\n");
        }

    }

    printw("\n\n");

    for (int k = 0; k < space; k++) {
        printw(" ");
    }

    printw("Voltar = ESC / Sair = X    ERROS: %d/3", erros);

    refresh();
}

void printInfo(float cols, float ups) {
    for (int i = 0; i < ups; i++) {
        printw("\n");
    }

    int b = 0;
    int spc;

    spc = (cols/2) - 28;

    
    for (int i = 0; i <SIZE; i++) {
        if (i == 3 || i == 6) {

            for (int k = 0; k < spc + 20; k++) {
                printw(" ");
            }
            

            printw("╠═══╪═══╪═══╬═══╪═══╪═══╬═══╪═══╪═══╣\n");
        }

        if (i == 0) {
            for (int k = 0; k < spc + 20; k++) {
                printw(" ");
            }

            printw("╔═══╤═══╤═══╦═══╤═══╤═══╦═══╤═══╤═══╗\n");
        }

        
        

        for( int k = 0; k < spc; k++) {
            printw(" ");
        }

        switch(b) {
            case 0:
                attron(COLOR_PAIR(8));
                printw("D");
                attroff(COLOR_PAIR(8));
                printw(" - Direita         ");
                break;
            case 1:
                attron(COLOR_PAIR(1));
                printw("A");
                attroff(COLOR_PAIR(1));
                printw(" - Esquerda        ");
                break;
            case 2:
                attron(COLOR_PAIR(2));
                printw("W");
                attroff(COLOR_PAIR(2));
                printw(" - Cima            ");
                break;
            case 3:
                attron(COLOR_PAIR(3));
                printw("S");
                attroff(COLOR_PAIR(3));
                printw(" - Baixo           ");
                break;
            case 4:
                attron(COLOR_PAIR(4));
                printw("Q");
                attroff(COLOR_PAIR(4));
                printw(" - Direita/Cima    ");
                break;
            case 5:
                attron(COLOR_PAIR(5));
                printw("E");
                attroff(COLOR_PAIR(5));
                printw(" - Esquerda/Cima   ");
                break;
            case 6:
                attron(COLOR_PAIR(6));
                printw("Z");
                attroff(COLOR_PAIR(6));
                printw(" - Direita/Baixo   ");
                break;
            case 7:
                attron(COLOR_PAIR(7));
                printw("C");
                attroff(COLOR_PAIR(7));
                printw(" - Esquerda/Baixo  ");
                break;
            default:
                printw("                    ");
        }

        b++;
        
        printw("║");

        for (int j = 0; j <SIZE; j++) {

            if(j == 2 || j == 5 || j == 8) {
                
                if((i >= 0 && i < 4) && (j >= 0 && j < 4)) {
                    attron(COLOR_PAIR(4));
                    printw(" q");
                    attroff(COLOR_PAIR(4));
                } else if ((i >= 0 && i < 4) && (j >= 5 && j <SIZE)) {
                    attron(COLOR_PAIR(5));
                    printw(" e");
                    attroff(COLOR_PAIR(5));
                } else if ((i >= 5 && i <SIZE) && (j >= 0 && j < 4)) {
                    attron(COLOR_PAIR(6));
                    printw(" z");
                    attroff(COLOR_PAIR(6));
                } else if ((i >= 5 && i <SIZE) && (j >= 5 && j <SIZE)) {
                    attron(COLOR_PAIR(7));
                    printw(" c");
                    attroff(COLOR_PAIR(7));
                } else if (i == 4 && (j >= 0 && j < 4)) {
                    attron(COLOR_PAIR(1));
                    printw(" a");
                    attroff(COLOR_PAIR(1));
                } else if (i == 4 && (j >= 5 && j <SIZE)) {
                    attron(COLOR_PAIR(8));
                    printw(" d");
                    attroff(COLOR_PAIR(8));
                } else if ((i >= 0 && i < 4) && j == 4) {
                    attron(COLOR_PAIR(2));
                    printw(" w");
                    attroff(COLOR_PAIR(2));
                } else if ((i >= 5 && i <SIZE) && j == 4) {
                    attron(COLOR_PAIR(3));
                    printw(" s");
                    attroff(COLOR_PAIR(3));
                }

                printw(" ║");
                
                
            } else {

                if((i >= 0 && i < 4) && (j >= 0 && j < 4)) {
                    attron(COLOR_PAIR(4));
                    printw(" q");
                    attroff(COLOR_PAIR(4));
                } else if ((i >= 0 && i < 4) && (j >= 5 && j <SIZE)) {
                    attron(COLOR_PAIR(5));
                    printw(" e");
                    attroff(COLOR_PAIR(5));
                } else if ((i >= 5 && i <SIZE) && (j >= 0 && j < 4)) {
                    attron(COLOR_PAIR(6));
                    printw(" z");
                    attroff(COLOR_PAIR(6));
                } else if ((i >= 5 && i <SIZE) && (j >= 5 && j <SIZE)) {
                    attron(COLOR_PAIR(7));
                    printw(" c");
                    attroff(COLOR_PAIR(7));
                } else if (i == 4 && (j >= 0 && j < 4)) {
                    attron(COLOR_PAIR(1));
                    printw(" a");
                    attroff(COLOR_PAIR(1));
                } else if (i == 4 && (j >= 5 && j <SIZE)) {
                    attron(COLOR_PAIR(8));
                    printw(" d");
                    attroff(COLOR_PAIR(8));
                } else if ((i >= 0 && i < 4) && j == 4) {
                    attron(COLOR_PAIR(2));
                    printw(" w");
                    attroff(COLOR_PAIR(2));
                } else if ((i >= 5 && i <SIZE) && j == 4) {
                    attron(COLOR_PAIR(3));
                    printw(" s");
                    attroff(COLOR_PAIR(3));
                } else if(i == 4 && j == 4) {
                    printw(" @");
                }
                
                printw(" │");
                
            }
        }    
        printw("\n");

        if (i != 2 && i != 5 && i != 8) {
            for (int k = 0; k < spc + 20; k++) {
                printw(" ");
            }

            printw("╟───┼───┼───╫───┼───┼───╫───┼───┼───╢\n");
        }

        if (i == 8) {
            for (int k = 0; k < spc + 20; k++) {
                printw(" ");
            }

            printw("╚═══╧═══╧═══╩═══╧═══╧═══╩═══╧═══╧═══╝\n");
        }

    }

    printw("\n\n");

    for (int k = 0; k < spc; k++) {
        printw(" ");
    }

    printw("Voltar = ESC / Sair = X");

    refresh();
}

void printVitoria(int rows, int cols, int option) {
    for (int i = 0; i < (rows/2) - 3; i++) {
        printw("\n");
    }
    
    for (int i = 0; i < (cols/2) - 35; i++) {
        printw(" ");
    }

    printw("___     ___         ________              ______              ___    \n");
    
    for (int i = 0; i < (cols/2) - 35; i++) {
        printw(" ");
    }
    
    printw("\\  \\   /  /   __   |__    __|   _____    |  /\\  \\    __      /   \\   \n");
    
    for (int i = 0; i < (cols/2) - 35; i++) {
        printw(" ");
    }
    
    printw(" \\  \\ /  /   |  |     |  |     /  _  \\   |  \\/  /   |  |    / / \\ \\  \n");
    
    for (int i = 0; i < (cols/2) - 35; i++) {
        printw(" ");
    }
    
    printw("  \\  -  /    |  |     |  |    |  | |  |  |  |\\  \\   |  |   /  ___  \\ \n");
    
    for (int i = 0; i < (cols/2) - 35; i++) {
        printw(" ");
    }

    printw("   \\___/     |__|     |__|     \\_____/   |__| \\__\\  |__|  /__/   \\__\\\n");

    printw("\n\n");

    if(option == 0) {
        for (int i = 0; i < (cols/2) - 20; i++) {
            printw(" ");
        }

        printw("┌──────────────────┐                    \n");

        for (int i = 0; i < (cols/2) - 20; i++) {
            printw(" ");
        }

        printw("│ N O V O  J O G O │      V O L T A R   \n");
        
        for (int i = 0; i < (cols/2) - 20; i++) {
            printw(" ");
        }
        
        printw("└──────────────────┘                    \n");
    } else {
        for (int i = 0; i < (cols/2) - 20; i++) {
            printw(" ");
        }

        printw("                       ┌───────────────┐\n");

        for (int i = 0; i < (cols/2) - 20; i++) {
            printw(" ");
        }

        printw("  N O V O  J O G O     │  V O L T A R  │\n");
        
        for (int i = 0; i < (cols/2) - 20; i++) {
            printw(" ");
        }
        
        printw("                       └───────────────┘\n");
    }          
}

void printDerrota(int rows, int cols, int option) {
    for (int i = 0; i < (rows/2) - 3; i++) {
        printw("\n");
    }
    
    for (int i = 0; i < (cols/2) - 40; i++) {
        printw(" ");
    }

    printw(" _____       ______     ______     ______              ________       ___      \n");
    
    for (int i = 0; i < (cols/2) - 40; i++) {
        printw(" ");
    }
    
    printw("|  __  \\    / /__\\ \\   |  /\\  \\   |  /\\  \\    _____   |__    __|     /   \\    \n");
    
    for (int i = 0; i < (cols/2) - 40; i++) {
        printw(" ");
    }
    
    printw("| |  |  |  |  ______|  |  \\/  /   |  \\/  /   /  _  \\     |  |       / / \\ \\  \n");
    
    for (int i = 0; i < (cols/2) - 40; i++) {
        printw(" ");
    }
    
    printw("| |__|  |  \\  \\_____   |  |\\  \\   |  |\\  \\  |  |_|  |    |  |      /  ___  \\  \n");
    
    for (int i = 0; i < (cols/2) - 40; i++) {
        printw(" ");
    }

    printw("|______/    \\______/   |__| \\__\\  |__| \\__\\  \\_____/     |__|     /__/   \\__\\\n");

    printw("\n\n");

    if(option == 0) {
        for (int i = 0; i < (cols/2) - 20; i++) {
            printw(" ");
        }

        printw("┌──────────────────┐                    \n");

        for (int i = 0; i < (cols/2) - 20; i++) {
            printw(" ");
        }

        printw("│ N O V O  J O G O │      V O L T A R   \n");
        
        for (int i = 0; i < (cols/2) - 20; i++) {
            printw(" ");
        }
        
        printw("└──────────────────┘                    \n");
    } else {
        for (int i = 0; i < (cols/2) - 20; i++) {
            printw(" ");
        }

        printw("                       ┌───────────────┐\n");

        for (int i = 0; i < (cols/2) - 20; i++) {
            printw(" ");
        }

        printw("  N O V O  J O G O     │  V O L T A R  │\n");
        
        for (int i = 0; i < (cols/2) - 20; i++) {
            printw(" ");
        }
        
        printw("                       └───────────────┘\n");
    }          
}

void printSave(int rows, int cols, int option) {
    for (int i = 0; i < (rows/2) - 3; i++) {
        printw("\n");
    }
    
    for (int i = 0; i < (cols/2) - 9; i++) {
        printw(" ");
    }

    printw("QUER SALVAR O JOGO?\n\n");

    if(option == 0) {
        for (int i = 0; i < (cols/2) - 10; i++) {
            printw(" ");
        }

        printw("┌───────┐            \n");

        for (int i = 0; i < (cols/2) - 10; i++) {
            printw(" ");
        }

        printw("│ S I M │     N Ã O  \n");
        
        for (int i = 0; i < (cols/2) - 10; i++) {
            printw(" ");
        }
        
        printw("└───────┘            \n");
    } else {
        for (int i = 0; i < (cols/2) - 10; i++) {
            printw(" ");
        }

        printw("            ┌───────┐\n");

        for (int i = 0; i < (cols/2) - 10; i++) {
            printw(" ");
        }

        printw("  S I M     │ N Ã O │\n");
        
        for (int i = 0; i < (cols/2) - 10; i++) {
            printw(" ");
        }
        
        printw("            └───────┘\n");
    }
}

int testnum(int sudoku[SIZE][SIZE], int row, int col, int num) {
    
    for (int x = 0; x < SIZE; x++) {
        if (sudoku[row][x] == num || sudoku[x][col] == num) {
            return 0;
        }
    }

    int startRow = row - row % 3;
    int startCol = col - col % 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (sudoku[i + startRow][j + startCol] == num) {
                return 0;
            }
        }
    }

    return 1;
}

int testpos(int initp[INITG][2], int num) {
    
    for (int i = 0; i < num ; i++) {
        if (initp[num][0] == initp[i][0] && initp[num][1] == initp[i][1]) {
            return 0;
        }
    }

    return 1;
}

int solveSudoku(int sudoku[SIZE][SIZE]) {
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            if (sudoku[row][col] == 0) {
                for (int num = 1; num <= SIZE; num++) {
                    if (testnum(sudoku, row, col, num)) {
                        sudoku[row][col] = num;
                        if (solveSudoku(sudoku)) {
                            return 1;
                        }
                        sudoku[row][col] = 0;
                    }
                }
                return 0;
            }
        }
    }
    return 1;
}

void fillSudoku(int sudoku[SIZE][SIZE]) {
    srand(time(NULL));

    for (int col = 0; col < SIZE; col++) {
        int num;
        do {
            num = (rand() % SIZE) + 1;
        } while (!testnum(sudoku, 0, col, num));

        sudoku[0][col] = num;
    }

    for (int row = 1; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            sudoku[row][col] = 0;
        }
    }
}

void initposition(int initp[INITG][2]) {
    srand(time(NULL));

    for (int i = 0; i < INITG; i++) {

        do {
            initp[i][0] = rand() % SIZE;
            initp[i][1] = rand() % SIZE;
            
        } while(!testpos(initp, i));
    
    }
}

void initjogo(int sudoku[SIZE][SIZE], int jogo[SIZE][SIZE], int initp[INITG][2]) {
    for (int e = 0; e < INITG; e++) {
        for (int i = 0; i <SIZE; i++) {
            for (int j = 0; j <SIZE; j++) {
                if(i == initp[e][0] && j == initp[e][1]) {
                    jogo[i][j] = sudoku[i][j];
                }
            }
        }
    }
}

void initponteiro(int ponteiro[2], int jogo[SIZE][SIZE]) {
    int w, z;
    for (w = 0; w <SIZE; w++) {
        for (z = 0; z <SIZE; z++) {
            if (jogo[w][z] == 0) {
                ponteiro[0] = w;
                ponteiro[1] = z;
                w =SIZE;  // Para sair do loop externo
                break;
            }
        }
    }
}

void controlgame(int jogo[SIZE][SIZE], int sudoku[SIZE][SIZE], int ponteiro[2], int *ch, int *start, int initp[INITG][2], int *erros, int *continuar, FILE *arquivo) {
    switch (*ch) {
        case KEY_UP:
        case 'w':
            if(ponteiro[0] > 0) {
                for (int i = ponteiro[0] - 1; i >= 0; i--) {
                    if(jogo[i][ponteiro[1]] == 0 || ((jogo[i][ponteiro[1]] != sudoku[i][ponteiro[1]]) && jogo[i][ponteiro[1]] != 0)) {
                        ponteiro[0] = i;
                        break;
                    }
                    
                }
            }
            break;
        case KEY_DOWN:
        case 's':
            if(ponteiro[0] < 8) {
                for (int i = ponteiro[0] + 1; i <= 8; i++) {
                    if(jogo[i][ponteiro[1]] == 0 || ((jogo[i][ponteiro[1]] != sudoku[i][ponteiro[1]]) && jogo[i][ponteiro[1]] != 0)) {
                        ponteiro[0] = i;
                        break;
                    }
                    
                }
            }
            break;
        case KEY_LEFT:
        case 'a':
            if(ponteiro[1] > 0) {
                for (int i = ponteiro[1] - 1; i >= 0; i--) {
                    if(jogo[ponteiro[0]][i] == 0 || ((jogo[ponteiro[0]][i] != sudoku[ponteiro[0]][i]) && jogo[ponteiro[0]][i] != 0)) {
                        ponteiro[1] = i;
                        break;
                    }
                    
                }
            }
            break;
        case KEY_RIGHT:
        case 'd':
            if(ponteiro[1] < 8) {
                for (int i = ponteiro[1] + 1; i <= 8; i++) {
                    if(jogo[ponteiro[0]][i] == 0 || ((jogo[ponteiro[0]][i] != sudoku[ponteiro[0]][i]) && jogo[ponteiro[0]][i] != 0)) {
                        ponteiro[1] = i;
                        break;
                    }
                    
                }
            }
            break;
        case 'q':
            for(int i = ponteiro[0] - 1; i >= 0; i--) {
                for(int j = ponteiro[1] - 1; j >= 0; j--) {
                    if(jogo[i][j] == 0 || ((jogo[i][j] != sudoku[i][j]) && jogo[i][j] != 0)) {
                        ponteiro[0] = i;
                        ponteiro[1] = j;
                        i = -1;
                        break;
                    }
                }
            }
            
            break;
        case 'e':
            for(int i = ponteiro[0] - 1; i >= 0; i--) {
                for(int j = ponteiro[1] + 1; j <SIZE; j++) {
                    if(jogo[i][j] == 0 || ((jogo[i][j] != sudoku[i][j]) && jogo[i][j] != 0)) {
                        ponteiro[0] = i;
                        ponteiro[1] = j;
                        i = -1;
                        break;
                    }
                }
            }
            break;
        case 'z':
            for(int i = ponteiro[0] + 1; i <SIZE; i++) {
                for(int j = ponteiro[1] - 1; j >= 0; j--) {
                    if(jogo[i][j] == 0 || ((jogo[i][j] != sudoku[i][j]) && jogo[i][j] != 0)) {
                        ponteiro[0] = i;
                        ponteiro[1] = j;
                        i =SIZE;
                        break;
                    }
                }
            }
            
            break;
        case 'c':
            for(int i = ponteiro[0] + 1; i <SIZE; i++) {
                for(int j = ponteiro[1] + 1; j <SIZE; j++) {
                    if(jogo[i][j] == 0 || ((jogo[i][j] != sudoku[i][j]) && jogo[i][j] != 0)) {
                        ponteiro[0] = i;
                        ponteiro[1] = j;
                        i =SIZE;
                        break;
                    }
                }
            }
            
            break;
        case 49:
            *ch = 1;
            break;
        case 50:
            *ch = 2;
            break;
        case 51:
            *ch = 3;
            break;
        case 52:
            *ch = 4;
            break;
        case 53:
            *ch = 5;
            break;
        case 54:
            *ch = 6;
            break;
        case 55:
            *ch = 7;
            break;
        case 56:
            *ch = 8;
            break;
        case 57:
            *ch = 9;
            break;
        case 27:
            if(*continuar == 0) {
                *start = 3;
            } else {
                *start = 0;

                savejogo save;

                arquivo = fopen("save.bin", "wb");

                for (int i = 0; i < 9; i++) {
                    for (int j = 0; j < 9; j++) {
                        save.sudoku[i][j] = sudoku[i][j];
                        save.jogo[i][j] = jogo[i][j];
                    }
                }

                save.erros = *erros;

                fwrite(&save, sizeof(savejogo), 1, arquivo);

                fclose(arquivo);
            }

            break;
    }

    if((*ch > 0 && *ch < 10) && (jogo[ponteiro[0]][ponteiro[1]] == 0 || jogo[ponteiro[0]][ponteiro[1]] != sudoku[ponteiro[0]][ponteiro[1]])) {
        jogo[ponteiro[0]][ponteiro[1]] = *ch;

        if(jogo[ponteiro[0]][ponteiro[1]] != sudoku[ponteiro[0]][ponteiro[1]]) {
            ++*erros;
        }

        if(*erros == 3) {
            *start = 5;
            *erros = 0;
            *continuar = 0;
            
            for(int i = 0; i <SIZE; i++) {
                for(int j = 0; j <SIZE; j++) {
                    sudoku[i][j] = 0;
                    jogo[i][j] = 0;
                }
            }

            initposition(initp);
            fillSudoku(sudoku);
            solveSudoku(sudoku);
            initjogo(sudoku, jogo, initp);
            initponteiro(ponteiro, jogo);
        }
        
    }

    if(testevit(sudoku, jogo)) {
        *start = 4;
    }
}

void controlhome(int jogo[SIZE][SIZE], int sudoku[SIZE][SIZE], int ponteiro[2], int ch, int *option, int *start, int *continuar, int initp[INITG][2]) {
    switch (ch) {
        case KEY_LEFT:
        case 'a':
            if(*option > 0) {
                *option = *option - 1;
            }
            break;
        case KEY_RIGHT:
        case 'd':
            if(*continuar == 0) {
                if(*option < 1) {
                    *option = *option + 1;
                }
            } else {
                if(*option < 2) {
                    *option = *option + 1;
                }
            }
            break;
        case 10:
        case 32:
            if(*continuar == 0) {
                if(*option == 0) {
                    *start = 1;
                } else {
                    *start = 2;
                }
            } else {
                if(*option == 0) {
                    *start = 1;
                } else if(*option == 1) {
                    *start = 1;
                    *continuar = 0;

                    remove("save.bin");

                    for(int i = 0; i <SIZE; i++) {
                        for(int j = 0; j <SIZE; j++) {
                            sudoku[i][j] = 0;
                            jogo[i][j] = 0;
                        }
                    }

                    initposition(initp);
                    fillSudoku(sudoku);
                    solveSudoku(sudoku);
                    initjogo(sudoku, jogo, initp);
                    initponteiro(ponteiro, jogo);
                } else {
                    *start = 2;
                }
            }
            
            break;
    }
}

void controlsave(int jogo[SIZE][SIZE], int sudoku[SIZE][SIZE], int ponteiro[2], int ch, int *option, int *start, int initp[INITG][2], int *erros, int *continuar, FILE *arquivo) {

    switch(ch) {
        case KEY_LEFT:
        case 'a':
            *option = 0;
            break;
        case KEY_RIGHT:
        case 'd':
            *option = 1;
            break;
        case 10:
        case 32:
            if(*option == 0) {
                *start = 0;
                *continuar = 1;

                savejogo save;

                arquivo = fopen("save.bin", "wb");

                for (int i = 0; i < 9; i++) {
                    for (int j = 0; j < 9; j++) {
                        save.sudoku[i][j] = sudoku[i][j];
                        save.jogo[i][j] = jogo[i][j];
                    }
                }

                save.erros = *erros;

                fwrite(&save, sizeof(savejogo), 1, arquivo);

                fclose(arquivo);
            } else {
                *start = 0;
                *erros = 0;
                *option = 0;
                
                for(int i = 0; i <SIZE; i++) {
                    for(int j = 0; j <SIZE; j++) {
                        sudoku[i][j] = 0;
                        jogo[i][j] = 0;
                    }
                }

                initposition(initp);
                fillSudoku(sudoku);
                solveSudoku(sudoku);
                initjogo(sudoku, jogo, initp);
                initponteiro(ponteiro, jogo);
            }
            
            break;
    }

    
}

void controlvitder(int jogo[SIZE][SIZE], int sudoku[SIZE][SIZE], int ponteiro[2], int ch, int *option, int *start, int initp[INITG][2], int *erros, int *continuar, FILE *arquivo) {
    switch(ch) {
        case KEY_LEFT:
        case 'a':
            *option = 0;
            break;
        case KEY_RIGHT:
        case 'd':
            *option = 1;
            break;
        case 10:
        case 32:
            *continuar = 0;

            if(*continuar == 1) {
                remove("save.bin");
            }

            if(*option == 0) {
                *start = 1;

            } else {
                *start = 0;
            }

            *erros = 0;
            *option = 0;
            
            for(int i = 0; i <SIZE; i++) {
                for(int j = 0; j <SIZE; j++) {
                    sudoku[i][j] = 0;
                    jogo[i][j] = 0;
                }
            }

            initposition(initp);
            fillSudoku(sudoku);
            solveSudoku(sudoku);
            initjogo(sudoku, jogo, initp);
            initponteiro(ponteiro, jogo);
            
            break;
    }
}

void opensave(int jogo[SIZE][SIZE], int sudoku[SIZE][SIZE], int ponteiro[2], int initp[INITG][2], int *erros, int *continuar, FILE *arquivo) {
    arquivo = fopen("save.bin", "rb");

    if (arquivo != NULL) {
        *continuar = 1;

        savejogo save;

        fread(&save, sizeof(savejogo), 1, arquivo);

        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                sudoku[i][j] = save.sudoku[i][j];
                jogo[i][j] = save.jogo[i][j];
                
            }
        }

        *erros = save.erros;

        initponteiro(ponteiro, jogo);

        fclose(arquivo);

    } else {
        initposition(initp);
        fillSudoku(sudoku);
        solveSudoku(sudoku);
        initjogo(sudoku, jogo, initp);
        initponteiro(ponteiro, jogo);
    }
}

int testevit(int sudoku[SIZE][SIZE], int jogo[SIZE][SIZE]) {

    for(int i = 0; i < SIZE; i++) {
        for(int j = 0; j < SIZE; j++) {
            if(sudoku[i][j] != jogo[i][j]) {
                return 0;
            }
        }
    }

    return 1;
}