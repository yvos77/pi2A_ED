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

    free(produtos);
    return EXIT_SUCCESS;
}