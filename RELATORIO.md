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

- O vetor foi completamente carregado antes do início das medições
- Foram realizadas **1.000 buscas consecutivas** por cenário
- Os testes foram **repetidos 3 vezes**, sendo apresentada a média
- A medição de tempo utilizou a função `clock()` da biblioteca `<time.h>`

### 3.2 Cenários de Busca

| Cenário | Descrição |
|---------|-----------|
| Início | Elemento no índice 0 do vetor |
| Meio | Elemento no índice central do vetor |
| Final | Elemento no último índice do vetor |
| Inexistente | Elemento com `id = -1` (não presente no vetor) |

### 3.3 Experimentos com Diferentes Tamanhos

Para analisar a relação entre tamanho do vetor e tempo de busca, os testes foram executados com quatro subconjuntos do dataset:

| Experimento | Tamanho |
|-------------|---------|
| 25% | 50.000 registros |
| 50% | 100.001 registros |
| 75% | 150.002 registros |
| 100% | 200.003 registros |

---

## 4. Resultados Obtidos

### 4.1 Validação do Vetor Dinâmico

| Atributo | Valor |
|----------|-------|
| Capacidade inicial | 100 |
| Total de registros armazenados | 200.003 |
| Número de realocações | 11 |

### 4.2 Resultados por Tamanho de Entrada

#### 25% — 50.000 registros

| Cenário | Tempo Total (s) | Tempo Médio (s) |
|---------|----------------|-----------------|
| Início | 0.000009 | 0.000000009 |
| Meio | 0.146211 | 0.000146211 |
| Final | 0.627075 | 0.000627075 |
| Inexistente | 0.693520 | 0.000693520 |

#### 50% — 100.001 registros

| Cenário | Tempo Total (s) | Tempo Médio (s) |
|---------|----------------|-----------------|
| Início | 0.000008 | 0.000000008 |
| Meio | 0.778666 | 0.000778666 |
| Final | 1.656987 | 0.001656987 |
| Inexistente | 1.379244 | 0.001379244 |

#### 75% — 150.002 registros

| Cenário | Tempo Total (s) | Tempo Médio (s) |
|---------|----------------|-----------------|
| Início | 0.000009 | 0.000000009 |
| Meio | 1.003548 | 0.001003548 |
| Final | 2.023789 | 0.002023789 |
| Inexistente | 2.032740 | 0.002032740 |

#### 100% — 200.003 registros

| Cenário | Tempo Total (s) | Tempo Médio (s) |
|---------|----------------|-----------------|
| Início | 0.000008 | 0.000000008 |
| Meio | 1.406636 | 0.001406636 |
| Final | 2.687563 | 0.002687563 |
| Inexistente | 2.725924 | 0.002725924 |

---

## 5. Análise Preliminar

### 5.1 Comportamento Observado

Os resultados confirmam o comportamento esperado da busca sequencial. O tempo de busca cresce proporcionalmente à posição do elemento no vetor: elementos no início são encontrados quase instantaneamente, enquanto elementos no final ou inexistentes exigem a varredura de todo o vetor.

### 5.2 Relação entre Tamanho do Vetor e Tempo de Busca

A tabela abaixo resume o tempo de busca no pior caso (elemento no final) para cada tamanho:

| Tamanho | Tempo Final (s) | Fator de crescimento |
|---------|----------------|----------------------|
| 50.000 | 0.000627 | — |
| 100.001 | 0.001657 | ~2.6x |
| 150.002 | 0.002024 | ~3.2x |
| 200.003 | 0.002688 | ~4.3x |

O crescimento do tempo de busca acompanha o crescimento do vetor, evidenciando a complexidade **O(n)** da busca sequencial.

### 5.3 Limitações da Busca Sequencial

- **Ineficiência em grandes volumes:** com 200.003 registros, cada busca no pior caso leva ~2.7ms, o que é inaceitável em sistemas de alta demanda.
- **Ausência de pré-requisitos:** apesar de não exigir ordenação, isso impossibilita otimizações como a busca binária.
- **Escalabilidade limitada:** o tempo cresce linearmente com o tamanho dos dados, tornando a estrutura inadequada para datasets maiores.

Esses fatores motivam a implementação de estruturas mais eficientes, como a **Tabela Hash**, prevista para a Fase II do projeto.

---

## 6. Conclusão Parcial

A busca sequencial cumpre seu papel como baseline experimental. Os resultados obtidos demonstram claramente seu comportamento linear e suas limitações, fornecendo uma base sólida para comparação com a Tabela Hash na próxima fase.