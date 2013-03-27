VERSION = 1.0.0
CC			= gcc
CFLAGS	= -Wall -Wextra -Werror -c -g
LDFLAGS	= -lm -lpthread
OTFLAGS = -march=native
DBFLAGS	= -g -D DEBUG
BIN   	= ./bin
SRC   	= ./test


TEST1_SRC = $(SRC)/basic-test.c
TEST1_OBJ = $(BIN)/basic-test.o
TEST1_BIN = $(BIN)/basic-test

TEST2_SRC = $(SRC)/path-calculation.c
TEST2_OBJ = $(BIN)/path-calculation.o
TEST2_BIN = $(BIN)/path-calculation

Test:
	$(CC) $(CFLAGS) $(OTFLAGS) -O2 $(TEST1_SRC)	-o $(TEST1_OBJ)
	$(CC) $(LDFLAGS) $(TEST1_OBJ) -o $(TEST1_BIN)
	$(CC) $(CFLAGS) $(OTFLAGS) -O2 $(TEST2_SRC)	-o $(TEST2_OBJ)
	$(CC) $(LDFLAGS) $(TEST2_OBJ) -o $(TEST2_BIN)
				
clean:
	rm $(BIN)/*.o

