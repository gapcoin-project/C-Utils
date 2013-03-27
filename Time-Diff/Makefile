VERSION = 1.0.0
CC			= gcc
CFLAGS	= -Wall -Wextra -Werror -c -g
LDFLAGS	= -lm -lpthread
OTFLAGS = -march=native
DBFLAGS	= -g -D DEBUG
BIN   	= ./bin
SRC   	= ./src


TIME_DIFF_SRC = $(SRC)/time-diff.c
TIME_DIFF_OBJ = $(BIN)/time-diff.o

TEST_SRC = ./test/basic-test.c
TEST_OBJ = $(BIN)/basic-test.o
TEST_BIN = $(BIN)/basic-test


time-diff:
	mkdir -p bin
	$(CC) $(CFLAGS) $(OTFLAGS) -O2 $(TIME_DIFF_SRC)	-o $(TIME_DIFF_OBJ)

test: time-diff
	$(CC) $(CFLAGS) $(OTFLAGS) $(TEST_SRC) -o $(TEST_OBJ)
	$(CC) $(LDFLAGS) $(TEST_OBJ) $(TIME_DIFF_OBJ) -o $(TEST_BIN)

				
clean:
	rm $(BIN)/*.o

