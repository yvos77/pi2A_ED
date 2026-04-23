#include <stdio.h>
#include <stdlib.h>
#include "csv_reader.h"
#include "busca.h"
#include "tempo.h"

/*
 * Protocolo experimental:
 *   - 1000 IDs no total, distribuídos em 4 categorias:
 *       200 do início      (índices [0,          total/3)      )
 *       300 do meio        (índices [total/3,     2*total/3)    )
 *       400 do final       (índices [2*total/3,   total)        )
 *       100 inexistentes   (ids -1 a -100, garantidamente ausentes)
 *   - IDs de cada região selecionados de forma uniformemente espaçada.
 *   - O mesmo conjunto de 1000 IDs é buscado 3 vezes (repetições).
 *   - O clock() é medido individualmente por busca e acumulado.
 *   - Tempo médio = tempo total acumulado / (1000 * 3).
 */

#define N_INICIO       200
#define N_MEIO         300
#define N_FINAL        400
#define N_INEXISTENTE  100
#define NUM_BUSCAS     (N_INICIO + N_MEIO + N_FINAL + N_INEXISTENTE) /* 1000 */
#define NUM_REPETICOES 3

static void selecionar_ids(int *ids, int n, Produto *produtos, int lo, int hi) {
    for (int i = 0; i < n; i++) {
        int idx = lo + (int)((long long)i * (hi - lo) / n);
        ids[i] = produtos[idx].id;
    }
}

static void montar_ids(int ids[NUM_BUSCAS], Produto *produtos, int total) {
    int r1 = total / 3;
    int r2 = (2 * total) / 3;

    selecionar_ids(ids,                            N_INICIO,      produtos, 0,  r1);
    selecionar_ids(ids + N_INICIO,                 N_MEIO,        produtos, r1, r2);
    selecionar_ids(ids + N_INICIO + N_MEIO,        N_FINAL,       produtos, r2, total);

    for (int i = 0; i < N_INEXISTENTE; i++)
        ids[N_INICIO + N_MEIO + N_FINAL + i] = -(i + 1);
}

/*
 * Executa as 3 repetições sobre o mesmo conjunto de 1000 IDs.
 * O clock() é iniciado e finalizado para cada busca individualmente,
 * e os tempos são somados — conforme orientação do professor.
 * Armazena o tempo de cada repetição em tempos_rep[].
 * Grava o log detalhado de cada busca em 'log'.
 * Retorna o tempo total acumulado nas 3 repetições.
 */
static double executar_buscas(Produto *produtos, int total, int ids[NUM_BUSCAS],
                               double tempos_rep[NUM_REPETICOES], FILE *log) {
    double tempo_total = 0.0;

    for (int rep = 0; rep < NUM_REPETICOES; rep++) {
        double tempo_rep = 0.0;

        fprintf(log, "========================================\n");
        fprintf(log, "REPETICAO %d\n", rep + 1);
        fprintf(log, "========================================\n");
        fprintf(log, "%-6s  %-12s  %s\n", "Busca", "ID", "Tempo (s)");
        fprintf(log, "------  ------------  ----------------\n");

        for (int i = 0; i < NUM_BUSCAS; i++) {
            clock_t inicio = tempo_iniciar();
            busca_sequencial(produtos, total, ids[i]);
            double t = tempo_finalizar(inicio);
            tempo_rep += t;
            fprintf(log, "%-6d  %-12d  %.9f\n", i + 1, ids[i], t);
        }

        fprintf(log, "\nTotal da repeticao %d: %.6f s  |  Medio: %.9f s\n\n",
                rep + 1, tempo_rep, tempo_rep / NUM_BUSCAS);

        tempos_rep[rep] = tempo_rep;
        tempo_total += tempo_rep;
    }

    return tempo_total;
}

void executar_experimento(Produto *produtos, int total) {
    printf("\n==============================================\n");
    printf("Experimento com %d registros\n", total);
    printf("==============================================\n");
    printf("IDs buscados: %d inicio | %d meio | %d final | %d inexistentes\n",
           N_INICIO, N_MEIO, N_FINAL, N_INEXISTENTE);

    int ids[NUM_BUSCAS];
    montar_ids(ids, produtos, total);

    /* Abre o arquivo de log */
    FILE *log = fopen("resultados.txt", "w");
    if (!log) {
        fprintf(stderr, "Erro: nao foi possivel criar resultados.txt\n");
        return;
    }

    fprintf(log, "LOG DE BUSCAS — BUSCA SEQUENCIAL\n");
    fprintf(log, "Dataset: %d registros\n", total);
    fprintf(log, "Distribuicao: %d inicio | %d meio | %d final | %d inexistentes\n\n",
            N_INICIO, N_MEIO, N_FINAL, N_INEXISTENTE);

    double tempos_rep[NUM_REPETICOES];
    double tempo_total = executar_buscas(produtos, total, ids, tempos_rep, log);

    /* Resumo no log */
    fprintf(log, "========================================\n");
    fprintf(log, "RESUMO FINAL\n");
    fprintf(log, "========================================\n");
    fprintf(log, "%-15s  %20s  %20s\n", "Repeticao", "Tempo Total (s)", "Tempo Medio (s)");
    for (int rep = 0; rep < NUM_REPETICOES; rep++) {
        fprintf(log, "%-15d  %20.6f  %20.9f\n",
                rep + 1, tempos_rep[rep], tempos_rep[rep] / NUM_BUSCAS);
    }
    double tempo_medio_final = tempo_total / (NUM_BUSCAS * NUM_REPETICOES);
    fprintf(log, "%-15s  %20.6f  %20.9f\n",
            "MEDIA FINAL", tempo_total / NUM_REPETICOES, tempo_medio_final);

    fclose(log);
    printf("\nLog detalhado salvo em: resultados.txt\n");

    /* Resumo no terminal */
    printf("\n%-15s  %20s  %20s\n", "Repeticao", "Tempo Total (s)", "Tempo Medio (s)");
    for (int rep = 0; rep < NUM_REPETICOES; rep++) {
        printf("%-15d  %20.6f  %20.9f\n",
               rep + 1, tempos_rep[rep], tempos_rep[rep] / NUM_BUSCAS);
    }
    printf("\n%-15s  %20.6f  %20.9f\n",
           "MEDIA FINAL", tempo_total / NUM_REPETICOES, tempo_medio_final);
}

int main(void) {
    int total = 0;
    Produto *produtos = ler_csv("data/dataset2.csv", &total);
    if (!produtos) return EXIT_FAILURE;

    printf("Registros carregados: %d\n", total);

    printf("\n--- Validacao do Vetor Dinamico ---\n");
    printf("Capacidade inicial: 100\n");
    printf("Total de registros armazenados: %d\n", total);
    int realocacoes = 0, cap = 100;
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
    printf("Busca inexist - id -1: %s (indice %d)\n",
           idx >= 0 ? "ENCONTRADO" : "NAO ENCONTRADO", idx);

    printf("\n\n========================================\n");
    printf("EXPERIMENTO — DATASET COMPLETO\n");
    printf("Protocolo: %d buscas x %d repeticoes = %d execucoes\n",
           NUM_BUSCAS, NUM_REPETICOES, NUM_BUSCAS * NUM_REPETICOES);
    printf("Distribuicao: %d inicio | %d meio | %d final | %d inexistentes\n",
           N_INICIO, N_MEIO, N_FINAL, N_INEXISTENTE);
    printf("========================================\n");

    executar_experimento(produtos, total);

    free(produtos);
    return EXIT_SUCCESS;
}
