#include <stdio.h>
#include <stdlib.h>

#include "../include/branch_and_bound.h"

typedef struct Node {
    int level, profit, weight;
    float bound;
} Node;

typedef struct {
    Node *nodes;
    int capacity;
    int size;
} DynamicQueue;

void DynamicQueue_init(DynamicQueue *queue, int initialCapacity) {
    queue->nodes = (Node *)malloc(initialCapacity * sizeof(Node));
    if (!queue->nodes) {
        fprintf(stderr,
                "Memory allocation failed during queue initialization.\n");
        exit(EXIT_FAILURE);
    }
    queue->capacity = initialCapacity;
    queue->size = 0;
}

void DynamicQueue_push(DynamicQueue *queue, Node node) {
    if (queue->size >= queue->capacity) {
        queue->capacity *= 2;
        queue->nodes =
            (Node *)realloc(queue->nodes, queue->capacity * sizeof(Node));
        if (!queue->nodes) {
            fprintf(stderr,
                    "Memory allocation failed during queue resizing.\n");
            exit(EXIT_FAILURE);
        }
    }
    queue->nodes[queue->size++] = node;
}

Node DynamicQueue_pop(DynamicQueue *queue) {
    if (queue->size == 0) {
        fprintf(stderr, "Queue pop operation attempted on an empty queue.\n");
        exit(EXIT_FAILURE);
    }
    return queue->nodes[--queue->size];
}

void DynamicQueue_free(DynamicQueue *queue) { free(queue->nodes); }

float bound(Node u, int n, int W, Item arr[]) {
    if (u.weight >= W) {
        return 0;
    }

    float result = (float)u.profit;
    int totweight = u.weight;

    for (int j = u.level + 1; j < n; j++) {
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

int cmp(const void *a, const void *b) {
    Item *itemA = (Item *)a;
    Item *itemB = (Item *)b;
    float r1 = (float)itemA->value / itemA->weight;
    float r2 = (float)itemB->value / itemB->weight;
    if (r1 > r2)
        return -1;
    if (r1 < r2)
        return 1;
    return 0;
}

int knapsack_branch_and_bound(int W, Item arr[], int n) {
    qsort(arr, n, sizeof(Item), cmp);

    DynamicQueue queue;
    DynamicQueue_init(&queue, 1024);

    Node u, v;
    u.level = -1;
    u.profit = u.weight = 0;
    u.bound = bound(u, n, W, arr);
    DynamicQueue_push(&queue, u);

    int maxProfit = 0;

    while (queue.size) {
        u = DynamicQueue_pop(&queue);

        if (u.level == n - 1 || u.bound <= maxProfit) {
            continue;
        }

        v.level = u.level + 1;

        // Include the next item
        v.weight = u.weight + arr[v.level].weight;
        v.profit = u.profit + arr[v.level].value;
        if (v.weight <= W && v.profit > maxProfit) {
            maxProfit = v.profit;
        }
        v.bound = bound(v, n, W, arr);
        if (v.bound > maxProfit) {
            DynamicQueue_push(&queue, v);
        }

        // Exclude the next item
        v.weight = u.weight;
        v.profit = u.profit;
        v.bound = bound(v, n, W, arr);
        if (v.bound > maxProfit) {
            DynamicQueue_push(&queue, v);
        }
    }

    DynamicQueue_free(&queue);
    return maxProfit;
}
