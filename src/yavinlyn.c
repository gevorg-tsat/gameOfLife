#define END_GAME '.' //для выхода из игры
char key;
key = getch() != END_GAME; //если введенный символ не точка, игра начинается




void drawGreeting(); //функция для вывода приветственной консоли
void drawGreeting() {
    printf("================================================================================\n");
    printf("                                                                                \n");
    printf("                                                                                \n");
    printf("                         x  x                         Welcome to the game       \n");
    printf("                             x      xx                       <Live>             \n");
    printf("                         x   x     xxxx         To start the game, click Enter  \n");
    printf("           x              xxxx     xx xx           To end the game, click '.'   \n");
    printf("          xxx          x             xx                                         \n");
    printf("         x x x      x  x      x x                                               \n");
    printf("        xxx xxx     x        x  xx                                              \n");
    printf("         x x x      x  x      x x                                               \n");
    printf("          xxx          x                                                        \n");
    printf("           x              xxxx       xx                                         \n");
    printf("                         x   x     xx xx                                        \n");
    printf("                             x     xxxx                                         \n");
    printf("                         x  x       xx                                          \n");
    printf("                                                                                \n");
    printf("                                                                                \n");
    printf("================================================================================\n");
    getchar();
}

void drawLiveDeath(); //функция для вывода в случае, если жизнь погибла
void drawLiveDeath() {
      printf("================================================================================\n");
      printf("                                                                                \n");
      printf("             XXXXXXXXXX                                                         \n");
      printf("          XXX          XXX                                                      \n");
      printf("        XX                XX                                                    \n");
      printf("       X    xxxx     xxxx   X                  Game over!                       \n");
      printf("      X    x  X x   x  X x   X             Your Live is death :(                \n");
      printf("      X     xxxx     xxxx    X                                                  \n");
      printf("       X         x  x       X                                                   \n");
      printf("        XXX              XXX                                                    \n");
      printf("           XX   X  X  X  XX                                                     \n");
      printf("                X  X  X                                                         \n");
      printf("                X  X  X                                                         \n");
      printf("                                                                                \n");
      printf("================================================================================\n");
}

void drawGameOver(); //функция для вывода в случае, если пользователь завершил игру
void drawGameOver() {
    printf("================================================================================\n");
    printf("                                                                                \n");
    printf("     X X          X X                                                           \n");
    printf("      XX           XX                                                           \n");
    printf("      X            X                      You finished the game!                \n");
    printf("           X             X                                                      \n");
    printf("            XX            XX                                                    \n");
    printf("           XX            XX                                                     \n");
    printf("                                                                                \n");
    printf("                                                                                \n");
    printf("================================================================================\n");
}

void drawInfinity(); //функция, если жизнь замирает, входя в бесконечный цикл
void drawInfinity() {
    printf("================================================================================\n");
    printf("                                                                                \n");
    printf("       XXXXXXXX           XXXXXXXX                                              \n");
    printf("     X         XX       XX         X                                            \n");
    printf("    X             XX  XX            X              Infinity no limit!           \n");
    printf("   X                XX               X                                          \n");
    printf("    X             XX  XX            X                                           \n");
    printf("     X          XX      XX         X                                            \n");
    printf("      XXXXXXXXXX          XXXXXXXXX                                             \n");
    printf("                                                                                \n");
    printf("================================================================================\n");
    
}


#define SYMVOL_BORT '='
#define SYMVOL_BORT_SIDE 'N'
#define SYMVOL_DEATH ' '
#define SYMVOL_LIVE '#'
#define SYMVOL_K_LIVE 'L'
#define SYMVOL_K_DEATH 'D'
#define N 80
#define M 25

int drawField(int **A, int N, int M, int K_x, int K_y); //функция отрисовки поля
int drawField(int **A, int N, int M, int K_x, int K_y) {
    for(int a=0; a<=81; a++) {
        printw("%c", SYMVOL_BORT);
        if(a == 81) {
            printw("\n");
        }
    }
    for(int i = 0; i<N; i++) {
        printw("%c", SYMVOL_BORT_SIDE);
        for(int j=0; j<M; j++) {
            if(A[i][j] != 1 && i == K_x && j == K_y) {
                printw("%c", SYMVOL_K_DEATH);
            } if(A[i][j] == 1 && i == K_x && j == K_y) {
                    printw("%c", SYMVOL_K_LIVE);
                } else {
                    printw("%c", SYMVOL_DEATH);
            } else {
                printw("%c", SYMVOL_LIVE); 
            } 
            }
        }      
}
        printw("%c", SYMVOL_BORT_SIDE);
        if(i<M-1) {
            printw("\n");
        }  
    for(int k = 0; ) {
        if(A != SYMVOL_DEATH) {
            printw("%c", SYMVOL_K_LIVE);
        } else {
            printw("%c", SYMVOL_K_DEATH);
        } 
        }
    }    
    }
    for(int a=0; a<=81; a++) {
        printw("%c", SYMVOL_BORT); {
        if(a == 81) {
            printw("\n");
        }
    }
}
