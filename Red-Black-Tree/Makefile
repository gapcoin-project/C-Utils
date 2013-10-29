VERSION = 1.0.0
CC			= gcc
CFLAGS	= -Wall -Wextra -Werror -c -g -Winline
LDFLAGS	= -lm -lpthread
OTFLAGS = -march=native -O2
DBFLAGS	= -g -D DEBUG
BIN   	= ./bin
SRC   	= ./test
.PHONY: test

RED_BLACK_TREE_SRC = ./src/red-black-tree.c
RED_BLACK_TREE_OBJ = $(BIN)/red-black-tree.o

TEST_SRC = $(SRC)/basic-test.c
TEST_OBJ = $(BIN)/basic-test.o
TEST_BIN = $(BIN)/basic-test


test:
	@mkdir -p $(BIN)
	$(CC) $(CFLAGS) $(RED_BLACK_TREE_SRC) -o $(RED_BLACK_TREE_OBJ)
	$(CC) $(CFLAGS) $(TEST_SRC) -o $(TEST_OBJ)
	$(CC) $(CFLAGS) ../String/src/string.c \
				-o bin/string.o
	$(CC) $(LDFLAGS) $(TEST_OBJ) $(RED_BLACK_TREE_OBJ) bin/string.o \
				-o $(TEST_BIN)

test-iteration: test
	$(CC) $(CFLAGS) ../Time-Diff/src/time-diff.c \
				-o bin/time-diff.o
	$(CC) $(CFLAGS) test/iteration-test.c -o bin/iteration-test.o
	$(CC) $(LDFLAGS) $(RED_BLACK_TREE_OBJ) bin/iteration-test.o bin/time-diff.o \
				bin/string.o -o bin/iteration-test
				
clean:
	rm -f $(BIN)/*.o

