# Análise Experimental Comparativa entre Busca Sequencial e Tabela Hash com Encadeamento

**Projeto Integrador — Estrutura de Dados**
**IESB — Ciência da Computação — 1º semestre de 2026**

* - Yuri Victor de Oliveira e Silva // 2412130171
* - Felipe Oliveira Cunha // 2412130016
* - Gabriel Vieira Braga // 2412130033

---

## Resumo

Este trabalho apresenta a implementação e a avaliação experimental de duas estruturas de busca em C — a busca sequencial sobre vetor dinâmico e a tabela hash com tratamento de colisões por encadeamento separado — aplicadas a um conjunto de 200.003 registros de produtos. A função hash adotada é o resto da divisão simples, `h(x) = x mod m`, com `m` igual ao número de registros. O objetivo é confrontar o comportamento assintótico previsto pela teoria com o desempenho medido na prática. Sob um protocolo padronizado de 1.000 buscas repetidas três vezes, a tabela hash apresentou tempo médio por busca cerca de 11.584 vezes menor que a busca sequencial, confirmando a transição de complexidade de O(n) para O(1). Sobre este dataset, a função da divisão comportou-se como um hash perfeito, sem nenhuma colisão — resultado cuja origem, na estrutura particular das chaves, é analisada criticamente.

**Palavras-chave:** estrutura de dados; tabela hash; encadeamento; busca sequencial; hash perfeito; análise de complexidade.

---

## 1. Introdução

A busca de um elemento em uma coleção de dados é uma das operações mais fundamentais da computação e está no núcleo de praticamente todo sistema de informação. A eficiência com que essa operação é realizada determina, em larga escala, a viabilidade de aplicações que lidam com grandes volumes de registros — de catálogos de produtos a índices de bancos de dados.

A forma mais elementar de busca é a **sequencial**: percorrer a coleção elemento a elemento até encontrar a chave desejada. Sua simplicidade tem um custo: o número de comparações cresce proporcionalmente ao tamanho da coleção, o que a torna inadequada para volumes elevados. A teoria de estruturas de dados oferece alternativas que rompem essa proporcionalidade, e entre as mais eficazes para busca por chave está a **tabela hash**, capaz de localizar um elemento em tempo médio constante, independentemente do tamanho do conjunto.

Este artigo documenta um estudo experimental em duas fases. A Fase I estabeleceu a busca sequencial como *baseline*; a Fase II implementa a tabela hash e a confronta com esse baseline sobre o mesmo dataset. A contribuição central é empírica: medir, sob protocolo controlado, o ganho real obtido ao substituir a varredura linear pela indexação por hash, e analisar criticamente em que medida a prática confirma a teoria.

---

## 2. Fundamentação Teórica

### 2.1 Estruturas de Dados e Custo de Busca

Uma estrutura de dados organiza informações de modo a otimizar determinadas operações. Para a busca por chave, o vetor não ordenado oferece inserção barata mas busca cara: na ausência de qualquer organização, não há alternativa senão examinar os elementos um a um. O custo é descrito pela notação assintótica O(n), em que n é o número de elementos: no pior caso e no caso médio de uma busca malsucedida, todos os n elementos são examinados; em uma busca bem-sucedida, examina-se em média n/2.

A notação O(·) abstrai constantes e termos de menor ordem para descrever como o custo **escala** com a entrada. É essa escala — e não o custo de uma operação isolada — que decide o comportamento de um algoritmo sob grandes volumes.

### 2.2 Tabelas Hash

Uma **tabela hash** armazena pares chave–valor em um arranjo de `m` posições, chamadas baldes (*buckets*). O endereço de cada chave é calculado por uma **função hash** `h(k)`, que mapeia a chave a um índice em `[0, m)`. Idealmente, a inserção e a busca passam a custar O(1): basta calcular `h(k)` e acessar diretamente o balde correspondente.

O obstáculo é a **colisão**: duas chaves distintas podem produzir o mesmo índice. Como o número de chaves possíveis costuma exceder `m`, colisões são em geral inevitáveis (princípio da casa dos pombos). O tratamento adotado aqui é o **encadeamento separado** (*separate chaining*): cada balde aponta para uma lista ligada que armazena todos os elementos mapeados àquele índice. A busca calcula `h(k)` e percorre a lista correspondente.

O desempenho da tabela hash é governado pelo **fator de carga** `α = n/m`. Sob hash uniforme, o comprimento médio das listas é α, e o custo médio de busca é O(1 + α). Mantendo α limitado por uma constante (tipicamente fazendo `m` proporcional a `n`), a busca permanece O(1) amortizado.

Um caso especial é o **hash perfeito**: quando a função não produz nenhuma colisão para o conjunto de chaves dado, cada balde guarda no máximo um elemento e a busca é O(1) de **pior caso**. Hashes perfeitos costumam exigir conhecimento prévio das chaves; como se verá, este trabalho obteve um, de forma fortuita, em razão da estrutura do dataset.

### 2.3 Função da Divisão

