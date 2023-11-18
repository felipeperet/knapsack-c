#ifndef BRANCH_AND_BOUND_H
#define BRANCH_AND_BOUND_H

typedef struct {
    int value;
    int weight;
    int index;
} Item;

int knapsack_branch_and_bound(int W, Item arr[], int n);

#endif // BRANCH_AND_BOUND_H
