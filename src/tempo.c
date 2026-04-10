#include <stdio.h>
#include "tempo.h"

clock_t tempo_iniciar(void) {
    return clock();
}

double tempo_finalizar(clock_t inicio) {
    return (double)(clock() - inicio) / CLOCKS_PER_SEC;
}

ResultadoTempo tempo_calcular_medio(double tempo_total, int num_buscas) {
    ResultadoTempo r;
    r.tempo_total = tempo_total;
    r.num_buscas = num_buscas;
    r.tempo_medio = tempo_total / num_buscas;
    return r;
}

void tempo_imprimir(ResultadoTempo resultado) {
    printf("Numero de buscas:  %d\n", resultado.num_buscas);
    printf("Tempo total:       %.6f segundos\n", resultado.tempo_total);
    printf("Tempo medio:       %.9f segundos\n", resultado.tempo_medio);
}