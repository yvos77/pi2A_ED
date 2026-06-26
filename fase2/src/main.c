#include <stdio.h>
#include <stdlib.h>
#include "csv_reader.h"
#include "vetor.h"
#include "busca.h"
#include "hash.h"
#include "tempo.h"

#define N_INICIO       200
#define N_MEIO         300
#define N_FINAL        400
#define N_INEXISTENTE  100
#define NUM_BUSCAS     (N_INICIO + N_MEIO + N_FINAL + N_INEXISTENTE)
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

    selecionar_ids(ids,                     N_INICIO, produtos, 0,  r1);
    selecionar_ids(ids + N_INICIO,          N_MEIO,   produtos, r1, r2);
    selecionar_ids(ids + N_INICIO + N_MEIO, N_FINAL,  produtos, r2, total);

    for (int i = 0; i < N_INEXISTENTE; i++)
        ids[N_INICIO + N_MEIO + N_FINAL + i] = -(i + 1);
}

static double bloco_sequencial(Produto *produtos, int total, int ids[NUM_BUSCAS]) {
    clock_t inicio = tempo_iniciar();
    for (int i = 0; i < NUM_BUSCAS; i++) {
        volatile int r = busca_sequencial(produtos, total, ids[i]);
        (void)r;
    }
    return tempo_finalizar(inicio);
}

static double bloco_hash(TabelaHash *t, int ids[NUM_BUSCAS]) {
    clock_t inicio = tempo_iniciar();
    for (int i = 0; i < NUM_BUSCAS; i++) {
        volatile Produto *r = hash_buscar(t, ids[i]);
        (void)r;
    }
    return tempo_finalizar(inicio);
}

static void analisar_distribuicao(TabelaHash *t, int total) {
    long vazios = 0, max_cadeia = 0, ocupados = 0;
    for (int i = 0; i < t->tamanho; i++) {
        long len = 0;
        for (No *n = t->baldes[i]; n; n = n->proximo) len++;
        if (len == 0) vazios++;
        else { ocupados++; if (len > max_cadeia) max_cadeia = len; }
    }
    double fator_carga = (double)total / t->tamanho;
    printf("\n--- Distribuicao da Tabela Hash ---\n");
    printf("Funcao hash:           %s\n", hash_nome_metodo());
    printf("Tamanho da tabela (m): %d\n", t->tamanho);
    printf("Registros (n):         %d\n", total);
    printf("Fator de carga (n/m):  %.4f\n", fator_carga);
    printf("Total de colisoes:     %ld\n", t->colisoes);
    printf("Baldes ocupados:       %ld\n", ocupados);
    printf("Baldes vazios:         %ld\n", vazios);
    printf("Maior cadeia:          %ld\n", max_cadeia);
}

