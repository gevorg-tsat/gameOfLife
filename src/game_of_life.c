#include <stdio.h>
#include <stdlib.h>
#define HEIGHT 20
#define WIDTH 80

void outputMatrix(int **a, int n, int m);
int enterCoordinates(int **a, int n, int m);
int max(int a, int b);
void zeroMatrix(int **a, int n, int m);


int main() {
    // настройки ввода
    char betweenXY = ' ';
    char theEndOfCoordinates = '\n';
    char exit = '.';
    
    // костыль для решения проблемы с выделением памяти под матрицу поля
    int size = max(HEIGHT, WIDTH);
    
    int **field = malloc(HEIGHT * WIDTH * sizeof(int) + size * sizeof(int *));
    int *ptr = (int *)(field + size);
    for (int i = 0; i < size; i++) {
        field[i] = ptr + size * i;
    }
    enterCoordinates(field, HEIGHT, WIDTH);
    outputMatrix(field, HEIGHT, WIDTH);
    
    free(field);
    
    
    return 0;
}


int enterCoordinates(int **a, int n, int m) {
    int isSuccess = 1;
    int x = 0;
    int y = 0;
    int endOfEnter = 0;
    char spaceBetweenXY;
    char endOfCoordinates;
    zeroMatrix(a, n, m);
    while (endOfEnter != 1 && isSuccess != 0) {
        if (scanf("%d%c%d%c", &x, &spaceBetweenXY, &y, &endOfCoordinates) == 4) {
            fflush(stdin);
            isSuccess = 1;
            // выравнивание координат под индексы матрицы
            x--;
            y--;
            // если координаты вне диапазоны матрицы
            if (x < 0 || x > WIDTH || y < 0 || y > HEIGHT) {
                printf("out of frame\n");
                isSuccess = 0;
            }
            // если координаты не разделены символом betweenXY
            if (spaceBetweenXY != ' ') {
                printf("not space\n");
                isSuccess = 0;
            }
            // если конец координат не оканчивается на символ theEndOfCoordinates или символ exit
            if (endOfCoordinates != '\n' && endOfCoordinates != '.') {
                printf("bad end of coordinates: '%c'\n", endOfCoordinates);
                isSuccess = 0;
            }
            // если ввод удачный, заполняем матрицу
            if (isSuccess) {
                for (int i = 0; i < n; i++) {
                    for (int j = 0; j < m; j++) {
                        if (x == j && y == i) {
                            a[i][j] = 1;
                        }
                    }
                }
            }
            if (endOfCoordinates == '.') {
                endOfEnter = 1;
            }
        }
    }
    return isSuccess;
}


int max(int a, int b) {
    int max = a;
    if (b > a) {
        max = b;
    }
    return max;
}

void outputMatrix(int **a, int n, int m) {
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
