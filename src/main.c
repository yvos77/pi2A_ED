#include <stdio.h>
#include <stdlib.h>
#include "csv_reader.h"
#include "busca.h"

int main(void) {
    int total = 0;
    Produto *produtos = ler_csv("data/dataset2.csv", &total);

    if (!produtos) {
        return EXIT_FAILURE;
    }

    printf("Registros carregados: %d\n", total);

    printf("\n--- Validacao do Vetor Dinamico ---\n");
    printf("Capacidade inicial: 100\n");
    printf("Total de registros armazenados: %d\n", total);
    int realocacoes = 0;
    int cap = 100;
    while (cap < total) { cap *= 2; realocacoes++; }
    printf("Numero aproximado de realocacoes: %d\n", realocacoes);
    printf("Primeiro registro: [%d] %s\n", produtos[0].id, produtos[0].nome);
    printf("Ultimo registro:   [%d] %s\n", produtos[total-1].id, produtos[total-1].nome);

    printf("\n--- Teste da Busca Sequencial ---\n");

    // Busca no início
    int idx = busca_sequencial(produtos, total, produtos[0].id);
    printf("Busca inicio  - id %d: %s (indice %d)\n",
           produtos[0].id, idx >= 0 ? "ENCONTRADO" : "NAO ENCONTRADO", idx);

    // Busca no meio
    int id_meio = produtos[total/2].id;
    idx = busca_sequencial(produtos, total, id_meio);
    printf("Busca meio    - id %d: %s (indice %d)\n",
           id_meio, idx >= 0 ? "ENCONTRADO" : "NAO ENCONTRADO", idx);

    // Busca no final
    int id_fim = produtos[total-1].id;
    idx = busca_sequencial(produtos, total, id_fim);
    printf("Busca final   - id %d: %s (indice %d)\n",
           id_fim, idx >= 0 ? "ENCONTRADO" : "NAO ENCONTRADO", idx);

    // Busca inexistente
    idx = busca_sequencial(produtos, total, -1);
    printf("Busca inexist - id %d: %s (indice %d)\n",
           -2, idx >= 0 ? "ENCONTRADO" : "NAO ENCONTRADO", idx);

    free(produtos);
    return EXIT_SUCCESS;
}