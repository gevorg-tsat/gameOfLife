#include <stdio.h>
#include <stdlib.h>

int allocate(int ***matrix, int n, int m);
void update(int ***matrix, int n, int m);
int check_life(int **matrix, int n, int m, int row, int column);

int main() {
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
    for (int i = row - 1; i <= row + 1; i++)
        for (int j = column - 1; j <= column + 1; j++) 
            if (j != row && i != column) {
                neighbors[counter++][0] = i;
                neighbors[counter++][1] = j;
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
        counter += matrix[neighbors[i][0]][neighbors[i][0]];
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
