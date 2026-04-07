#ifndef CSV_READER_H
#define CSV_READER_H

typedef struct {
    int id;
    char nome[51];
    char categoria[31];
    float valor;
} Produto;

// Lê o CSV e retorna ponteiro para array de Produto.
// *total recebe a quantidade de registros lidos.
// Retorna NULL em caso de erro.
Produto* ler_csv(const char *caminho, int *total);

#endif