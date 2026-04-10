#ifndef BUSCA_H
#define BUSCA_H

#include "csv_reader.h"

// Busca sequencial por id.
// Retorna o índice do elemento no vetor, ou -1 se não encontrado.
int busca_sequencial(Produto *vetor, int tamanho, int id);

#endif