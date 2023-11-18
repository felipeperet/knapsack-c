#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

void generate_instance(const char *foldername, const char *filename, int n,
                       int W) {
    char filepath[256];
    sprintf(filepath, "%s/%s", foldername, filename);

    FILE *file = fopen(filepath, "w");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "%d\n", W);
    for (int i = 0; i < n; ++i) {
        int weight = rand() % 30 + 1;
        int value = rand() % 100 + 1;
        fprintf(file, "%d\t%d\n", weight, value);
    }

    fclose(file);
}

int main() {
    srand(time(NULL));
    const char *foldername = "data/inputs";
    int n_fixed = 400;    // Número fixo de itens
    int W_variable = 100; // Capacidade inicial da mochila que irá variar

    // Criar o diretório 'inputs', se não existir
    if (mkdir(foldername, 0777) != 0 && errno != EEXIST) {
        perror("Erro ao criar o diretório 'inputs'");
        exit(EXIT_FAILURE);
    }

    // Gerar 20 instâncias com n variando
    for (int n = 100, i = 1; i <= 20; n *= 2, ++i) {
        char filename[50];
        sprintf(filename, "input-%d-n.txt", n);
        generate_instance(foldername, filename, n, 100);
    }

    // Gerar 20 instâncias com W variando e n fixo em 400
    for (int i = 1; i <= 20; W_variable *= 2, ++i) {
        char filename[50];
        sprintf(filename, "input-%d-W.txt", W_variable);
        generate_instance(foldername, filename, n_fixed, W_variable);
    }

    return 0;
}
