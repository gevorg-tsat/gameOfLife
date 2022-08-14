// Copyright [2022] <Cool_team>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// настройка поля
#define HEIGHT 20
#define WIDTH 80
#define SYMBOL_BORT '='
#define SYMBOL_BORT_SIDE 'N'
#define SYMBOL_DEATH ' '
#define SYMBOL_LIVE '#'
#define SYMBOL_K_LIVE 'L'
#define SYMBOL_K_DEATH 'D'

// настройки управления игрой
#define PAUSE ' '
#define CURSOR_UP 'w'
#define CURSOR_DOWN 's'
#define CURSOR_LEFT 'a'
#define CURSOR_RIGHT 'd'
#define CURSOR_SWITCH 'e'
#define ENTER_LIFE '\n'
#define EXIT '.'

// настройки ручного ввода
#define BETWEEN_XY ' '
#define END_OF_COORDINATES '\n'

int enterHumanCoordinates(int **a, int n, int m);
int enterFileCoordinates(int **a, int n, int m);
int changeSpeedGameScanf();
int check_life(int **matrix, int n, int m, int row, int column);
int allocate(int ***matrix, int n, int m);
int countAlive(int **matrix, int n, int m);
int update(int ***matrix, int n, int m);

void changeSpeedGame(char speedMode, int *speedGame);
void printwField(int **A, int N, int M, int k_x, int k_y);
void printfField(int **A, int N, int M);
void cursorControl(char command, int *isCursor, int *k_x, int *k_y,
                   int **field);
void zeroMatrix(int **a, int n, int m);
void drawGreeting();
void drawLiveDeath();
void drawGameOver();
void drawInfinity();
void drawManual();
void gameControl(char command, int *isPause, int *isExit, int *isCursor,
                 int *speedGame, int *k_x, int *k_y, int **field);

int main(int argc, char **argv) {
  int SPEED_GAME = 100000;
  // координаты курсора
  int k_x = -1;
  int k_y = -1;
  // для прохождения проверки на codestyle
  FILE *file = NULL;
  // создание матрицы поля
  int **field = NULL;
  allocate(&field, HEIGHT, WIDTH);
  drawGreeting();
  // выбор чтения файла или ввода с клавиатуры
  if (*(argv[1]) == '1' || *(argv[1]) == '0') {
    if (*(argv[1]) == '1') {
      // чтение матрицы из файла
      enterFileCoordinates(field, HEIGHT, WIDTH);
      // перенаправление потока ввода на клавиатуру
      file = freopen("/dev/tty", "r", stdin);
    } else {
      // чтение координат с клавиатуры
      enterHumanCoordinates(field, HEIGHT, WIDTH);
      // чтение скорости
      SPEED_GAME = changeSpeedGameScanf();
    }
  }
  // для прохождения проверки на codestyle
  if (argc) {
  } else {
  }
  // переводит терминал в curses-режим
  // (выделяет память под необходимые данные
  // для работы библиотеки, и инициализирует окно stdscr)
  initscr();
  // не выводить вводимые символы
  noecho();
  // не ожидать enter
  nodelay(stdscr, true);
  printwField(field, HEIGHT, WIDTH, k_x, k_y);
  // флаги управления игрой
  int isExit = 0;
  int isAllDeath = 0;
  int isPause = 0;
  int isMovement = 1;
  int isCursor = 0;
  while (!isExit && !isAllDeath && isMovement) {
    // задержка в микросекундах
    usleep(SPEED_GAME);
    // проверяет буфер и, если он не пуст, отправляет информацию на экран
    refresh();
    // очистка экрана
    clear();
    // обновление матрицы поля в соответствии с правилами игры
    if (!isPause) {
      isMovement = update(&field, HEIGHT, WIDTH);
    }
    printwField(field, HEIGHT, WIDTH, k_x, k_y);
    drawManual();
    // обработка управляющих команд
    gameControl(getch(), &isPause, &isExit, &isCursor, &SPEED_GAME, &k_x, &k_y,
                field);
    // нет живых клеток
    if (!countAlive(field, HEIGHT, WIDTH))
      isAllDeath = 1;
  }
  // выход из curses-режима
  endwin();
  // вывод последнего кадра игры
  printfField(field, HEIGHT, WIDTH);
  if (isExit) {
    drawGameOver();
  } else if (isAllDeath) {
    drawLiveDeath();
  } else if (!isMovement) {
    drawInfinity();
  }
  free(field);
  // для прохождения проверки на codestyle
  fclose(file);
  return 0;
}

