#include <stdio.h>
#include <stdlib.h>

#include "../include/dynamic.h"
#include "../include/utilities.h"

// Aloca memória para um array de 2 dimensões.
int **allocate_matrix(int rows, int cols) {
    int **matrix = malloc(rows * sizeof(int *));

    if (matrix == NULL) {
        fprintf(stderr, "Erro de alocação de memória\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < rows; i++) {
        matrix[i] = malloc(cols * sizeof(int));
        if (matrix[i] == NULL) {
            fprintf(stderr, "Erro de alocação de memória\n");
            for (int j = 0; j < i; j++) {
                free(matrix[j]);
            }
            free(matrix);
            exit(EXIT_FAILURE);
        }
    }

    return matrix;
}

// Desaloca memória para um array de 2 dimensões.
void deallocate_matrix(int **matrix, int rows) {
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

// Implementação do problema da mochila 0-1 utilizando programação dinâmica.
int knapsack_dynamic(int W, int wt[], int val[], int n) {
    int i, w;
    int **K = allocate_matrix(n + 1, W + 1);

    for (i = 0; i <= n; i++) {
        for (w = 0; w <= W; w++) {
            if (i == 0 || w == 0)
                K[i][w] = 0;
            else if (wt[i - 1] <= w)
                K[i][w] =
                    max(val[i - 1] + K[i - 1][w - wt[i - 1]], K[i - 1][w]);
            else
                K[i][w] = K[i - 1][w];
        }
    }

    int result = K[n][W];

    deallocate_matrix(K, n + 1);

    return result;
}
