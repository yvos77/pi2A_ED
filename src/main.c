#include <stdio.h>
#include <stdlib.h>
#include "csv_reader.h"
#include "busca.h"
#include "tempo.h"

#define NUM_BUSCAS 1000
#define NUM_REPETICOES 3

double executar_teste(Produto *produtos, int total, int id) {
    double soma = 0.0;
    for (int r = 0; r < NUM_REPETICOES; r++) {
        clock_t inicio = tempo_iniciar();
        for (int i = 0; i < NUM_BUSCAS; i++)
            busca_sequencial(produtos, total, id);
        soma += tempo_finalizar(inicio);
    }
    return soma / NUM_REPETICOES;
}

void imprimir_teste(const char *descricao, double tempo_total) {
    ResultadoTempo r = tempo_calcular_medio(tempo_total, NUM_BUSCAS);
    printf("\n%s\n", descricao);
    tempo_imprimir(r);
}

void executar_experimento(Produto *produtos, int total) {
    printf("\n==============================================\n");
    printf("Experimento com %d registros\n", total);
    printf("==============================================\n");

    int id_inicio     = produtos[0].id;
    int id_meio       = produtos[total/2].id;
    int id_final      = produtos[total-1].id;
    int id_inexistente = -1;

    double t_inicio     = executar_teste(produtos, total, id_inicio);
    double t_meio       = executar_teste(produtos, total, id_meio);
    double t_final      = executar_teste(produtos, total, id_final);
    double t_inexistente = executar_teste(produtos, total, id_inexistente);

    printf("\n%-35s %15s %20s\n", "Cenario", "Tempo Total(s)", "Tempo Medio(s)");
    printf("%-35s %15.6f %20.9f\n", "Inicio",      t_inicio,       t_inicio      / NUM_BUSCAS);
    printf("%-35s %15.6f %20.9f\n", "Meio",        t_meio,         t_meio        / NUM_BUSCAS);
    printf("%-35s %15.6f %20.9f\n", "Final",       t_final,        t_final       / NUM_BUSCAS);
    printf("%-35s %15.6f %20.9f\n", "Inexistente", t_inexistente,  t_inexistente / NUM_BUSCAS);
}

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
           -1, idx >= 0 ? "ENCONTRADO" : "NAO ENCONTRADO", idx);

    // Experimentos com diferentes tamanhos
    printf("\n\n========================================\n");
    printf("EXPERIMENTOS COM DIFERENTES TAMANHOS\n");
    printf("Protocolo: %d buscas x %d repeticoes\n", NUM_BUSCAS, NUM_REPETICOES);
    printf("========================================\n");

    int tamanhos[] = {total/4, total/2, (3*total)/4, total};
    int num_tamanhos = 4;

    for (int t = 0; t < num_tamanhos; t++) {
        executar_experimento(produtos, tamanhos[t]);
    }

    free(produtos);
    return EXIT_SUCCESS;
}