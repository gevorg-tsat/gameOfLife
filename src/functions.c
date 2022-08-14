int update(int ***matrix, int n, int m) {
    int **matrix_new;
    allocate(&matrix_new, n, m);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            matrix_new[i][j] = check_life(*matrix, n, m, i, j);
    int changes_check = 1;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            if (matrix_new[i][j] != matrix[i][j])
                changes_check = 0;
    free(*matrix);
    *matrix = matrix_new;
    return changes_check;
}
