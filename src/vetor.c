#include <stdio.h>
#include <stdlib.h>
#include "vetor.h"

VetorDinamico* vetor_criar(int capacidade_inicial) {
    VetorDinamico *v = malloc(sizeof(VetorDinamico));
    if (!v) {
        fprintf(stderr, "Erro: falha ao alocar VetorDinamico.\n");
        return NULL;
    }

    v->dados = malloc(capacidade_inicial * sizeof(Produto));
    if (!v->dados) {
        fprintf(stderr, "Erro: falha ao alocar dados do vetor.\n");
        free(v);
        return NULL;
    }

    v->tamanho = 0;
    v->capacidade = capacidade_inicial;
    return v;
}

int vetor_inserir(VetorDinamico *v, Produto p) {
    if (v->tamanho >= v->capacidade) {
        v->capacidade *= 2;
        Produto *tmp = realloc(v->dados, v->capacidade * sizeof(Produto));
        if (!tmp) {
            fprintf(stderr, "Erro: falha ao realocar vetor.\n");
            return 0;
        }
        v->dados = tmp;
    }

    v->dados[v->tamanho++] = p;
    return 1;
}

void vetor_destruir(VetorDinamico *v) {
    if (v) {
        free(v->dados);
        free(v);
    }
}