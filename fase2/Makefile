CC      = gcc
CFLAGS  = -O2 -Wall -Wextra -std=c11
SRC     = src/main.c src/csv_reader.c src/vetor.c src/busca.c src/hash.c src/tempo.c
OUT     = programa

# Funcao hash: 0 = divisao (k mod m) | 1 = multiplicacao (Knuth)
# A funcao oficial do grupo e a DIVISAO (0). Sobrescreva: make HASH=1
HASH   ?= 0

all: $(OUT)

# Recompila quando o codigo mudar OU quando o valor de HASH mudar
$(OUT): $(SRC) .hash_atual
	$(CC) $(CFLAGS) -DHASH_METODO=$(HASH) $(SRC) -o $(OUT) -lm

# Registra o ultimo HASH usado; so altera o arquivo quando HASH muda,
# o que forca o rebuild acima sem precisar de make clean
.hash_atual: FORCE
	@[ "`cat $@ 2>/dev/null`" = "$(HASH)" ] || echo "$(HASH)" > $@

FORCE:

run: all
	./$(OUT)

clean:
	rm -f $(OUT) resultados.txt resultados_hash.txt .hash_atual

.PHONY: all run clean FORCE