// логика управления игрой
void gameControl(char command, int *isPause, int *isExit, int *isCursor,
                 int *SPEED_GAME, int *k_x, int *k_y, int **field) {
  switch (command) {
  case PAUSE: {
    if (*isPause == 0) {
      *isPause = 1;
    } else if (*isPause == 1) {
      *isPause = 0;
    }
    break;
  }
  case EXIT:
    *isExit = 1;
    break;
  }
  if (command >= '0' && command <= '9') {
    changeSpeedGame(command, SPEED_GAME);
  }
  cursorControl(command, isCursor, k_x, k_y, field);
}

// логика управления курсором
void cursorControl(char command, int *isCursor, int *k_x, int *k_y,
                   int **field) {
  switch (command) {
  case CURSOR_SWITCH: {
    if (*isCursor == 0) {
      *isCursor = 1;
      *k_x = WIDTH / 2;
      *k_y = HEIGHT / 2;
    } else if (*isCursor == 1) {
      *isCursor = 0;
      *k_x = -1;
      *k_y = -1;
    }
    break;
  }
  case CURSOR_UP: {
    if (*isCursor)
      if (*k_y > 0)
        (*k_y)--;
    break;
  }
  case CURSOR_DOWN: {
    if (*isCursor)
      if (*k_y < HEIGHT - 1)
        (*k_y)++;
    break;
  }
  case CURSOR_LEFT: {
    if (*isCursor)
      if (*k_x > 0)
        (*k_x)--;
    break;
  }
  case CURSOR_RIGHT: {
    if (*isCursor)
      if (*k_x < WIDTH - 1)
        (*k_x)++;
    break;
  }
  case ENTER_LIFE: {
    if (*isCursor) {
      field[*k_y][*k_x] = 1;
    }
    break;
  }
  }
}

// изменение скорости игры
void changeSpeedGame(char speedMode, int *speedGame) {
  switch (speedMode) {
  case '1':
    *speedGame = 1600000;
    break;
  case '2':
    *speedGame = 800000;
    break;
  case '3':
    *speedGame = 400000;
    break;
  case '4':
    *speedGame = 200000;
    break;
  case '5':
    *speedGame = 100000;
    break;
  case '6':
    *speedGame = 50000;
    break;
  case '7':
    *speedGame = 25000;
    break;
  case '8':
    *speedGame = 6250;
    break;
  case '9':
    *speedGame = 3125;
    break;
  case '0':
    *speedGame = 1561;
    break;
  default:
    *speedGame = 100000;
  }
}

// изменение скорости игры с помощью ввода
int changeSpeedGameScanf() {
  int speedMode;
  int speedGame;
  scanf("%d", &speedMode);
  changeSpeedGame(speedMode + '0', &speedGame);
  return speedGame;
}

// ввод координат с клавиатуры
int enterHumanCoordinates(int **a, int n, int m) {
  int isSuccess = 1;
  int x = 0;
  int y = 0;
  int endOfEnter = 0;
  char endOfCoordinates;
  zeroMatrix(a, n, m);
  while (endOfEnter != 1 && isSuccess != 0 &&
         scanf("%d%d%c", &x, &y, &endOfCoordinates) == 3) {
    isSuccess = 1;
    // выравнивание координат под индексы матрицы
    x--;
    y--;
    // если координаты вне диапазоны матрицы
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
      printw("out of frame\n");
      isSuccess = 0;
    }
    // если конец координат не оканчивается на символ theEndOfCoordinates или
    // символ exit
    if (endOfCoordinates != END_OF_COORDINATES && endOfCoordinates != '.') {
      printw("bad end of coordinates: '%c'\n", endOfCoordinates);
      isSuccess = 0;
    }
    // если ввод удачный, заполняем матрицу
    if (isSuccess)
      a[x][y] = 1;
    if (endOfCoordinates == '.') {
      endOfEnter = 1;
    }
  }
  return isSuccess;
}

