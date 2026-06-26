# Relatório Técnico — Fase II
## Tabela Hash com Encadeamento: Análise Comparativa

**Disciplina:** Estrutura de Dados
**Projeto:** Projeto Integrador — Fase II
**Função hash do grupo:** Resto da divisão simples — `h(x) = x mod m`
**Data:** junho de 2026

---

## 1. Descrição do Problema

A Fase I estabeleceu a busca sequencial como baseline experimental, com complexidade O(n). Esta fase implementa uma **Tabela Hash com tratamento de colisões por encadeamento separado** e valida experimentalmente seu ganho de desempenho sobre o mesmo dataset de 200.003 produtos.

A função hash atribuída ao grupo é o **resto da divisão simples**, `h(x) = x mod m`, com `x` igual ao `id` do registro e `m` igual à quantidade de registros do dataset.

---

## 2. Caracterização do Dataset

| Atributo | Valor |
|----------|-------|
| Arquivo | `data/dataset2.csv` |
| Total de registros carregados | **200.003** |
| Faixa dos IDs | 1 a 200.003, **únicos e sem duplicatas** |
| Chave de busca | campo `id` (inteiro) |

A estrutura de cada registro:

```c
typedef struct {
    int id;
    char nome[51];
    char categoria[31];
    float valor;
} Produto;
```

> **Característica decisiva:** os IDs cobrem exatamente o intervalo contíguo `[1, 200.003]` e o tamanho da tabela é `m = 200.003`. Como se verá na Seção 5, essa coincidência entre o conjunto de chaves e o tamanho da tabela tem efeito direto e marcante sobre a contagem de colisões da função da divisão.

---

## 3. Implementação da Tabela Hash

### 3.1 Estrutura

- **Tamanho da tabela:** `m = n = 200.003` (conforme requisito: tamanho igual ao volume de dados).
- **Tratamento de colisões:** encadeamento separado. Cada balde é a cabeça de uma lista ligada simples (`struct No`).
- **Inserção:** O(1) — inserção no início da lista do balde.
- **Busca:** percorre a lista do balde até achar o `id` ou retornar `NULL`.

### 3.2 Função Hash do Grupo

```c
h(x) = x mod m
```

onde `x` é o `id` do registro e `m = 200.003`. É o **método da divisão simples** — rápido (uma única operação de módulo) e direto. Sua qualidade de distribuição depende da escolha de `m` e do padrão das chaves.

### 3.3 Contabilização de Colisões

Uma colisão é contabilizada toda vez que uma inserção cai em um balde **já ocupado** (`baldes[i] != NULL` antes da inserção). O contador `colisoes` acumula esse total durante a carga.

**Validação do contador.** Para comprovar que o mecanismo de contagem funciona — e não apenas retorna zero por inércia — o código mantém uma segunda função hash selecionável (multiplicação de Knuth, via `make HASH=1`). Executado sobre o **mesmo** dataset, o contador registra **22.650 colisões** com a função da multiplicação e **0 colisões** com a função da divisão. Isso demonstra que o contador está operacional: o zero da divisão é um resultado real da função, não uma falha de instrumentação.

---

## 4. Metodologia de Testes

O protocolo replica a Fase I para garantir comparação justa:

- A tabela é **completamente carregada** antes de qualquer medição.
- **1.000 IDs** selecionados com espaçamento uniforme, distribuídos em quatro categorias:

| Categoria | Quantidade | Região |
|-----------|-----------|--------|
| Início | 200 | `[0, total/3)` |
| Meio | 300 | `[total/3, 2·total/3)` |
| Final | 400 | `[2·total/3, total)` |
| Inexistentes | 100 | IDs negativos (`-1` a `-100`) |
| **Total** | **1.000** | — |

- O **mesmo** conjunto de 1.000 IDs é usado para os dois métodos (sequencial e hash) e é **repetido nas 3 repetições**.
- Cada repetição mede o **tempo do bloco completo de 1.000 buscas** com `clock()`; o tempo médio por busca = tempo total ÷ número de execuções. (Medir o bloco, e não cada busca isolada, é necessário porque uma única busca hash — na casa dos nanossegundos — fica abaixo da resolução do `clock()`.)
- O protocolo é repetido **3 vezes** (3 × 1.000 = 3.000 execuções por método); apresenta-se a média final.
- Logs gerados: `resultados.txt` (sequencial) e `resultados_hash.txt` (hash), ambos com as 3 repetições completas (3.000 buscas registradas em cada).

---

## 5. Resultados Obtidos

### 5.1 Resultados Estruturais (determinísticos)

| Métrica | Valor |
|---------|-------|
| Registros carregados | 200.003 |
| Tamanho da tabela (m) | 200.003 |
| Fator de carga (n/m) | 1,0000 |
| **Total de colisões** | **0** |
| Baldes ocupados | 200.003 |
| Baldes vazios | 0 |
| Maior cadeia | **1** |

Com a função da divisão, **não houve uma única colisão**: cada um dos 200.003 baldes recebeu exatamente um registro, e toda cadeia tem comprimento 1.

### 5.2 Por que zero colisões? — Hash Perfeito

O resultado decorre diretamente da estrutura das chaves. Os IDs formam o conjunto `{1, 2, …, 200.003}` e `m = 200.003`. Para qualquer `x` nesse intervalo, `x mod m` produz um índice distinto (`1 → 1`, `2 → 2`, …, `200.002 → 200.002`, `200.003 → 0`). A função torna-se uma **bijeção** entre chaves e baldes — ou seja, um **hash perfeito** para este dataset específico.

