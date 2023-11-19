# Análise de Complexidade do Problema da Mochila

## Descrição
Este projeto é uma análise das complexidades de tempo de execução de três 
abordagens diferentes para resolver o Problema da Mochila 0-1: programação 
dinâmica, backtracking e branch-and-bound.

## Estrutura do Projeto
``` bash
knapsack/
├── bin/       - Executáveis compilados
├── src/       - Códigos fonte dos algoritmos
├── include/   - Arquivos cabeçalho
├── obj/       - Arquivos objeto gerados na compilação
├── data/      - Entradas e saídas dos testes
├── Makefile   - Script para automatizar a compilação
├── LICENSE.md - Script para automatizar a compilação
└── README.md  - Informações e instruções sobre o projeto
```

## Compilação e Execução
Para realizar a compilação e execução do projeto, siga os passos abaixo:

1. **Compilar o Projeto**
Utilize o comando abaixo para compilar o projeto:
```sh
make
```

2. **Gerar Arquivos de Entrada**
Para executar o programa que gera os arquivos de entrada, utilize:
```sh
make generate-inputs
```

3. **Executar o Programa Principal**
Para executar o programa principal, use o comando:
```sh
make run
```
Após a execução, um prompt no terminal solicitará a escolha do algoritmo 
(programação dinâmica, branch and bound ou backtracking) para processar as 
instâncias que foram geradas anteriormente

4. **Executar Análise dos Resultados**
Para realizar a análise dos resultados, execute:
```sh
make analysis
```
Este comando processará os arquivos de saída localizados em /data/outputs e 
gerará uma análise gráfica comparando a complexidade de tempo de execução de 
cada algoritmo. Os gráficos serão salvos no diretório /data/plots.

5. **Limpar Arquivos Compilados**
Para remover os arquivos compilados e limpar o diretório, use:
```sh
make clean
```

## Uso
O programa pode ser utilizado para comparar a eficiência dos algoritmos de 
resolução do Problema da Mochila, com base no tempo de execução para diversas 
instâncias de entrada.

## Licença
Este projeto é disponibilizado com a Licença MIT, a licença mais permissiva 
disponível, permitindo reutilização ampla e modificação para qualquer propósito.
