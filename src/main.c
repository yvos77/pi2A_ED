#include <stdio.h>
#include <stdlib.h>
#include "csv_reader.h"

int main(void) {
    int total = 0;
    Produto *produtos = ler_csv("data/dataset2.csv", &total);

    if (!produtos) {
        return EXIT_FAILURE;
    }

    printf("Registros carregados: %d\n", total);
    printf("Primeiros 3 registros:\n");
    for (int i = 0; i < 3 && i < total; i++) {
        printf("  [%d] %s | %s | R$ %.2f\n",
               produtos[i].id,
               produtos[i].nome,
               produtos[i].categoria,
               produtos[i].valor);
    }

    printf("\n--- Validacao do Vetor Dinamico ---\n");
    printf("Capacidade inicial: 100\n");
    printf("Total de registros armazenados: %d\n", total);
    int realocacoes = 0;
    int cap = 100;
    while (cap < total) { cap *= 2; realocacoes++; }
    printf("Numero aproximado de realocacoes: %d\n", realocacoes);
    printf("Primeiro registro: [%d] %s\n", produtos[0].id, produtos[0].nome);
    printf("Ultimo registro:   [%d] %s\n", produtos[total-1].id, produtos[total-1].nome);

    free(produtos);
    return EXIT_SUCCESS;
}