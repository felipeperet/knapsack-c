#include <stdlib.h>

#include "../include/branch_and_bound.h"

// Estrutura para armazenar informações do estado atual.
typedef struct Node {
    int level, profit, weight;
    float bound;
} Node;

// Função para calcular o limite superior (bound) no nó.
float bound(Node u, int n, int W, Item arr[]) {
    if (u.weight >= W)
        return 0;

    float result = (float)u.profit;

    int totweight = u.weight;
    int j;
    for (j = u.level + 1; j < n; j++) {
        if (totweight + arr[j].weight <= W) {
            totweight += arr[j].weight;
            result += arr[j].value;
        } else {
            result += (W - totweight) * (float)arr[j].value / arr[j].weight;
            break;
        }
    }

    return result;
}

// Função de comparação para a ordenação dos itens pela razão valor/peso
int cmp(const void *a, const void *b) {
    Item *itemA = (Item *)a;
    Item *itemB = (Item *)b;
    float r1 = (float)itemA->value / itemA->weight;
    float r2 = (float)itemB->value / itemB->weight;
    return (r1 > r2) ? -1 : (r1 < r2) ? 1 : 0;
}

// Implementação do problema da mochila 0-1 utilizando branch-and-bound
int knapsack_branch_and_bound(int W, Item arr[], int n) {
    qsort(arr, n, sizeof(Item), cmp);

    Node *Q = (Node *)malloc(sizeof(Node) * (2 << n));
    int qsize = 0;

    Node u, v;
    u.level = -1;
    u.profit = u.weight = 0;
    u.bound = bound(u, n, W, arr);

    Q[qsize++] = u;
    int maxProfit = 0;

    while (qsize) {
        u = Q[--qsize];

        if (u.level == n - 1 || u.bound <= maxProfit)
            continue;

        v.level = u.level + 1;
        v.weight = u.weight + arr[v.level].weight;
        v.profit = u.profit + arr[v.level].value;

        if (v.weight <= W && v.profit > maxProfit)
            maxProfit = v.profit;

        v.bound = bound(v, n, W, arr);

        if (v.bound > maxProfit)
            Q[qsize++] = v;

        v.weight = u.weight;
        v.profit = u.profit;
        v.bound = bound(v, n, W, arr);

        if (v.bound > maxProfit)
            Q[qsize++] = v;
    }

    free(Q);
    return maxProfit;
}