Esse é o melhor caso possível: a tabela hash atinge busca em O(1) **de pior caso**, não apenas amortizado, pois nenhuma busca percorre mais de um nó.

É importante registrar, porém, que essa perfeição é um **artefato da estrutura particular das chaves**, e não uma propriedade geral da função da divisão. Ela depende de três condições simultâneas: chaves contíguas, sem repetições, e `m` igual à cardinalidade do conjunto. Bastaria um único `id` fora de `[1, m]` — ou um `m` diferente — para reintroduzir colisões. A Seção 6.2 discute essa fragilidade.

### 5.3 Tempos por Repetição

Resultados medidos na execução do grupo (compilação com `-O2`):

| Repetição | Busca Sequencial (s) | Tabela Hash (s) |
|-----------|----------------------|-----------------|
| 1 | 0,507229 | 0,000063 |
| 2 | 0,546960 | 0,000049 |
| 3 | 0,544381 | 0,000026 |
| **Total (3 reps)** | **1,598570** | **0,000138** |

### 5.4 Tabela Comparativa Final

| Métrica | Busca Sequencial | Tabela Hash |
|---------|------------------|-------------|
| Tempo total (3 × 1.000 buscas) | 1,598570 s | 0,000138 s |
| **Tempo médio por busca** | **0,000532857 s** | **0,000000046 s** |
| Complexidade teórica | O(n) | O(1) |
| Colisões | — | 0 |
| **Speedup** | 1× (referência) | **≈ 11.584×** |

> Os tempos absolutos dependem da máquina; as contagens estruturais (registros, colisões, distribuição) são determinísticas. Ao reexecutar em outro computador, apenas os tempos mudam.

---

## 6. Análise Interpretativa

### 6.1 Desempenho Teórico vs. Prático

A teoria prevê busca sequencial O(n) e tabela hash O(1). Os dados confirmam: a busca hash foi cerca de **onze mil vezes mais rápida** (≈ 11.584×). O tempo médio da sequencial (≈ 533 µs) reflete a varredura de, em média, metade dos 200 mil registros nas buscas bem-sucedidas e a varredura completa nas inexistentes; a busca hash (≈ 46 ns) resume-se a calcular `x mod m` e ler um único nó.

Vale uma leitura cuidadosa do número. Em **comparações**, a razão é ainda maior: a sequencial faz, em média, cerca de 100.000 comparações por busca bem-sucedida, contra **1** da hash (sem colisões) — uma razão de ~100.000×. O speedup **medido** (≈ 11.584×) é menor que isso porque cada operação tem custo diferente: a varredura sequencial percorre um vetor contíguo, extremamente amigável ao cache (com *prefetch* do processador), e cada comparação custa pouquíssimo; já cada busca na hash calcula a função e salta para um balde em posição **aleatória** da memória, pagando provável falha de cache e perseguição de ponteiro. Em resumo: a hash faz muito menos trabalho, mas cada acesso seu é mais caro — e, ainda assim, vence por ordens de magnitude.

### 6.2 Eficiência da Função Hash

Para este dataset, a função da divisão é **ótima**: distribuição perfeita, fator de carga 1,0 sem qualquer colisão. Vale, contudo, uma leitura crítica. A excelência aqui não advém de uma virtude intrínseca do método da divisão, mas da coincidência entre o conjunto de chaves e o tamanho da tabela. O método da divisão é sensível ao padrão das chaves: se os IDs apresentassem periodicidades alinhadas a `m`, ou se `m` fosse uma potência de 2, a distribuição poderia degradar-se. A literatura recomenda, por isso, escolher `m` como um número primo distante de potências de 2 — e `200.003` é, de fato, primo, o que reforça a robustez da escolha.

O experimento de controle com a função da multiplicação (22.650 colisões, cadeia máxima 2) mostra o comportamento da estrutura quando a função **embaralha** as chaves em vez de preservá-las: mesmo aí, com fator de carga 1,0, a busca permanece efetivamente O(1). Ou seja, a tabela é robusta sob ambas as funções — a da divisão apenas tira proveito máximo da regularidade das chaves.

### 6.3 Vantagens e Limitações Observadas

**Vantagens da tabela hash:**
- Ganho de ≈ 11.584× no tempo médio de busca.
- Tempo de busca praticamente independente do tamanho do dataset.
- Com a função da divisão neste dataset, busca O(1) de pior caso (cadeia sempre unitária).

**Limitações:**
- Custo de memória maior: além do vetor, mantém-se um array de `m` ponteiros e um nó por registro.
- A perfeição da função da divisão é **frágil**: depende de chaves contíguas e de `m = n`; chaves fora do intervalo ou um `m` mal escolhido reintroduziriam colisões.
- Não preserva ordem nem suporta consultas por intervalo (onde uma árvore de busca seria preferível).

---

## 7. Conclusão

A Fase II confirma experimentalmente o ganho da Tabela Hash sobre a busca sequencial: a complexidade caiu de O(n) para O(1), com speedup medido de aproximadamente **11.584×** no dataset de 200.003 registros. A função hash do grupo — resto da divisão simples — comportou-se como **hash perfeito** neste dataset, sem nenhuma colisão, graças à coincidência entre chaves contíguas e `m = n`, com `m` primo. A análise crítica registra que essa perfeição é específica do conjunto de chaves, e o experimento de controle com a função da multiplicação valida tanto o contador de colisões quanto a robustez geral da estrutura. O resultado fecha o ciclo iniciado na Fase I: do baseline linear à estrutura indexada, com evidência empírica clara do porquê tabelas hash são a estrutura de escolha para busca por chave em grandes volumes.