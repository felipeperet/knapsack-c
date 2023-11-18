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

void run_algorithm(const char *input_dir, const char *output_dir,
                   const char *algorithm_name) {
    DIR *dir = opendir(input_dir);
    if (dir == NULL) {
        perror("Erro ao abrir o diretório de entrada");
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) { // Verifica se é um arquivo regular
            printf("Processando instância: %s\n", entry->d_name);

            char input_filepath[256], output_filepath[256],
                output_folderpath[256];
            int ret;

            ret = snprintf(input_filepath, sizeof(input_filepath), "%s/%s",
                           input_dir, entry->d_name);
            if (ret >= sizeof(input_filepath) || ret < 0) {
                fprintf(stderr,
                        "Erro ao formatar o caminho do arquivo de entrada.\n");
                continue;
            }

            ret = snprintf(output_folderpath, sizeof(output_folderpath),
                           "%s/%s", output_dir, algorithm_name);
            if (ret >= sizeof(output_folderpath) || ret < 0) {
                fprintf(stderr,
                        "Erro ao formatar o caminho da pasta de saída.\n");
                continue;
            }

            mkdir(output_folderpath,
                  0777); // Cria o diretório de saída se não existir

            ret = snprintf(output_filepath, sizeof(output_filepath), "%s/%s",
                           output_folderpath, entry->d_name);
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
            ret = fscanf(input_file, "%d", &W); // Lê a capacidade da mochila
            if (ret != 1) {
                perror("Erro ao ler a capacidade da mochila do arquivo de "
                       "entrada");
                fclose(input_file);
                continue;
            }

            // Conta o número de itens
            while ((ret = fscanf(input_file, "%d %d", &weight, &value)) == 2) {
                n++;
            }

            // Verifica se o último fscanf falhou sem ser o fim do arquivo
            if (ret != EOF) {
                perror("Erro ao ler os pesos e valores do arquivo de entrada");
                fclose(input_file);
                continue;
            }

            int *weights = malloc(n * sizeof(int));
            int *values = malloc(n * sizeof(int));
            Item *items = malloc(n * sizeof(Item));

            rewind(input_file); // Volta ao início do arquivo
            ret = fscanf(input_file, "%d",
                         &W); // Lê a capacidade da mochila novamente
            if (ret != 1) {
                perror("Erro ao ler a capacidade da mochila do arquivo de "
                       "entrada");
                fclose(input_file);
                free(weights);
                free(values);
                free(items);
                continue;
            }

            for (int i = 0; i < n; i++) {
                ret = fscanf(input_file, "%d %d", &weights[i], &values[i]);
                if (ret != 2) {
                    perror("Erro ao ler pesos e valores dos itens do arquivo "
                           "de entrada");
                    break;
                }
                items[i].weight = weights[i];
                items[i].value = values[i];
            }

            fclose(input_file); // Fecha o arquivo de entrada

            clock_t start = clock();
            int max_profit = 0;
            if (strcmp(algorithm_name, "dynamic") == 0) {
                max_profit = knapsack_dynamic(W, weights, values, n);
            } else if (strcmp(algorithm_name, "backtracking") == 0) {
                max_profit =
                    knapsack_backtracking(W, weights, values, n, 0, 0, 0);
            } else if (strcmp(algorithm_name, "branch_and_bound") == 0) {
                max_profit = knapsack_branch_and_bound(W, items, n);
            }
            clock_t end = clock();
            double cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

            FILE *output_file = fopen(output_filepath, "w");
            if (output_file == NULL) {
                perror("Erro ao abrir o arquivo de saída");
                continue;
            }

            fprintf(output_file,
                    "Lucro máximo: %d\nTempo de execução: %.2f segundos\n",
                    max_profit, cpu_time_used);
            fclose(output_file); // Fecha o arquivo de saída

            free(weights);
            free(values);
            free(items);
        }
    }

    closedir(dir); // Fecha o diretório de entrada
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

    run_algorithm(input_dir, output_dir, algorithm_name);

    return 0;
}
