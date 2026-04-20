# Relatório Técnico — Fase I
## Análise de Desempenho da Busca Sequencial

**Disciplina:** Estrutura de Dados  
**Projeto:** Projeto Integrador — Fase I  
**Data:** abril de 2026  

---

## 1. Descrição do Problema

O objetivo desta fase é estabelecer um baseline experimental para análise de desempenho de algoritmos de busca. Para isso, foi implementada a busca sequencial sobre um vetor dinâmico de produtos carregado a partir de um arquivo CSV.

A busca sequencial percorre o vetor elemento a elemento até encontrar o registro com o `id` buscado, ou retornar `-1` caso não seja encontrado. Por ser simples e linear, serve como ponto de referência para comparação com estruturas mais eficientes, como a Tabela Hash, a ser implementada na Fase II.

---

## 2. Caracterização do Dataset

O dataset utilizado é um arquivo CSV contendo registros de produtos com a seguinte estrutura:

```c
typedef struct {
    int id;
    char nome[51];
    char categoria[31];
    float valor;
} Produto;
```

| Atributo | Descrição |
|----------|-----------|
| `id` | Identificador único do produto |
| `nome` | Nome do produto (até 50 caracteres) |
| `categoria` | Categoria do produto (até 30 caracteres) |
| `valor` | Preço do produto em reais |

- **Arquivo:** `data/dataset2.csv`
- **Total de registros carregados:** 200.003
- **Formato:** CSV com cabeçalho na primeira linha

---

## 3. Metodologia de Testes

### 3.1 Protocolo Experimental

Os testes seguiram o seguinte protocolo:

- O vetor foi completamente carregado antes do início das medições.
- Foram selecionados **1.000 IDs** distribuídos em quatro categorias (detalhadas abaixo), usando espaçamento uniforme dentro de cada região do vetor.
- O mesmo conjunto de 1.000 IDs foi buscado **3 vezes consecutivas** com os exatos mesmos IDs.
- O `clock()` foi medido **individualmente para cada busca** e acumulado.
- O **tempo médio por busca** foi calculado dividindo o tempo total acumulado pelo número total de execuções (1.000 × 3 = 3.000).
- Um arquivo `resultados.txt` é gerado com o log completo de cada busca (ID e tempo individual) nas 3 repetições.

### 3.2 Distribuição dos 1.000 IDs

| Categoria | Quantidade | Região do vetor |
|-----------|-----------|-----------------|
| Início | 200 | Índices `[0, total/3)` |
| Meio | 300 | Índices `[total/3, 2×total/3)` |
| Final | 400 | Índices `[2×total/3, total)` |
| Inexistentes | 100 | IDs negativos (`-1` a `-100`) |
| **Total** | **1.000** | — |

Os IDs de cada região foram selecionados de forma **uniformemente espaçada** dentro do intervalo correspondente, garantindo cobertura representativa de toda a região.

### 3.3 Dataset Utilizado

Os testes foram realizados sobre o dataset completo com **200.003 registros**, sem subdivisões por tamanho.

---

## 4. Resultados Obtidos

> *(A ser preenchido pelo Contribuidor 2 após execução do programa.)*

### 4.1 Validação do Vetor Dinâmico

| Atributo | Valor |
|----------|-------|
| Capacidade inicial | 100 |
| Total de registros armazenados | 200.003 |
| Número aproximado de realocações | 11 |

### 4.2 Resultados — Dataset Completo

*(pendente)*

---

## 5. Análise Preliminar

> *(A ser preenchido pelo Contribuidor 2.)*

---

## 6. Conclusão Parcial

> *(A ser preenchido pelo Contribuidor 2.)*
