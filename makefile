# Makefile para o projeto de ranking de algoritmos de ordenação

CC      = gcc
CFLAGS  = -Wall -Wextra -O2 -std=c11 -I include
LDFLAGS = -lm

SRC_DIR = src
OBJ_DIR = obj
BIN     = ranking

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

.PHONY: all clean benchmark demo

all: $(OBJ_DIR) $(BIN)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

demo: all
	./$(BIN)

benchmark: all
	./$(BIN) --benchmark 100 > data/resultados.csv
	@echo "CSV salvo em data/resultados.csv"

clean:
	rm -rf $(OBJ_DIR) $(BIN) data/resultados.csv