// ввод матрицы из файла
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

// заполнение матрицы нулями
void zeroMatrix(int **a, int n, int m) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      a[i][j] = 0;
    }
  }
}

// выделение памяти для матрицы
int allocate(int ***matrix, int n, int m) {
  int isSuccess = 0;
  *matrix = (int **)malloc(n * m * sizeof(int) + n * sizeof(int *));
  int *ptr = (int *)(*matrix + n);
  for (int i = 0; i < n; i++)
    (*matrix)[i] = ptr + m * i;
  return isSuccess;
}

// возвращает матрицу 'нового' поколения клеток
int update(int ***matrix, int n, int m) {
  int **matrix_new;
  allocate(&matrix_new, n, m);
  for (int i = 0; i < n; i++)
    for (int j = 0; j < m; j++)
      matrix_new[i][j] = check_life(*matrix, n, m, i, j);
  int changes_check = 1;
  for (int i = 0; i < n; i++)
    for (int j = 0; j < m; j++)
      if (matrix_new[i][j] != (*matrix)[i][j])
        changes_check = 0;
  free(*matrix);
  *matrix = matrix_new;
  return !changes_check;
}

// проверяет, будет ли жить клетка в следующий ход
int check_life(int **matrix, int n, int m, int row, int column) {
  int **neighbors;
  allocate(&neighbors, 8, 2);
  int counter = -1;
  int isSuccess = 0;
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
    isSuccess = 1;
  }
  if (!(matrix[row][column]) && counter == 3) {
    isSuccess = 1;
  }
  free(neighbors);
  return isSuccess;
}

// подсчёт живых клеток
int countAlive(int **matrix, int n, int m) {
  int result = 0;
  for (int i = 0; i < n; i++)
    for (int j = 0; j < m; j++)
      result += matrix[i][j];
  return result;
}

// вывод поля в режиме ncurses
void printwField(int **A, int N, int M, int k_x, int k_y) {
  for (int x = 0; x <= WIDTH + 1; x++) {
    printw("%c", SYMBOL_BORT);
    if (x == WIDTH + 1)
      printw("\n");
  }
  for (int y = 0; y < N; y++) {
    printw("%c", SYMBOL_BORT_SIDE);
    for (int x = 0; x < M; x++) {
      if (A[y][x] != 1 && x == k_x && y == k_y)
        printw("%c", SYMBOL_K_DEATH);
      else if (A[y][x] == 1 && x == k_x && y == k_y)
        printw("%c", SYMBOL_K_LIVE);
      else if (A[y][x] == 1)
        printw("%c", SYMBOL_LIVE);
      else if (A[y][x] == 0)
        printw("%c", SYMBOL_DEATH);
    }
    printw("%c", SYMBOL_BORT_SIDE);
    if (y < M - 1)
      printw("\n");
  }
  for (int x = 0; x <= WIDTH + 1; x++) {
    printw("%c", SYMBOL_BORT);
    if (x == WIDTH + 1)
      printw("\n");
  }
}

// вывод поля
void printfField(int **A, int N, int M) {
  for (int x = 0; x <= WIDTH + 1; x++) {
    printf("%c", SYMBOL_BORT);
    if (x == WIDTH + 1)
      printf("\n");
  }
  for (int y = 0; y < N; y++) {
    printf("%c", SYMBOL_BORT_SIDE);
    for (int x = 0; x < M; x++) {
      if (A[y][x] != 1) {
        printf("%c", SYMBOL_DEATH);
      } else {
        printf("%c", SYMBOL_LIVE);
      }
    }
    printf("%c", SYMBOL_BORT_SIDE);
    if (y < M - 1)
      printf("\n");
  }
  for (int x = 0; x <= WIDTH + 1; x++) {
    printf("%c", SYMBOL_BORT);
    if (x == WIDTH + 1)
      printf("\n");
  }
}

