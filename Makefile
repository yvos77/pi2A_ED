CC = gcc
CFLAGS = -Wall -Wextra -std=c99
SRC = src/main.c src/csv_reader.c src/vetor.c src/busca.c src/tempo.c
OUT = programa

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

clean:
	rm -f $(OUT)