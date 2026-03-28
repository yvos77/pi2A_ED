#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

// Estrutura do Produto conforme especificado
typedef struct {
    int id;
    char nome[51];
    char categoria[31];
    float valor;
} Produto;

// Função para ler o CSV e armazenar no vetor dinâmico
Produto* carregar_dados(const char* nome_arquivo, int* total_registros) {
    FILE* arquivo = fopen(nome_arquivo, "r");
    
    // Verificação de erro 1: Arquivo inexistente
    if (arquivo == NULL) {
        fprintf(stderr, "Erro crítico: Nao foi possivel abrir o arquivo '%s'. Verifique se ele esta na mesma pasta do codigo.\n", nome_arquivo);
        return NULL;
    }

    // Alocação inicial (capacidade inicial de 10.000 para evitar muitos reallocs)
    int capacidade = 10000; 
    Produto* vetor = (Produto*)malloc(capacidade * sizeof(Produto));
    
    // Verificação de erro 2: Falha de alocação de memória
    if (vetor == NULL) {
        fprintf(stderr, "Erro crítico: Falha na alocacao inicial de memoria.\n");
        fclose(arquivo);
        return NULL;
    }

    char linha[256];
    int cont = 0;
    int linhas_ignoradas = 0;

    printf("Iniciando leitura do arquivo '%s'. Isso pode levar alguns segundos...\n", nome_arquivo);

    // Lendo o arquivo linha por linha
    while (fgets(linha, sizeof(linha), arquivo)) {
        int id;
        char nome[51];
        char categoria[31];
        float valor;

        // Tenta extrair os dados respeitando o limite de caracteres das strings
        // O formato %[^,] lê tudo até a próxima vírgula
        if (sscanf(linha, "%d,%50[^,],%30[^,],%f", &id, nome, categoria, &valor) == 4) {
            
            // Se o vetor lotar, dobramos a capacidade dinamicamente (ótimo para 2 milhões de dados)
            if (cont >= capacidade) {
                capacidade *= 2; 
                Produto* temp = (Produto*)realloc(vetor, capacidade * sizeof(Produto));
                
                if (temp == NULL) {
                    fprintf(stderr, "Erro crítico: Memoria insuficiente ao tentar expandir o vetor na linha %d.\n", cont + 1);
                    break; // Para a leitura, mas salva o que já leu
                }
                vetor = temp;
            }

            // Salvando no vetor
            vetor[cont].id = id;
            
            strncpy(vetor[cont].nome, nome, 50);
            vetor[cont].nome[50] = '\0'; // Garantindo o fim da string
            
            strncpy(vetor[cont].categoria, categoria, 30);
            vetor[cont].categoria[30] = '\0';
            
            vetor[cont].valor = valor;

            cont++;
        } else {
            // Conta a primeira linha (cabeçalho) ou linhas vazias/com erro
            linhas_ignoradas++;
        }
    }

    fclose(arquivo);

    if (linhas_ignoradas > 0) {
        printf("Aviso: %d linha(s) ignorada(s) (provavelmente o cabecalho ou linhas vazias).\n", linhas_ignoradas);
    }

    // Ajuste final: Reduz o tamanho do vetor para liberar a memória que sobrou
    Produto* ajuste_final = (Produto*)realloc(vetor, cont * sizeof(Produto));
    if (ajuste_final != NULL) {
        vetor = ajuste_final;
    }

    *total_registros = cont;
    return vetor;
}

int main() {
    SetConsoleOutputCP(CP_UTF8);

    int total_dados = 0;
    
    // Nome do arquivo atualizado para o que você fez upload
    const char* arquivo_csv = "dataset2.csv"; 

    // Executa a leitura
    Produto* base_dados = carregar_dados(arquivo_csv, &total_dados);

    // Se deu tudo certo, mostra os resultados
    if (base_dados != NULL) {
        printf("\n SUCESSO! %d registros foram carregados na memoria.\n", total_dados);
        
        // Imprime o primeiro e o último registro para provar que leu certo
        if (total_dados > 0) {
            printf("--------------------------------------------------\n");
            printf("Primeiro registro lido:\n");
            printf("ID: %d | Nome: %s | Categ: %s | Valor: R$ %.2f\n", 
                    base_dados[0].id, base_dados[0].nome, base_dados[0].categoria, base_dados[0].valor);
            
            printf("\nUltimo registro lido:\n");
            printf("ID: %d | Nome: %s | Categ: %s | Valor: R$ %.2f\n", 
                    base_dados[total_dados-1].id, base_dados[total_dados-1].nome, base_dados[total_dados-1].categoria, base_dados[total_dados-1].valor);
            printf("--------------------------------------------------\n");
        }

        // Libera a memória RAM no final do programa
        free(base_dados);
        printf("\nMemoria liberada com sucesso. Encerrando.\n");
    }

    return 0;
}