#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csv_reader.h"
#include "vetor.h"

#define LINHA_MAX 256

Produto* ler_csv(const char *caminho, int *total) {
    FILE *arquivo = fopen(caminho, "r");
    if (!arquivo) {
        fprintf(stderr, "Erro: arquivo '%s' nao encontrado.\n", caminho);
        return NULL;
    }

    VetorDinamico *v = vetor_criar(100);
    if (!v) {
        fclose(arquivo);
        return NULL;
    }

    char linha[LINHA_MAX];
    *total = 0;

    // Pula o cabeçalho
    if (!fgets(linha, LINHA_MAX, arquivo)) {
        fprintf(stderr, "Erro: arquivo vazio ou formato invalido.\n");
        vetor_destruir(v);
        fclose(arquivo);
        return NULL;
    }

    while (fgets(linha, LINHA_MAX, arquivo)) {
        if (linha[0] == '\n' || linha[0] == '\r') continue;

        Produto p;
        int campos = sscanf(linha, "%d,%50[^,],%30[^,],%f",
                            &p.id, p.nome, p.categoria, &p.valor);

        if (campos != 4) {
            fprintf(stderr, "Aviso: linha invalida ignorada: %s", linha);
            continue;
        }

        if (!vetor_inserir(v, p)) {
            vetor_destruir(v);
            fclose(arquivo);
            return NULL;
        }
    }

    fclose(arquivo);

    *total = v->tamanho;
    Produto *resultado = v->dados; // transfere a posse do array
    free(v);                       // libera só a struct, não os dados
    return resultado;
}