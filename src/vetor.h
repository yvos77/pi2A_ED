#ifndef VETOR_H
#define VETOR_H

#include "csv_reader.h"

typedef struct {
    Produto *dados;
    int tamanho;
    int capacidade;
} VetorDinamico;

VetorDinamico* vetor_criar(int capacidade_inicial);
int vetor_inserir(VetorDinamico *v, Produto p);
void vetor_destruir(VetorDinamico *v);

#endif