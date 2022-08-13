#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include <unistd.h>

// настройка поля
#define HEIGHT 20
#define WIDTH 80
#define SYMBOL_BORT '='
#define SYMBOL_BORT_SIDE 'N'
#define SYMBOL_DEATH ' '
#define SYMBOL_LIVE '#'
// настройки ручного ввода
#define BETWEEN_XY ' '
#define END_OF_COORDINATES '\n'
#define EXIT '.'

int enterHumanCoordinates(int **a, int n, int m);
int enterFileCoordinates(int **a, int n, int m);
int enterHumanSpeedGame();
int check_life(int **matrix, int n, int m, int row, int column);
int allocate(int ***matrix, int n, int m);
int drawFieldPrintw(int **A, int N, int M);
int drawFieldPrintf(int **A, int N, int M);
int countAlive(int **matrix, int n, int m);

void zeroMatrix(int **a, int n, int m);
void update(int ***matrix, int n, int m);
void drawGreeting();
void output(int **a, int n, int m);
void drawLiveDeath();
void drawGameOver();


int main(int argc, char **argv) {
    
    int SPEED_GAME;
    
    // создание матрицы поля
    int **field = NULL;
    allocate(&field, HEIGHT, WIDTH);
    
    // вывод приветствия
    drawGreeting();
    
    // выбор чтения файла или ввода с клавиатуры
    if (*(argv[1]) == '1' || *(argv[1]) == '0') {
        if (*(argv[1]) == '1') {
            enterFileCoordinates(field, HEIGHT, WIDTH);
            SPEED_GAME = enterHumanSpeedGame();
        } else {
            enterHumanCoordinates(field, HEIGHT, WIDTH);
            SPEED_GAME = enterHumanSpeedGame();
        }
    }
    
    // переводит терминал в curses-режим
    // (выделяет память под необходимые данные
    // для работы библиотеки, и инициализирует окно stdscr)
    initscr();
    // не выводить вводимые символы
    noecho();
    // не ожидать enter
    nodelay(stdscr, true);
    // отрисовка поля
    drawFieldPrintw(field, HEIGHT, WIDTH);
    int exit = 0;
    int death_all = 0;
    while (!exit && !death_all) {
        // задержка в микросекундах
        usleep(SPEED_GAME);
        // проверяет буфер и, если он не пуст, отправляет информацию на экран
        refresh();
        // очистка экрана
        clear();
        // обновление матрицы поля в соответствии с правилами игры
        update(&field, HEIGHT, WIDTH);
        drawFieldPrintw(field, HEIGHT, WIDTH);
        
        switch(getch()) {
            case 'a': {
                if (SPEED_GAME > 6250) SPEED_GAME /= 2;
                break;
            }
            case 'z': {
                if (SPEED_GAME < 100000) SPEED_GAME *= 2;
                break;
            }
            case EXIT: exit = 1;
        }
        
        
        if (!countAlive(field, HEIGHT, WIDTH))
            death_all = 1;
        
    }
    
    // выход из curses-режима
    endwin();
    
    drawFieldPrintf(field, HEIGHT, WIDTH);
    if (exit) {
        printf("\n");
        drawGameOver();
    }
    if (death_all) {
        printf("\n");
        drawLiveDeath();
    }
    
    output(field, HEIGHT, WIDTH);
    free(field);
    return 0;
}

int enterHumanSpeedGame() {
    int speedMode;
    int speed;
    scanf("%d", &speedMode);
    switch(speedMode) {
        case 1: speed = 100000; break;
        case 2: speed = 50000; break;
        case 3: speed = 25000; break;
        case 4: speed = 12500; break;
        case 5: speed = 6250; break;
        default: speed = 100000;
    }
    return speed;
}

int enterHumanCoordinates(int **a, int n, int m) {
    int isSuccess = 1;
    int x = 0;
    int y = 0;
    int endOfEnter = 0;
    char spaceBetweenXY;
    char endOfCoordinates;
    zeroMatrix(a, n, m);
    while (endOfEnter != 1 && isSuccess != 0 && scanf("%d%d%c", &x, &y, &endOfCoordinates) == 3) {
        //if () {
            fflush(stdin);
            isSuccess = 1;
            // выравнивание координат под индексы матрицы
            x--;
            y--;
            
            // если координаты вне диапазоны матрицы
            if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
                printw("out of frame\n");
                isSuccess = 0;
            }
            /*// если координаты не разделены символом betweenXY
            if (spaceBetweenXY != ' ') {
                printw("not space\n");
                isSuccess = 0;
            }*/
            // если конец координат не оканчивается на символ theEndOfCoordinates или символ exit
            if (endOfCoordinates != '\n' && endOfCoordinates != '.') {
                printw("bad end of coordinates: '%c'\n", endOfCoordinates);
                isSuccess = 0;
            }
            // если ввод удачный, заполняем матрицу
            if (isSuccess)
                a[x][y] = 1;
            if (endOfCoordinates == '.') {
                endOfEnter = 1;
            }
       // }
    }
    return isSuccess;
}

int enterFileCoordinates(int **a, int n, int m) {
     char ch;
     int isSuccess = 0;
     int element;
     for (int i = 0; i < n; i++)
       for (int j = 0; j < m; j++) {
         if (scanf("%d%c", &element, &ch) == 2 && (ch == '\n' || ch == ' ')) {
           a[i][j] = element;
           isSuccess = 1;
         } else {
           isSuccess = 0;
         }
       }
     return isSuccess;
}