Para chaves inteiras, um dos métodos clássicos é o **resto da divisão**: `h(k) = k mod m`. É simples e rápido — uma única operação aritmética. Sua qualidade depende fortemente da escolha de `m`: recomenda-se um número primo distante de potências de 2, pois valores de `m` que sejam potências de 2 fazem a função depender apenas dos bits menos significativos da chave, e padrões periódicos nas chaves alinhados a `m` concentram colisões. A escolha de `m` e o padrão das chaves são, portanto, determinantes para que a função distribua bem.

---

## 3. Metodologia Experimental

### 3.1 Dataset

Utilizou-se um arquivo CSV com **200.003 registros** de produtos, cada um com os campos `id` (inteiro), `nome`, `categoria` e `valor`. A chave de busca é o `id`. Os identificadores cobrem exatamente o intervalo contíguo `[1, 200.003]`, são **únicos e sem duplicatas** — propriedade que, como se verá, tem efeito direto sobre a contagem de colisões.

### 3.2 Implementação

O sistema foi escrito em C, modularizado em: leitura de CSV, vetor dinâmico, busca sequencial, tabela hash e medição de tempo. O vetor dinâmico dobra de capacidade ao encher, amortizando o custo de inserção. A tabela hash usa `m = n = 200.003` baldes (tamanho igual ao volume de dados, conforme o protocolo do projeto), função `h(x) = x mod m` e encadeamento separado, contabilizando uma colisão sempre que uma inserção encontra o balde de destino já ocupado.

Para validar o contador de colisões, o código mantém uma segunda função hash selecionável (multiplicação de Knuth). Sobre o mesmo dataset, o contador registra 22.650 colisões com a multiplicação e 0 com a divisão, comprovando que o mecanismo está operacional.

### 3.3 Protocolo de Medição

Para garantir comparabilidade entre as fases e reprodutibilidade:

1. A estrutura é completamente carregada antes de qualquer medição.
2. Selecionam-se **1.000 IDs** com espaçamento uniforme, distribuídos em elementos do início (200), do meio (300), do final (400) e **inexistentes** (100, com IDs negativos).
3. O **mesmo** conjunto de IDs é submetido aos dois métodos e repetido nas três repetições.
4. Mede-se o tempo do bloco completo de 1.000 buscas com `clock()`; o tempo médio por busca é o tempo total dividido pelo número de execuções. Mede-se o bloco — e não cada busca isolada — porque uma única busca em hash, na casa dos nanossegundos, fica abaixo da resolução do relógio.
5. O protocolo é repetido **3 vezes** (3.000 execuções por método) e reporta-se a média final.
6. Registra-se o número total de colisões e a distribuição das cadeias.

O código foi compilado com otimização `-O2`.

---

## 4. Resultados da Fase I — Busca Sequencial

A busca sequencial sobre os 200.003 registros, sob o protocolo descrito, apresentou tempo médio por busca de **0,000532857 s** (≈ 533 µs) por execução. O comportamento é coerente com a complexidade O(n): buscas a elementos finais e a inexistentes percorrem grande parte ou a totalidade do vetor, ao passo que elementos iniciais são resolvidos rapidamente. A distribuição assimétrica dos IDs (mais buscas ao final do vetor) e os 100 IDs inexistentes — que forçam varredura completa — elevam o tempo médio, aproximando-o de um cenário de uso realista, distante tanto do melhor quanto do pior caso isolados.

A Fase I confirmou a principal limitação da estrutura: o tempo cresce linearmente com o volume, tornando-a inviável para sistemas de alta demanda e motivando a adoção da tabela hash.

---

## 5. Resultados da Fase II — Tabela Hash

### 5.1 Carga e Colisões

Com a função da divisão e `m = n = 200.003` (fator de carga 1,0), a carga produziu:

| Métrica | Valor |
|---------|-------|
| Total de colisões | **0** |
| Baldes ocupados | 200.003 |
| Baldes vazios | 0 |
| **Maior cadeia** | **1** |

Não houve uma única colisão: cada balde recebeu exatamente um registro.

### 5.2 Hash Perfeito: Análise da Causa

O resultado decorre da estrutura das chaves. Os IDs formam o conjunto `{1, …, 200.003}` e `m = 200.003`; logo, `x mod m` mapeia cada chave a um índice distinto (`200.003` mapeia a 0, e `1…200.002` a `1…200.002`). A função é uma **bijeção** entre chaves e baldes — um **hash perfeito** para este dataset. A busca torna-se O(1) de pior caso, pois nenhuma cadeia excede um nó.

Essa perfeição, contudo, é um **artefato da estrutura das chaves**: depende de elas serem contíguas, únicas e em quantidade igual a `m`. Um único `id` fora de `[1, m]`, ou um `m` diferente, reintroduziria colisões. Favorece a robustez o fato de `m = 200.003` ser **primo**, condição recomendada para o método da divisão.

### 5.3 Desempenho

