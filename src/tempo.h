#ifndef TEMPO_H
#define TEMPO_H

#include <time.h>

typedef struct {
    double tempo_total;
    double tempo_medio;
    int num_buscas;
} ResultadoTempo;

clock_t tempo_iniciar(void);
double tempo_finalizar(clock_t inicio);
ResultadoTempo tempo_calcular_medio(double tempo_total, int num_buscas);
void tempo_imprimir(ResultadoTempo resultado);

#endif