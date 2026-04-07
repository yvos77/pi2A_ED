#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csv_reader.h"

#define TAMANHO_INICIAL 100
#define LINHA_MAX 256

Produto* ler_csv(const char *caminho, int *total) {
    FILE *arquivo = fopen(caminho, "r");
    if (!arquivo) {
        fprintf(stderr, "Erro: arquivo '%s' nao encontrado.\n", caminho);
        return NULL;
    }

    int capacidade = TAMANHO_INICIAL;
    Produto *vetor = malloc(capacidade * sizeof(Produto));
    if (!vetor) {
        fprintf(stderr, "Erro: falha ao alocar memoria.\n");
        fclose(arquivo);
        return NULL;
    }

    char linha[LINHA_MAX];
    *total = 0;

    // Pula o cabeçalho
    if (!fgets(linha, LINHA_MAX, arquivo)) {
        fprintf(stderr, "Erro: arquivo vazio ou formato invalido.\n");
        free(vetor);
        fclose(arquivo);
        return NULL;
    }

    while (fgets(linha, LINHA_MAX, arquivo)) {
        // Ignora linhas em branco
        if (linha[0] == '\n' || linha[0] == '\r') continue;

        Produto p;
        int campos = sscanf(linha, "%d,%50[^,],%30[^,],%f",
                            &p.id, p.nome, p.categoria, &p.valor);

        if (campos != 4) {
            fprintf(stderr, "Aviso: linha com formato invalido ignorada: %s", linha);
            continue;
        }

        // Cresce o vetor se necessário
        if (*total >= capacidade) {
            capacidade *= 2;
            Produto *tmp = realloc(vetor, capacidade * sizeof(Produto));
            if (!tmp) {
                fprintf(stderr, "Erro: falha ao realocar memoria.\n");
                free(vetor);
                fclose(arquivo);
                return NULL;
            }
            vetor = tmp;
        }

        vetor[(*total)++] = p;
    }

    fclose(arquivo);
    return vetor;
}