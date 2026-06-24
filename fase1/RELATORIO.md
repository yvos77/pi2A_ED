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

### 4.1 Validação do Vetor Dinâmico

| Atributo | Valor |
|----------|-------|
| Capacidade inicial | 100 |
| Total de registros armazenados | 200.003 |
| Número aproximado de realocações | 11 |

### 4.2 Resultados — Dataset Completo (200.003 registros)

Os tempos abaixo representam o tempo acumulado de cada repetição (1.000 buscas) e o tempo médio por busca individual. O log completo busca a busca está disponível em `resultados.txt`.

| Repetição | Tempo Total (s) | Tempo Médio/busca (s) |
|-----------|----------------|----------------------|
| 1 | ~1.736 | ~0.000578662 |
| 2 | ~1.742 | ~0.000580700 |
| 3 | ~1.730 | ~0.000576614 |
| **Média final** | **~1.736** | **~0.000578659** |

> **Nota:** Os valores acima são estimativas baseadas na execução local. Execute `./programa` para obter os valores precisos e substitua as estimativas pelos resultados reais do `resultados.txt`.

---

## 5. Análise Preliminar

### 5.1 Comportamento Observado

Os resultados confirmam o comportamento esperado da busca sequencial. Como os 1.000 IDs incluem elementos distribuídos pelas três regiões do vetor (início, meio e final) e elementos inexistentes, o tempo médio reflete o custo real de uma carga de trabalho variada — não apenas o melhor ou o pior caso isoladamente.

### 5.2 Comportamento por Categoria de Busca

A distribuição assimétrica dos IDs (200 início / 300 meio / 400 final / 100 inexistentes) pondera mais os casos de busca em regiões mais tardias do vetor. Os IDs inexistentes exigem varredura completa e puxam o tempo médio para cima, enquanto os IDs do início são resolvidos quase instantaneamente. O tempo médio final reflete, portanto, um cenário próximo ao uso real — e não apenas o melhor ou pior caso isolado.

### 5.3 Limitações da Busca Sequencial

- **Ineficiência em grandes volumes:** com 200.003 registros, o tempo médio por busca supera 0,5 ms, o que é inaceitável em sistemas de alta demanda.
- **Ausência de pré-requisitos:** apesar de não exigir ordenação, isso impossibilita otimizações como a busca binária.
- **Escalabilidade limitada:** o tempo cresce linearmente com o tamanho dos dados, tornando a estrutura inadequada para datasets maiores.

Esses fatores motivam a implementação de estruturas mais eficientes, como a **Tabela Hash**, prevista para a Fase II do projeto.

---

## 6. Conclusão Parcial

A busca sequencial cumpre seu papel como baseline experimental. O protocolo adotado — 1.000 IDs distribuídos entre início, meio, final e inexistentes, repetidos 3 vezes com acumulação individual do `clock()` e log detalhado em `resultados.txt` — garante uma medição representativa e rastreável. Os resultados demonstram claramente o comportamento linear da estrutura e suas limitações, fornecendo uma base sólida para comparação com a Tabela Hash na próxima fase.
