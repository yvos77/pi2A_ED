<h1>🔎 Projeto Integrador 2A — Estrutura de Dados</h1>

<p>
  <strong>Análise de Desempenho de Algoritmos de Busca em C</strong><br>
  Da busca sequencial à Tabela Hash — teoria e experimento sobre 200.003 registros.
</p>

---

## 📖 Sobre o Projeto

Este repositório reúne as **duas fases** do Projeto Integrador da disciplina de **Estrutura de Dados** (IESB). O objetivo é estudar, na prática, como diferentes estruturas afetam o desempenho da operação mais fundamental da computação: **a busca por chave**.

Partimos de um baseline simples — a **busca sequencial**, O(n) — e medimos o ganho obtido ao substituí-la por uma **Tabela Hash com encadeamento**, O(1) amortizado. Tudo sobre um mesmo dataset real de **200.003 produtos**, sob um protocolo experimental controlado.

---

## 🗂️ Estrutura do Repositório

```bash
pi2A_ED/
├── fase1/          → Busca Sequencial (baseline O(n))
├── fase2/          → Tabela Hash com encadeamento + análise comparativa
│   ├── src/        → código-fonte em C
│   ├── RELATORIO.md
│   └── ARTIGO.md   → artigo científico final
└── README.md       → você está aqui
```

---

## 🧩 As Duas Fases

| | **Fase I — Busca Sequencial** | **Fase II — Tabela Hash** |
|---|---|---|
| **Estrutura** | Vetor dinâmico | Tabela hash com encadeamento separado |
| **Algoritmo** | Varredura linear | `h(x) = x mod m` + lista no balde |
| **Complexidade** | O(n) | O(1) |
| **Papel** | Baseline de referência | Estrutura otimizada |
| 📁 | [`/fase1`](./fase1) | [`/fase2`](./fase2) |

A **Fase I** estabeleceu o ponto de comparação. A **Fase II** implementou a Tabela Hash, contabilizou colisões e mediu experimentalmente o ganho de desempenho, fechando com um artigo científico completo.

---

## 📊 Resultados em Destaque

Experimento: **1.000 buscas × 3 repetições** (mesmos IDs), distribuídas entre início, meio, final e inexistentes do dataset.

| Métrica | 🐢 Busca Sequencial | ⚡ Tabela Hash |
|---|---|---|
| Tempo médio por busca | 0,000532857 s | 0,000000046 s |
| Tempo total (3 reps) | 1,598570 s | 0,000138 s |
| Complexidade | O(n) | O(1) |
| Colisões | — | **0** |

<p align="center">
  <img src="https://img.shields.io/badge/Speedup-~11.584×_mais_rápida-success?style=for-the-badge" alt="Speedup">
</p>

> 💡 **Curiosidade:** como os IDs do dataset são exatamente `1…200.003` e o tamanho da tabela é `m = 200.003` (número primo), a função da divisão virou um **hash perfeito** — zero colisões, cada balde com um único elemento.

---

## ▶️ Como Executar

Cada fase é independente. Para a **Fase II**:

```bash
cd fase2

# coloque o dataset em data/dataset2.csv

make            # compila (função da divisão, padrão)
./programa      # roda o experimento comparativo

make HASH=1 && ./programa   # roda com a função da multiplicação (controle)
make clean                  # limpa binário e logs
```

> Requisitos: `gcc` e `make` (ambiente Linux). Alternativa sem make: `gcc -std=c11 src/*.c -o programa`.

---

## 🛠️ Tecnologias

- **Linguagem:** C (padrão C11)
- **Build:** GNU Make
- **Medição:** `clock()` da `<time.h>`
- **Controle de versão:** Git + GitHub Projects (Kanban)

---

## 📄 Documentação

- 📘 [Relatório Técnico (Fase II)](./fase2/RELATORIO.md) — metodologia, resultados e análise
- 📰 [Artigo Científico (Fase II)](./fase2/ARTIGO.md) — fundamentação teórica e comparação formal

---

## 👥 Equipe

<table align="center">
  <tr>
    <td align="center"><a href="https://github.com/yvos77"><b>Yuri Victor</b><br><sub>@yvos77</sub></a><br><sub>Experimento & Build</sub></td>
    <td align="center"><a href="https://github.com/f3lipe31"><b>Felipe O. Cunha</b><br><sub>@f3lipe31</sub></a><br><sub>Núcleo da Tabela Hash</sub></td>
    <td align="center"><a href="https://github.com/noonlyy"><b>Gabriel</b><br><sub>@noonlyy</sub></a><br><sub>Análise & Artigo</sub></td>
  </tr>
</table>

<p align="center"><sub>Projeto Integrador — Estrutura de Dados · IESB · 2026</sub></p>
