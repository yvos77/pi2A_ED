# Projeto Integrador — Fase I
## Análise de Desempenho da Busca Sequencial

**Disciplina:** Estrutura de Dados  
**Data:** 1º Bimestre do 1º semestre letivo de 2026

---

## 📋 Sobre o Projeto

Implementação e análise experimental da busca sequencial sobre um vetor dinâmico de produtos, carregado a partir de um arquivo CSV com 200.003 registros.

---

## 📁 Estrutura do Projeto

```bash
projeto/
├── data/
│   └── dataset2.csv        ← dataset (ignorado pelo .gitignore)
├── src/
│   ├── main.c
│   ├── csv_reader.c / .h   ← leitura do CSV
│   ├── vetor.c / .h        ← vetor dinâmico
│   ├── busca.c / .h        ← busca sequencial
│   └── tempo.c / .h        ← medição de tempo
├── .gitignore
├── Makefile
├── README.md
└── RELATORIO.md            ← relatório técnico completo
```

---

## 🚀 Como Executar

```bash
# Compilar e executar
make && ./programa

# Limpar
make clean
```

> ⚠️ O arquivo `data/dataset2.csv` deve estar presente antes de executar.

---

## 📊 Resultados — Resumo

> O tempo de busca cresce linearmente com o tamanho do vetor, confirmando a complexidade **O(n)** da busca sequencial.

---

## 📄 Relatório Técnico

O relatório técnico completo está disponível em [`RELATORIO.md`](./RELATORIO.md).
