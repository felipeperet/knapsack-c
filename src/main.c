#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#include "../include/backtracking.h"
#include "../include/branch_and_bound.h"
#include "../include/dynamic.h"

typedef struct {
    char name[256];
    int size;
} FileEntry;

int compare_file_entries(const void *a, const void *b) {
    FileEntry *entry_a = (FileEntry *)a;
    FileEntry *entry_b = (FileEntry *)b;
    return entry_a->size - entry_b->size;
}

void extract_size_from_name(const char *filename, int *size) {
    sscanf(filename, "input-%d-", size);
}

void process_files_in_order(FileEntry *entries, int count,
                            const char *input_dir, const char *output_dir,
                            const char *algorithm_name) {
    for (int i = 0; i < count; i++) {
        printf("Processando instância: %s\n", entries[i].name);

        char input_filepath[256], output_filepath[256], output_folderpath[256],
            output_filename[256];
        int ret;

        ret = snprintf(input_filepath, sizeof(input_filepath), "%s/%s",
                       input_dir, entries[i].name);
        if (ret >= sizeof(input_filepath) || ret < 0) {
            fprintf(stderr,
                    "Erro ao formatar o caminho do arquivo de entrada.\n");
            continue;
        }

        ret = snprintf(output_folderpath, sizeof(output_folderpath), "%s/%s",
                       output_dir, algorithm_name);
        if (ret >= sizeof(output_folderpath) || ret < 0) {
            fprintf(stderr, "Erro ao formatar o caminho da pasta de saída.\n");
            continue;
        }

        mkdir(output_folderpath, 0777);

        ret = snprintf(output_filename, sizeof(output_filename), "output%s",
                       strchr(entries[i].name, '-'));
        if (ret >= sizeof(output_filename) || ret < 0) {
            fprintf(stderr, "Erro ao formatar o nome do arquivo de saída.\n");
            continue;
        }

        ret = snprintf(output_filepath, sizeof(output_filepath), "%s/%s",
                       output_folderpath, output_filename);
        if (ret >= sizeof(output_filepath) || ret < 0) {
            fprintf(stderr,
                    "Erro ao formatar o caminho do arquivo de saída.\n");
            continue;
        }

        FILE *input_file = fopen(input_filepath, "r");
        if (input_file == NULL) {
            perror("Erro ao abrir o arquivo de entrada");
            continue;
        }

        int W, n = 0, weight, value;
        ret = fscanf(input_file, "%d", &W);
        if (ret != 1) {
            perror("Erro ao ler a capacidade da mochila do arquivo de entrada");
            fclose(input_file);
            continue;
        }

        while ((ret = fscanf(input_file, "%d %d", &weight, &value)) == 2) {
            n++;
        }

        if (ret != EOF) {
            perror("Erro ao ler os pesos e valores do arquivo de entrada");
            fclose(input_file);
            continue;
        }

        int *weights = malloc(n * sizeof(int));
        int *values = malloc(n * sizeof(int));
        Item *items = malloc(n * sizeof(Item));

        rewind(input_file);
        ret = fscanf(input_file, "%d", &W);
        if (ret != 1) {
            perror("Erro ao ler a capacidade da mochila do arquivo de entrada");
            fclose(input_file);
            free(weights);
            free(values);
            free(items);
            continue;
        }

        for (int i = 0; i < n; i++) {
            ret = fscanf(input_file, "%d %d", &weights[i], &values[i]);
            if (ret != 2) {
                perror("Erro ao ler pesos e valores dos itens do arquivo de "
                       "entrada");
                break;
            }
            items[i].weight = weights[i];
            items[i].value = values[i];
        }

        fclose(input_file);

        clock_t start = clock();
        int max_profit = 0;
        if (strcmp(algorithm_name, "dynamic") == 0) {
            max_profit = knapsack_dynamic(W, weights, values, n);
        } else if (strcmp(algorithm_name, "backtracking") == 0) {
            max_profit = knapsack_backtracking(W, weights, values, n, 0, 0, 0);
        } else if (strcmp(algorithm_name, "branch_and_bound") == 0) {
            max_profit = knapsack_branch_and_bound(W, items, n);
        }
        clock_t end = clock();
        double cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

        FILE *output_file = fopen(output_filepath, "w");
        if (output_file == NULL) {
            perror("Erro ao abrir o arquivo de saída");
            free(weights);
            free(values);
            free(items);
            continue;
        }

        fprintf(output_file,
                "Lucro máximo: %d\nTempo de execução: %.2f segundos\n",
                max_profit, cpu_time_used);
        fclose(output_file);

        free(weights);
        free(values);
        free(items);
    }
}

int main() {
    const char *input_dir = "data/inputs";
    const char *output_dir = "data/outputs";
    char algorithm_name[30];

    printf("Escolha o algoritmo (dynamic, backtracking, branch_and_bound): ");
    if (scanf("%29s", algorithm_name) != 1) {
        fprintf(stderr, "Erro ao ler a escolha do algoritmo.\n");
        return EXIT_FAILURE;
    }

    DIR *dir = opendir(input_dir);
    if (dir == NULL) {
        perror("Erro ao abrir o diretório de entrada");
        return EXIT_FAILURE;
    }

    FileEntry *entries = NULL;
    int count = 0;
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            count++;
            entries = realloc(entries, count * sizeof(FileEntry));
            strcpy(entries[count - 1].name, entry->d_name);
            extract_size_from_name(entry->d_name, &entries[count - 1].size);
        }
    }
    closedir(dir);

    qsort(entries, count, sizeof(FileEntry), compare_file_entries);

    process_files_in_order(entries, count, input_dir, output_dir,
                           algorithm_name);

    free(entries);

    return 0;
}