A busca na tabela hash apresentou tempo médio por busca de **0,000000046 s** (≈ 46 ns), resumido a calcular `x mod m` e ler um único nó — efetivamente independente do tamanho do dataset.

| Repetição | Sequencial (s) | Hash (s) |
|-----------|----------------|----------|
| 1 | 0,507229 | 0,000063 |
| 2 | 0,546960 | 0,000049 |
| 3 | 0,544381 | 0,000026 |
| Total | 1,598570 | 0,000138 |

---

## 6. Comparação Formal

| Métrica | Busca Sequencial | Tabela Hash |
|---------|------------------|-------------|
| Tempo médio por busca | 0,000532857 s | 0,000000046 s |
| Complexidade teórica | O(n) | O(1) |
| Tempo total (3 × 1.000 buscas) | 1,598570 s | 0,000138 s |
| Colisões | — | 0 |
| Observações experimentais | cresce com n; sensível à posição da chave | constante; cadeia sempre unitária |

O **speedup** medido foi de aproximadamente **11.584×** em favor da tabela hash.

> Os tempos absolutos referem-se à execução do grupo e dependem do hardware; as contagens estruturais (registros, colisões, distribuição) são determinísticas.

---

## 7. Discussão Crítica

### 7.1 Teoria versus Prática

A previsão assintótica confirmou-se de forma inequívoca: a queda de O(n) para O(1) traduziu-se em um ganho de cerca de onze mil vezes. Em **número de comparações**, a razão é ainda maior — a sequencial faz em média ~100.000 comparações por busca bem-sucedida, contra **1** da hash (sem colisões), uma razão de ~100.000×. O speedup **medido** (≈ 11.584×) fica abaixo disso porque o custo de cada operação difere: a varredura sequencial percorre um vetor contíguo, muito amigável ao cache (com *prefetch*), e cada comparação custa pouquíssimo; já cada busca na hash calcula a função e salta para um balde em posição aleatória da memória, pagando provável falha de cache e perseguição de ponteiro. A hash faz muito menos trabalho, mas cada acesso seu é mais caro — e, ainda assim, vence por ordens de magnitude. É um exemplo claro de como a hierarquia de memória modula a tradução da complexidade teórica em tempo real.

### 7.2 Eficiência da Função Hash

Para este dataset, a função da divisão foi ótima — distribuição perfeita, zero colisões. A leitura crítica, porém, é essencial: essa excelência não decorre de uma virtude intrínseca do método, mas da coincidência entre o conjunto de chaves (contíguo, do tamanho exato de `m`) e a tabela. O método da divisão é sensível ao padrão das chaves e à escolha de `m`; a opção por um `m` primo (200.003) é o que sustenta sua robustez para além deste caso particular. O experimento de controle com a função da multiplicação (22.650 colisões, cadeia máxima 2) mostra o comportamento da estrutura quando a função embaralha as chaves: mesmo então, com α = 1, a busca permanece efetivamente O(1). A tabela é robusta sob ambas as funções; a da divisão apenas tira proveito máximo da regularidade das chaves.

### 7.3 Vantagens e Limitações

A tabela hash entrega busca rápida e estável, ao custo de memória adicional (o array de `m` ponteiros e um nó por registro). A perfeição obtida aqui é frágil — atrelada à estrutura das chaves — e uma função mal dimensionada poderia degradar a estrutura ao pior caso O(n). Além disso, a tabela hash não preserva ordenação nem suporta consultas por intervalo, cenários em que árvores de busca balanceadas seriam preferíveis. Para o problema em questão — localização exata por chave em grande volume — as vantagens superam amplamente as limitações.

---

## 8. Conclusão

O estudo experimental confirmou, com dados, a superioridade da tabela hash sobre a busca sequencial para localização por chave em grandes volumes: um ganho de aproximadamente 11.584× no tempo médio de busca, correspondente à transição de complexidade de O(n) para O(1). A função hash do grupo — resto da divisão simples — comportou-se como hash perfeito sobre este dataset, sem nenhuma colisão, por força da coincidência entre chaves contíguas e `m = n` primo. A análise crítica evidenciou que essa perfeição é específica do conjunto de chaves, e o experimento de controle com a função da multiplicação validou tanto o contador de colisões quanto a robustez geral da estrutura. O trabalho cumpre seu objetivo de confrontar previsão teórica e medição prática, fornecendo evidência empírica clara do papel das estruturas de dados na eficiência de sistemas que dependem de busca.

---

## Referências

- CORMEN, T. H. et al. *Algoritmos: Teoria e Prática*. 3. ed. Rio de Janeiro: Elsevier, 2012.
- KNUTH, D. E. *The Art of Computer Programming, Vol. 3: Sorting and Searching*. 2. ed. Addison-Wesley, 1998.
- ZIVIANI, N. *Projeto de Algoritmos com Implementações em Pascal e C*. 3. ed. Cengage Learning, 2011.
- SEDGEWICK, R.; WAYNE, K. *Algorithms*. 4. ed. Addison-Wesley, 2011.
