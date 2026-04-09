#include <stdio.h>
#include <stdlib.h>
#include "csv_reader.h"
#include "busca.h"
#include "tempo.h"

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
    int idx;

    idx = busca_sequencial(produtos, total, produtos[0].id);
    printf("Busca inicio  - id %d: %s (indice %d)\n",
           produtos[0].id, idx >= 0 ? "ENCONTRADO" : "NAO ENCONTRADO", idx);

    idx = busca_sequencial(produtos, total, produtos[total/2].id);
    printf("Busca meio    - id %d: %s (indice %d)\n",
           produtos[total/2].id, idx >= 0 ? "ENCONTRADO" : "NAO ENCONTRADO", idx);

    idx = busca_sequencial(produtos, total, produtos[total-1].id);
    printf("Busca final   - id %d: %s (indice %d)\n",
           produtos[total-1].id, idx >= 0 ? "ENCONTRADO" : "NAO ENCONTRADO", idx);

    idx = busca_sequencial(produtos, total, -1);
    printf("Busca inexist - id %d: %s (indice %d)\n",
           -2, idx >= 0 ? "ENCONTRADO" : "NAO ENCONTRADO", idx);

    printf("\n--- Medicao de Tempo ---\n");

    // Teste início
    clock_t inicio = tempo_iniciar();
    for (int i = 0; i < 1000; i++)
        busca_sequencial(produtos, total, produtos[0].id);
    ResultadoTempo r1 = tempo_calcular_medio(tempo_finalizar(inicio), 1000);
    printf("Busca no inicio:\n");
    tempo_imprimir(r1);

    // Teste meio
    inicio = tempo_iniciar();
    for (int i = 0; i < 1000; i++)
        busca_sequencial(produtos, total, produtos[total/2].id);
    ResultadoTempo r2 = tempo_calcular_medio(tempo_finalizar(inicio), 1000);
    printf("\nBusca no meio:\n");
    tempo_imprimir(r2);

    // Teste final
    inicio = tempo_iniciar();
    for (int i = 0; i < 1000; i++)
        busca_sequencial(produtos, total, produtos[total-1].id);
    ResultadoTempo r3 = tempo_calcular_medio(tempo_finalizar(inicio), 1000);
    printf("\nBusca no final:\n");
    tempo_imprimir(r3);

    // Teste inexistente
    inicio = tempo_iniciar();
    for (int i = 0; i < 1000; i++)
        busca_sequencial(produtos, total, -1);
    ResultadoTempo r4 = tempo_calcular_medio(tempo_finalizar(inicio), 1000);
    printf("\nBusca inexistente:\n");
    tempo_imprimir(r4);

    free(produtos);
    return EXIT_SUCCESS;
}