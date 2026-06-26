# Projeto Integrador — Fase II
## Tabela Hash com Encadeamento vs. Busca Sequencial

**Disciplina:** Estrutura de Dados
**Data:** 2º Bimestre do 1º semestre letivo de 2026

---

## Sobre o Projeto

Implementação de uma **Tabela Hash com tratamento de colisões por encadeamento separado** sobre o mesmo dataset de 200.003 produtos da Fase I, com análise experimental comparativa entre a busca sequencial (baseline da Fase I) e a busca por tabela hash.

O programa carrega o CSV, constrói o vetor dinâmico e a tabela hash, contabiliza colisões, executa o protocolo experimental (1.000 buscas × 3 repetições) para os dois métodos com os mesmos IDs e imprime a tabela comparativa.

---

## Estrutura do Projeto

```bash
fase2/
├── data/
│   └── dataset2.csv        ← dataset (ignorado pelo .gitignore)
├── src/
│   ├── main.c              ← experimento comparativo
│   ├── csv_reader.c / .h   ← leitura do CSV
│   ├── vetor.c / .h        ← vetor dinâmico (Fase I)
│   ├── busca.c / .h        ← busca sequencial (Fase I)
│   ├── hash.c / .h         ← tabela hash com encadeamento (Fase II)
│   └── tempo.c / .h        ← medição de tempo
├── .gitignore
├── Makefile
├── README.md
├── RELATORIO.md
└── ARTIGO.md               ← artigo científico final
```

---

## Como Executar

```bash
# Compila com a função da divisão (padrão, oficial do grupo) e executa
make run

# Ou separadamente
make && ./programa

# Trocar a função hash: 0 = divisão (k mod m) | 1 = multiplicação (Knuth)
make HASH=1 && ./programa     # recompila sozinho ao trocar de função

# Limpar binário e logs
make clean
```

> O arquivo `data/dataset2.csv` deve estar presente antes de executar.
> Opcionalmente: `./programa caminho/para/outro.csv`

---

## Função Hash

A função oficial do grupo é o **resto da divisão simples**: `h(x) = x mod m`, com `m` = quantidade de registros. É a função usada por padrão (`make` → `HASH=0`).

| Método | Fórmula | Colisões neste dataset |
|--------|---------|------------------------|
| **Divisão (oficial)** | `h(x) = x mod m` | **0** (IDs são exatamente 1…m → hash perfeito) |
| Multiplicação (Knuth) | `h(x) = ⌊m · frac(x·A)⌋`, `A=(√5−1)/2` | 22.650 (controle de validação) |

A divisão dá zero colisões porque os IDs formam o intervalo contíguo `[1, m]`. A multiplicação é mantida apenas como experimento de controle, para comprovar que o contador de colisões funciona.

---

## Resultados — Resumo

| Métrica | Busca Sequencial | Tabela Hash |
|---------|------------------|-------------|
| Tempo médio/busca | 0,000532857 s | 0,000000046 s |
| Tempo total (3 reps) | 1,598570 s | 0,000138 s |
| Complexidade | O(n) | O(1) |
| Colisões | — | 0 |

> A busca por tabela hash foi cerca de **11.584× mais rápida** que a sequencial, confirmando o ganho assintótico de O(n) para O(1).

O relatório técnico completo está em [`RELATORIO.md`](./RELATORIO.md) e o artigo científico em [`ARTIGO.md`](./ARTIGO.md).
