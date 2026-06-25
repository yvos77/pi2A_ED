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

/* Stubs temporários para permitir a compilação nesta etapa */
TabelaHash* hash_criar(int tamanho) { (void)tamanho; return NULL; }
int hash_inserir(TabelaHash *t, Produto p) { (void)t; (void)p; return 0; }
Produto* hash_buscar(TabelaHash *t, int id) { (void)t; (void)id; return NULL; }
void hash_destruir(TabelaHash *t) { (void)t; }