#ifndef HASH_H
#define HASH_H

#include "csv_reader.h"

#define HASH_DIVISAO       0
#define HASH_MULTIPLICACAO 1

#ifndef HASH_METODO
#define HASH_METODO HASH_DIVISAO
#endif

typedef struct No {
    Produto produto;
    struct No *proximo;
} No;

typedef struct {
    No **baldes;
    int tamanho;
    long colisoes;
} TabelaHash;

TabelaHash* hash_criar(int tamanho);
unsigned long hash_funcao(int chave, int tamanho);
int hash_inserir(TabelaHash *t, Produto p);
Produto* hash_buscar(TabelaHash *t, int id);
void hash_destruir(TabelaHash *t);
const char* hash_nome_metodo(void);

#endif