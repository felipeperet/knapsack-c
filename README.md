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
Para compilar e executar o programa que gera os arquivos de entrada, use:
```sh
make generate-inputs
```

Para compilar o projeto, use:
```sh
make
```

Para executar o programa, use:
```sh
make run
```

Para limpar os arquivos compilados, use:
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
