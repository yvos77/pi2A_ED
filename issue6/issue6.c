#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
    #include <windows.h>
#endif

// --- ISSUE 2: Armazenamento dos registros em vetor dinâmico ---
typedef struct {
    int id;
    char nome[51];
    char categoria[31];
    float valor;
} Produto;

// --- ISSUE 3: Implementação da busca sequencial por id ---
int busca_sequencial(Produto* vetor, int total_registros, int id_buscado) {
    for (int i = 0; i < total_registros; i++) {
        if (vetor[i].id == id_buscado) {
            return i; 
        }
    }
    return -1; 
}

// --- ISSUE 1: Leitura do CSV ---
Produto* carregar_dados(const char* nome_arquivo, int* total_registros) {
    printf("[ISSUE 1] Abrindo arquivo: %s\n", nome_arquivo);
    FILE* arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        fprintf(stderr, "[ERRO] Arquivo '%s' nao encontrado.\n", nome_arquivo);
        return NULL;
    }

    printf("[ISSUE 2] Inicializando vetor dinamico com malloc...\n");
    int capacidade = 10000; 
    Produto* vetor = (Produto*)malloc(capacidade * sizeof(Produto));
    
    char linha[256];
    int cont = 0;

    fgets(linha, sizeof(linha), arquivo); // Skip header

    while (fgets(linha, sizeof(linha), arquivo)) {
        int id;
        char nome[51], categoria[31];
        float valor;

        if (sscanf(linha, "%d,%50[^,],%30[^,],%f", &id, nome, categoria, &valor) == 4) {
            if (cont >= capacidade) {
                capacidade *= 2; 
                vetor = (Produto*)realloc(vetor, capacidade * sizeof(Produto));
            }
            vetor[cont].id = id;
            strncpy(vetor[cont].nome, nome, 50);
            vetor[cont].nome[50] = '\0';
            strncpy(vetor[cont].categoria, categoria, 30);
            vetor[cont].categoria[30] = '\0';
            vetor[cont].valor = valor;
            cont++;
        }
    }
    fclose(arquivo);
    printf("[ISSUE 2] Vetor finalizado. Memoria alocada para %d registros.\n", cont);
    *total_registros = cont;
    return vetor;
}

// --- ISSUE 4, 5 & 6: Medição, Testes e Experimentos ---
void executar_experimento(Produto* base_dados, int total_dados) {
    printf("\n[ISSUE 6] Iniciando Experimentos Experimentais...\n");
    
    int num_testes_aleatorios = 10;
    int buscas_por_teste = 1000; 
    double soma_tempos_totais = 0;

    printf("[ISSUE 5] Executando %d rodadas de %d buscas consecutivas.\n", num_testes_aleatorios, buscas_por_teste);

    for (int i = 0; i < num_testes_aleatorios; i++) {
        int id_procurado = base_dados[rand() % total_dados].id;

        // --- ISSUE 4: Medição do tempo de busca ---
        clock_t t_inicio = clock();
        
        for(int j = 0; j < buscas_por_teste; j++) {
            busca_sequencial(base_dados, total_dados, id_procurado);
        }
        
        clock_t t_fim = clock();
        
        double tempo_total_ms = ((double)(t_fim - t_inicio) / CLOCKS_PER_SEC) * 1000.0;
        double tempo_medio_ms = tempo_total_ms / buscas_por_teste;
        
        soma_tempos_totais += tempo_medio_ms;
        
        printf("  > Rodada %d: ID %d | Tempo Medio: %.6f ms\n", i + 1, id_procurado, tempo_medio_ms);
    }

    printf("\n[ISSUE 7] Esboço do Relatorio Gerado:\n");
    printf("--------------------------------------------------\n");
    printf("RESULTADO FINAL: Tempo medio global de %.6f ms\n", soma_tempos_totais / num_testes_aleatorios);
    printf("--------------------------------------------------\n");
}

int main() {
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
    #endif

    srand(time(NULL));
    int total_dados = 0;
    const char* arquivo_csv = "../dataset2.csv"; 

    Produto* base_dados = carregar_dados(arquivo_csv, &total_dados);

    if (base_dados != NULL && total_dados > 0) {
        executar_experimento(base_dados, total_dados);
        free(base_dados);
        printf("\n[ISSUE 8] Projeto concluido e memoria liberada.\n");
    }

    return 0;
}

// #8 - Entregar relatório técnico final

// #7 - Esboço do relatório técnico

// #6 - Iniciar os experimentos

// #5 - Execução de múltiplos testes para cálculo do tempo médio de busca

// #4 - Implementação de função para medição do tempo de busca

// #3 - Implementação da função de busca sequencial por id

// #2 - Armazenamento dos registros em vetor dinâmico

// #1 - Leitura do CSV