void output(int **a, int n, int m) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m - 1; j++) {
      printf("%d ", a[i][j]);
    }
    if (i < n - 1) {
      printf("%d\n", a[i][m - 1]);
    } else {
      printf("%d", a[i][m - 1]);
    }
  }
}

void zeroMatrix(int **a, int n, int m) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            a[i][j] = 0;
        }
    }
}

int allocate(int ***matrix, int n, int m) {
    int isSuccess = 0;
    *matrix = (int**) malloc(n * m * sizeof(int) + n * sizeof(int*));
    if (*matrix == NULL)
        isSuccess = 1;
    int *ptr = (int*) (*matrix + n);
    for (int i = 0; i < n; i++)
        (*matrix)[i] = ptr + m * i;
    return isSuccess;
}

void update(int ***matrix, int n, int m) {
    int **matrix_new;
    allocate(&matrix_new, n, m);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            matrix_new[i][j] = check_life(*matrix, n, m, i, j);
    free(*matrix);
    *matrix = matrix_new;
}

int check_life(int **matrix, int n, int m, int row, int column) {
    int **neighbors;
    allocate(&neighbors, 8, 2);
    int counter = -1;
    for (int i = row - 1; i <= row + 1; i++) {
        for (int j = column - 1; j <= column + 1; j++) {
            if (i != row || j != column) {
                counter++;
                neighbors[counter][0] = i;
                neighbors[counter][1] = j;
            }
        }
    }
    for (int i = 0; i < 8; i++) {
        if (neighbors[i][0] < 0)
            neighbors[i][0] += n;
        if (neighbors[i][0] >= n)
            neighbors[i][0] -= n;
        if (neighbors[i][1] >= m)
            neighbors[i][1] -= m;
        if (neighbors[i][1] < 0)
            neighbors[i][1] += m;
    }
    counter = 0;
    for (int i = 0; i < 8; i++)
        counter += matrix[neighbors[i][0]][neighbors[i][1]];
    if (matrix[row][column] && (counter == 2 || counter == 3)) {
        free(neighbors);
        return 1;
    }
    if (!(matrix[row][column]) && counter == 3) {
        free(neighbors);
        return 1;
    }
    free(neighbors);
    return 0;
}

int countAlive(int **matrix, int n, int m) {
    int result = 0;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            result += matrix[i][j];
    return result;
}

int drawFieldPrintw(int **A, int N, int M) {
    for (int x = 0; x <= WIDTH + 1; x++) {
        printw("%c", SYMBOL_BORT);
        if (x == WIDTH + 1) printw("\n");
    }
    for (int y = 0; y < N; y++) {
        printw("%c", SYMBOL_BORT_SIDE);
        for(int x = 0; x < M; x++) {
            if(A[y][x] != 1) {
                printw("%c", SYMBOL_DEATH);
            } else {
                printw("%c", SYMBOL_LIVE);
            }
        }
        printw("%c", SYMBOL_BORT_SIDE);
        if (y < M - 1) printw("\n");
    }
    for (int x = 0; x <= WIDTH + 1; x++) {
        printw("%c", SYMBOL_BORT);
        if (x == WIDTH + 1) printw("\n");
    }
}

int drawFieldPrintf(int **A, int N, int M) {
    for (int x = 0; x <= WIDTH + 1; x++) {
        printf("%c", SYMBOL_BORT);
        if (x == WIDTH + 1) printf("\n");
    }
    for (int y = 0; y < N; y++) {
        printf("%c", SYMBOL_BORT_SIDE);
        for(int x = 0; x < M; x++) {
            if(A[y][x] != 1) {
                printf("%c", SYMBOL_DEATH);
            } else {
                printf("%c", SYMBOL_LIVE);
            }
        }
        printf("%c", SYMBOL_BORT_SIDE);
        if (y < M - 1) printf("\n");
    }
    for (int x = 0; x <= WIDTH + 1; x++) {
        printf("%c", SYMBOL_BORT);
        if (x == WIDTH + 1) printf("\n");
    }
}

void drawGreeting() {
    printf("==================================================================================\n");
    printf("                                                                                  \n");
    printf("                                                                                  \n");
    printf("                         x  x                         Welcome to the game         \n");
    printf("                             x      xx                       <Live>               \n");
    printf("                         x   x     xxxx         To start the game, click Enter    \n");
    printf("           x              xxxx     xx xx           To end the game, click '.'     \n");
    printf("          xxx          x             xx                                           \n");
    printf("         x x x      x  x      x x                                                 \n");
    printf("        xxx xxx     x        x  xx                                                \n");
    printf("         x x x      x  x      x x                                                 \n");
    printf("          xxx          x                                                          \n");
    printf("           x              xxxx       xx                                           \n");
    printf("                         x   x     xx xx                                          \n");
    printf("                             x     xxxx                                           \n");
    printf("                         x  x       xx                                            \n");
    printf("                                                                                  \n");
    printf("                                                                                  \n");
    printf("==================================================================================\n");
}

void drawGameOver() {
    printf("==================================================================================\n");
    printf("                                                                                  \n");
    printf("     X X          X X                                                             \n");
    printf("      XX           XX                                                             \n");
    printf("      X            X                      You finished the game!                  \n");
    printf("           X             X                                                        \n");
    printf("            XX            XX                                                      \n");
    printf("           XX            XX                                                       \n");
    printf("                                                                                  \n");
    printf("                                                                                  \n");
    printf("==================================================================================\n");
}

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