int main(int argc, char **argv) {
    const char *caminho = (argc > 1) ? argv[1] : "data/dataset2.csv";

    int total = 0;
    Produto *produtos = ler_csv(caminho, &total);
    if (!produtos) return EXIT_FAILURE;

    printf("Registros carregados: %d\n", total);

    TabelaHash *tabela = hash_criar(total);
    if (!tabela) { free(produtos); return EXIT_FAILURE; }

    for (int i = 0; i < total; i++) {
        if (hash_inserir(tabela, produtos[i]) < 0) {
            hash_destruir(tabela);
            free(produtos);
            return EXIT_FAILURE;
        }
    }

    analisar_distribuicao(tabela, total);

    printf("\n--- Validacao das buscas ---\n");
    Produto *r;
    r = hash_buscar(tabela, produtos[0].id);
    printf("Hash inicio  - id %d: %s\n", produtos[0].id, r ? "ENCONTRADO" : "NAO ENCONTRADO");
    r = hash_buscar(tabela, produtos[total/2].id);
    printf("Hash meio    - id %d: %s\n", produtos[total/2].id, r ? "ENCONTRADO" : "NAO ENCONTRADO");
    r = hash_buscar(tabela, produtos[total-1].id);
    printf("Hash final   - id %d: %s\n", produtos[total-1].id, r ? "ENCONTRADO" : "NAO ENCONTRADO");
    r = hash_buscar(tabela, -1);
    printf("Hash inexist - id -1: %s\n", r ? "ENCONTRADO" : "NAO ENCONTRADO");

    int ids[NUM_BUSCAS];
    montar_ids(ids, produtos, total);

    printf("\n========================================\n");
    printf("EXPERIMENTO COMPARATIVO\n");
    printf("Protocolo: %d buscas x %d repeticoes = %d execucoes por metodo\n",
           NUM_BUSCAS, NUM_REPETICOES, NUM_BUSCAS * NUM_REPETICOES);
    printf("Distribuicao: %d inicio | %d meio | %d final | %d inexistentes\n",
           N_INICIO, N_MEIO, N_FINAL, N_INEXISTENTE);
    printf("========================================\n");

    double seq_total = 0.0, hash_total = 0.0;
    double seq_rep[NUM_REPETICOES], hash_rep[NUM_REPETICOES];

    for (int rep = 0; rep < NUM_REPETICOES; rep++) {
        seq_rep[rep]  = bloco_sequencial(produtos, total, ids);
        hash_rep[rep] = bloco_hash(tabela, ids);
        seq_total  += seq_rep[rep];
        hash_total += hash_rep[rep];
        printf("Rep %d | sequencial: %.6f s | hash: %.9f s\n",
               rep + 1, seq_rep[rep], hash_rep[rep]);
    }

    double seq_medio_busca  = seq_total  / (NUM_REPETICOES * NUM_BUSCAS);
    double hash_medio_busca = hash_total / (NUM_REPETICOES * NUM_BUSCAS);

    printf("\n%-22s | %-18s | %s\n", "Metrica", "Busca Sequencial", "Tabela Hash");
    printf("-----------------------+--------------------+-------------------\n");
    printf("%-22s | %18.9f | %.9f\n", "Tempo medio/busca (s)", seq_medio_busca, hash_medio_busca);
    printf("%-22s | %18.6f | %.6f\n", "Tempo total 3 reps (s)", seq_total, hash_total);
    printf("%-22s | %18s | %s\n", "Complexidade media", "O(n)", "O(1 + n/m)");
    printf("%-22s | %18s | %ld\n", "Colisoes", "-", tabela->colisoes);
    if (hash_medio_busca > 0)
        printf("\nSpeedup (sequencial/hash): %.1fx\n", seq_medio_busca / hash_medio_busca);

    FILE *log = fopen("resultados_hash.txt", "w");
    if (log) {
        fprintf(log, "LOG DE BUSCAS - TABELA HASH (Fase II)\n");
        fprintf(log, "Funcao hash: %s\n", hash_nome_metodo());
        fprintf(log, "Dataset: %d registros | Tabela: m = %d | Colisoes: %ld\n",
                total, tabela->tamanho, tabela->colisoes);
        fprintf(log, "Protocolo: %d buscas x %d repeticoes = %d execucoes\n",
                NUM_BUSCAS, NUM_REPETICOES, NUM_BUSCAS * NUM_REPETICOES);

        for (int rep = 0; rep < NUM_REPETICOES; rep++) {
            fprintf(log, "\n========================================\n");
            fprintf(log, "REPETICAO %d  (bloco: %.9f s)\n", rep + 1, hash_rep[rep]);
            fprintf(log, "========================================\n");
            fprintf(log, "%-6s  %-12s  %-8s  %s\n", "Busca", "ID", "Balde", "Cadeia");
            fprintf(log, "------  ------------  --------  ------\n");
            for (int i = 0; i < NUM_BUSCAS; i++) {
                unsigned long b = hash_funcao(ids[i], tabela->tamanho);
                long len = 0;
                for (No *n = tabela->baldes[b]; n; n = n->proximo) len++;
                fprintf(log, "%-6d  %-12d  %-8lu  %ld\n", i + 1, ids[i], b, len);
            }
        }

        fprintf(log, "\n========================================\n");
        fprintf(log, "RESUMO FINAL\n");
        fprintf(log, "========================================\n");
        for (int rep = 0; rep < NUM_REPETICOES; rep++)
            fprintf(log, "Rep %d: bloco hash = %.9f s\n", rep + 1, hash_rep[rep]);
        fprintf(log, "Tempo medio por busca (hash): %.9f s\n", hash_medio_busca);
        fclose(log);
        printf("\nLog detalhado salvo em: resultados_hash.txt\n");
    }

    FILE *logs = fopen("resultados.txt", "w");
    if (logs) {
        fprintf(logs, "LOG DE BUSCAS - BUSCA SEQUENCIAL (Fase I, re-medido)\n");
        fprintf(logs, "Dataset: %d registros\n", total);
        fprintf(logs, "Protocolo: %d buscas x %d repeticoes = %d execucoes\n",
                NUM_BUSCAS, NUM_REPETICOES, NUM_BUSCAS * NUM_REPETICOES);

        for (int rep = 0; rep < NUM_REPETICOES; rep++) {
            fprintf(logs, "\n========================================\n");
            fprintf(logs, "REPETICAO %d  (bloco: %.6f s)\n", rep + 1, seq_rep[rep]);
            fprintf(logs, "========================================\n");
            fprintf(logs, "%-6s  %-12s  %s\n", "Busca", "ID", "Indice");
            fprintf(logs, "------  ------------  ------\n");
            for (int i = 0; i < NUM_BUSCAS; i++) {
                int idx = busca_sequencial(produtos, total, ids[i]);
                fprintf(logs, "%-6d  %-12d  %d\n", i + 1, ids[i], idx);
            }
        }

        fprintf(logs, "\n========================================\n");
        fprintf(logs, "RESUMO FINAL\n");
        fprintf(logs, "========================================\n");
        for (int rep = 0; rep < NUM_REPETICOES; rep++)
            fprintf(logs, "Rep %d: bloco sequencial = %.6f s\n", rep + 1, seq_rep[rep]);
        fprintf(logs, "Tempo medio por busca (sequencial): %.9f s\n", seq_medio_busca);
        fclose(logs);
    }

    hash_destruir(tabela);
    free(produtos);
    return EXIT_SUCCESS;
}