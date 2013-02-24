VERSION = 1.0.0
CC			= gcc
CFLAGS	= -Wall -Wextra -Werror -c -g
LDFLAGS	= -lm -lpthread
OTFLAGS = -march=native
DBFLAGS	= -g -D DEBUG
BIN   	= ./bin
SRC   	= ./test


TEST_SRC = $(SRC)/basic-test.c
TEST_OBJ = $(BIN)/basic-test.o
TEST_BIN = $(BIN)/basic-test


Test:
	$(CC) $(CFLAGS) $(OTFLAGS) -O2 $(TEST_SRC)	-o $(TEST_OBJ)
	$(CC) $(LDFLAGS) $(TEST_OBJ) -o $(TEST_BIN)
				
clean:
	rm $(BIN)/*.o

