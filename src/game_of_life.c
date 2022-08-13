#include <stdio.h>
#include <stdlib.h>

int allocate(int ***matrix, int n, int m);
void update(int ***matrix, int n, int m);
int check_life(int **matrix, int n, int m, int row, int column);
void output(int **matr, int n, int m);


int main() {
    int **matrix = NULL;
    allocate(&matrix, 20, 20);
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++)
            matrix[i][j] = 0;
    }
    matrix[5][6] = 1;
    matrix[5][8] = 1;
    matrix[6][7] = 1;
    matrix[6][8] = 1;
    matrix[7][6] = 1;
    output(matrix, 20, 20);
    while(getchar() == '\n') {
        printf("\x1B[2J\x1B[H");
        update(&matrix, 20, 20);
        output(matrix, 20, 20);
    }
    return 0;
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
    int counter = 0;
    for (int i = row - 1; i <= row + 1; i++) {
        for (int j = column - 1; j <= column + 1; j++) {
            if (j != row && i != column) {
                neighbors[counter][0] = i;
                neighbors[counter][1] = j;
                counter++;
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

int allocate(int ***matrix, int n, int m) {
    *matrix = malloc(n * m * sizeof(int) + n * sizeof(int*));
    if (*matrix == NULL)
        return 1;
    int *ptr = (int*) (*matrix + n);
    for (int i = 0; i < n; i++)
        (*matrix)[i] = ptr + m * i;
    return 0;
}

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