void drawGreeting() {
  printf("====================================================================="
         "=============\n");
  printf("                                                                     "
         "             \n");
  printf("                                                     Welcome to the "
         "game          \n");
  printf("                         x  x                           <Game of "
         "life>!           \n");
  printf("                             x      xx         Enter the coordinates "
         "end speed    \n");
  printf("                         x   x     xxxx                to start the "
         "game.         \n");
  printf("           x              xxxx     xx xx                 For "
         "example:             \n");
  printf("          xxx          x             xx                              "
         "             \n");
  printf("         x x x      x  x      x x                           13 18    "
         "             \n");
  printf("        xxx xxx     x        x  xx                          11 18    "
         "             \n");
  printf("         x x x      x  x      x x                           11 19    "
         "             \n");
  printf("          xxx          x                                    12 17    "
         "             \n");
  printf("           x              xxxx       xx                     12 15    "
         "             \n");
  printf("                         x   x     xx xx                    13 11.   "
         "             \n");
  printf("                             x     xxxx                     5        "
         "             \n");
  printf("                         x  x       xx                               "
         "             \n");
  printf("                                                   To end the game, "
         "click '.'     \n");
  printf("                                                                     "
         "             \n");
  printf("====================================================================="
         "=============\n");
}

void drawGameOver() {
  printf("====================================================================="
         "=============\n");
  printf("                                                                     "
         "             \n");
  printf("     X X          X X                                                "
         "             \n");
  printf("      XX           XX                                                "
         "             \n");
  printf("      X            X                      You finished the game!     "
         "             \n");
  printf("           X             X                                           "
         "             \n");
  printf("            XX            XX                                         "
         "             \n");
  printf("           XX            XX                                          "
         "             \n");
  printf("                                                                     "
         "             \n");
  printf("                                                                     "
         "             \n");
  printf("====================================================================="
         "=============\n");
}

void drawLiveDeath() {
  printf("====================================================================="
         "=============\n");
  printf("                                                                     "
         "             \n");
  printf("             XXXXXXXXXX                                              "
         "             \n");
  printf("          XXX          XXX                                           "
         "             \n");
  printf("        XX                XX                                         "
         "             \n");
  printf("       X    xxxx     xxxx   X                  Game over!            "
         "             \n");
  printf("      X    x  X x   x  X x   X             Your Live is death :(     "
         "             \n");
  printf("      X     xxxx     xxxx    X                                       "
         "             \n");
  printf("       X         x  x       X                                        "
         "             \n");
  printf("        XXX              XXX                                         "
         "             \n");
  printf("           XX   X  X  X  XX                                          "
         "             \n");
  printf("                X  X  X                                              "
         "             \n");
  printf("                X  X  X                                              "
         "             \n");
  printf("                                                                     "
         "             \n");
  printf("====================================================================="
         "=============\n");
}

void drawInfinity() {
  printf("====================================================================="
         "=============\n");
  printf("                                                                     "
         "             \n");
  printf("       XXXXXXXX           XXXXXXXX                                   "
         "             \n");
  printf("     X         XX       XX         X                                 "
         "             \n");
  printf("    X             XX  XX            X              Infinity no "
         "limit!             \n");
  printf("   X                XX               X                               "
         "             \n");
  printf("    X             XX  XX            X                                "
         "             \n");
  printf("     X          XX      XX         X                                 "
         "             \n");
  printf("      XXXXXXXXXX          XXXXXXXXX                                  "
         "             \n");
  printf("                                                                     "
         "             \n");
  printf("====================================================================="
         "=============\n");
}

void drawManual() {
  printw("====================================================================="
         "=============\n");
  printw("                                                                     "
         "             \n");
  printw("                            cursor on/off - e                        "
         "             \n");
  printw("                            to move the cursor:                      "
         "             \n");
  printw("                     up - w; down - s; left - a; right - d           "
         "             \n");
  printw("                                for exit - .                         "
         "             \n");
  printw("                                                                     "
         "             \n");
  printw("====================================================================="
         "=============\n");
}
