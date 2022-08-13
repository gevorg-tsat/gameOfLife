#include <stdio.h>
#include <stdlib.h>
#define WIDTH 80
#define HEIGHT 25
​
int allocate(int ***matrix, int n, int m);
void update(int ***matrix, int n, int m);
int check_life(int **matrix, int n, int m, int row, int column);
void output(int **matr, int n, int m);
​
​
int main(int argc, char **argv) {
    if (*(argv[1]) == '1' || *(argv[1]) == '0') {
        if (*(argv[1]) == '1') {
            printf("FILE\n");
        } else {
            printf("MAN\n");
        }
    }
​
    int **matrix = NULL;
    allocate(&matrix, HEIGHT, WIDTH);
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++)
            matrix[i][j] = 0;
    }
    matrix[0][6] = 1;
    matrix[0][8] = 1;
    matrix[1][7] = 1;
    matrix[1][8] = 1;
    matrix[2][7] = 1;
    output(matrix, HEIGHT, WIDTH);
    printf("\x1B[2J\x1B[H");
    printf("\e[0;$%d;$%d;t]", HEIGHT, WIDTH * 2);
    while(getchar() == '\n') {
        printf("\e[0;$%d;$%d;t]", HEIGHT, WIDTH * 2);
        printf("\x1B[2J\x1B[H");
        update(&matrix, HEIGHT, WIDTH);
        output(matrix, HEIGHT, WIDTH);
    }
    return 0;
}
​
void update(int ***matrix, int n, int m) {
    int **matrix_new;
    allocate(&matrix_new, n, m);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            matrix_new[i][j] = check_life(*matrix, n, m, i, j);
    free(*matrix);
    *matrix = matrix_new;
}
​
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
                //printf("%d %d\n", neighbors[counter][0], neighbors[counter][1]);
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
​
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
​
void output(int **matr, int n, int m) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < m; j++)
            printf("%d ", matr[i][j]);
        printf("\n");
    }
    for (int j = 0; j < m - 1; j++)
        printf("%d ", matr[n-1][j]);
    printf("%d", matr[n-1][m-1]);
}