#ifndef CSV_READER_H
#define CSV_READER_H

typedef struct {
    int id;
    char nome[51];
    char categoria[31];
    float valor;
} Produto;

Produto* ler_csv(const char *caminho, int *total);

#endif
