CC      = gcc
CFLAGS  = -O2 -Wall -Wextra -std=c11
SRC     = src/main.c src/csv_reader.c src/vetor.c src/busca.c src/hash.c src/tempo.c
OUT     = programa

# Funcao hash: 0 = divisao (k mod m) | 1 = multiplicacao (Knuth)
# A funcao oficial do grupo e a DIVISAO (0). Sobrescreva: make HASH=1
HASH   ?= 0

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) -DHASH_METODO=$(HASH) $(SRC) -o $(OUT) -lm

run: all
	./$(OUT)

clean:
	rm -f $(OUT) resultados.txt resultados_hash.txt

.PHONY: all run clean