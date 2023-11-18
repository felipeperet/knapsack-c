#include "../include/backtracking.h"
#include "../include/utilities.h"

// Implementação do problema da mochila 0-1 utilizando backtracking.
int knapsack_backtracking(int W, int wt[], int val[], int n, int idx,
                          int total_val, int total_wt) {
    if (total_wt > W || idx == n)
        return total_val;

    int value_with = 0;
    if (total_wt + wt[idx] <= W) {
        value_with = knapsack_backtracking(
            W, wt, val, n, idx + 1, total_val + val[idx], total_wt + wt[idx]);
    }

    int value_without =
        knapsack_backtracking(W, wt, val, n, idx + 1, total_val, total_wt);

    return max(value_with, value_without);
}
