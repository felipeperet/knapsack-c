#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../include/backtracking.h"
#include "../include/branch_and_bound.h"
#include "../include/dynamic.h"

int main() {
    srand(time(NULL)); // Inicializa a semente do gerador de números aleatórios.
    bool all_equal = true;

    for (int instance = 0; instance < 1000; instance++) {
        int n = rand() % 5 + 1;  // Tamanho da instância entre 1 e 5.
        int W = rand() % 20 + 1; // Capacidade da mochila entre 1 e 20.
        int val[n], wt[n];
        Item arr[n];

        // Gera valores e pesos aleatórios para os itens e preenche o array de
        // itens.
        for (int i = 0; i < n; i++) {
            val[i] = rand() % 100 + 1; // Valor entre 1 e 100.
            wt[i] = rand() % 20 + 1;   // Peso entre 1 e 20.
            arr[i].value = val[i];
            arr[i].weight = wt[i];
        }

        // Compara os resultados dos três algoritmos.
        int result_dynamic = knapsack_dynamic(W, wt, val, n);
        int result_backtracking = knapsack_backtracking(W, wt, val, n, 0, 0, 0);
        int result_branch_and_bound = knapsack_branch_and_bound(W, arr, n);

        // Se algum resultado for diferente, atualiza all_equal para falso.
        if (result_dynamic != result_backtracking ||
            result_dynamic != result_branch_and_bound) {
            all_equal = false;
            break;
        }
    }

    if (all_equal) {
        printf("Todos os algoritmos forneceram os mesmos resultados para 1000 "
               "instâncias aleatórias.\n");
    } else {
        printf("Falso: Houve pelo menos uma instância onde os algoritmos "
               "forneceram resultados diferentes.\n");
    }

    return 0;
}
