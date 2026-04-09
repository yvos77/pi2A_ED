#include "busca.h"

int busca_sequencial(Produto *vetor, int tamanho, int id) {
    for (int i = 0; i < tamanho; i++) {
        if (vetor[i].id == id) {
            return i;
        }
    }
    return -1;
}