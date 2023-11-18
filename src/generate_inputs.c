#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

void create_directory(const char *path) {
    char tmp[256];
    char *p = NULL;
    size_t len;

    snprintf(tmp, sizeof(tmp), "%s", path);
    len = strlen(tmp);
    if (tmp[len - 1] == '/') {
        tmp[len - 1] = 0;
    }
    for (p = tmp + 1; *p; p++) {
        if (*p == '/') {
            *p = 0;
            mkdir(tmp, S_IRWXU);
            *p = '/';
        }
    }
    mkdir(tmp, S_IRWXU);
}

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
    const char *input_folder = "data/inputs";
    const char *output_folder = "data/outputs";
    int n_fixed = 400;    // Número fixo de itens
    int W_variable = 100; // Capacidade inicial da mochila que irá variar

    // Criar os diretórios para os arquivos de entrada e saída, caso não existam
    create_directory(input_folder);
    create_directory(output_folder);

    // Gerar 20 instâncias com n variando
    for (int n = 100, i = 1; i <= 20; n *= 2, ++i) {
        char filename[50];
        sprintf(filename, "input-%d-n.txt", n);
        generate_instance(input_folder, filename, n, 100);
    }

    // Gerar 20 instâncias com W variando e n fixo em 400
    for (int i = 1; i <= 20; W_variable *= 2, ++i) {
        char filename[50];
        sprintf(filename, "input-%d-W.txt", W_variable);
        generate_instance(input_folder, filename, n_fixed, W_variable);
    }

    return 0;
}
