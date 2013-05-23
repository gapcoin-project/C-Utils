VERSION = 1.0.0
CC			= gcc
CFLAGS	= -Wall -Wextra -Werror -c -g 
LDFLAGS	= -lrt
OTFLAGS = -march=native
DBFLAGS	= -g -D DEBUG
BIN   	= ./bin
SRC   	= ./test

TIMEOUT_SRC = ./src/timeout.c
TIMEOUT_OBJ = $(BIN)/timeout.o

TIMEDIFF_SRC = ../Time-Diff/src/time-diff.c
TIMEDIFF_OBJ = $(BIN)/time-diff.o

TEST_SRC = $(SRC)/basic-test.c
TEST_OBJ = $(BIN)/basic-test.o
TEST_BIN = $(BIN)/basic-test

timeout: 
	@mkdir -p $(BIN)
	$(CC) $(CFLAGS) $(DBFLAGS) $(TIMEOUT_SRC)	-o $(TIMEOUT_OBJ)

time-diff: 
	@mkdir -p $(BIN)
	$(CC) $(CFLAGS) $(DBFLAGS) $(TIMEDIFF_SRC)	-o $(TIMEDIFF_OBJ)

Test: timeout time-diff
	$(CC) $(CFLAGS) $(DBFLAGS) $(TEST_SRC)	-o $(TEST_OBJ)
	$(CC) $(LDFLAGS) $(TEST_OBJ) $(TIMEOUT_OBJ) $(TIMEDIFF_OBJ) -o $(TEST_BIN)

				
clean:
	rm $(BIN)/*.o

