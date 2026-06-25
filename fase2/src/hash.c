#include <stdio.h>
#include <stdlib.h>
#include "hash.h"

#define A_KNUTH 0.6180339887498949

unsigned long hash_funcao(int chave, int tamanho) {
#if HASH_METODO == HASH_DIVISAO
    long m = (long)tamanho;
    return (unsigned long)(((chave % m) + m) % m);
#else
    unsigned long k = (unsigned long)(long long)chave;
    double prod = (double)k * A_KNUTH;
    double frac = prod - (double)(unsigned long long)prod;
    return (unsigned long)((double)tamanho * frac);
#endif
}

const char* hash_nome_metodo(void) {
#if HASH_METODO == HASH_DIVISAO
    return "Divisao  h(k) = k mod m";
#else
    return "Multiplicacao (Knuth)  h(k) = floor(m * frac(k*A))";
#endif
}

TabelaHash* hash_criar(int tamanho) {
    TabelaHash *t = malloc(sizeof(TabelaHash));
    if (!t) {
        fprintf(stderr, "Erro: falha ao alocar TabelaHash.\n");
        return NULL;
    }

    t->baldes = calloc((size_t)tamanho, sizeof(No *));
    if (!t->baldes) {
        fprintf(stderr, "Erro: falha ao alocar baldes da tabela.\n");
        free(t);
        return NULL;
    }

    t->tamanho = tamanho;
    t->colisoes = 0;
    return t;
}

int hash_inserir(TabelaHash *t, Produto p) {
    unsigned long indice = hash_funcao(p.id, t->tamanho);

    No *novo = malloc(sizeof(No));
    if (!novo) {
        fprintf(stderr, "Erro: falha ao alocar no da tabela.\n");
        return -1;
    }
    novo->produto = p;

    int houve_colisao = (t->baldes[indice] != NULL);
    if (houve_colisao) {
        t->colisoes++;
    }

    novo->proximo = t->baldes[indice];
    t->baldes[indice] = novo;
    return houve_colisao;
}

Produto* hash_buscar(TabelaHash *t, int id) {
    unsigned long indice = hash_funcao(id, t->tamanho);
    No *atual = t->baldes[indice];

    while (atual) {
        if (atual->produto.id == id) {
            return &atual->produto;
        }
        atual = atual->proximo;
    }
    return NULL;
}

void hash_destruir(TabelaHash *t) {
    if (!t) return;
    for (int i = 0; i < t->tamanho; i++) {
        No *atual = t->baldes[i];
        while (atual) {
            No *prox = atual->proximo;
            free(atual);
            atual = prox;
        }
    }
    free(t->baldes);
    free(t);
}