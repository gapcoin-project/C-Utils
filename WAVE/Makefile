VERSION = 1.0.0
CC			= gcc
CFLAGS	= -Wall -Wextra -Werror -c -g
LDFLAGS	= -lm -lpthread
OTFLAGS = -march=native
DBFLAGS	= -g -D DEBUG
BIN   	= ./bin
SRC   	= ./test

WAVE_SRC = ./src/wave.c
WAVE_OBJ = $(BIN)/wave.o

TEST_SRC = $(SRC)/basic-test.c
TEST_OBJ = $(BIN)/basic-test.o
TEST_BIN = $(BIN)/basic-test

TO_WAVE_TEST_SRC = $(SRC)/to-wave.c
TO_WAVE_TEST_OBJ = $(BIN)/to-wave.o
TO_WAVE_TEST_BIN = $(BIN)/to-wave


Test:
	mkdir -p ./bin
	$(CC) $(CFLAGS) $(OTFLAGS) -O2 $(WAVE_SRC)	-o $(WAVE_OBJ)
	$(CC) $(CFLAGS) $(OTFLAGS) -O2 $(TEST_SRC)	-o $(TEST_OBJ)
	$(CC) $(LDFLAGS) $(TEST_OBJ) $(WAVE_OBJ) -o $(TEST_BIN)

To-Wave-Test:
	mkdir -p ./bin
	$(CC) $(CFLAGS) $(OTFLAGS) -O2 $(WAVE_SRC)	-o $(WAVE_OBJ)
	$(CC) $(CFLAGS) $(OTFLAGS) -O2 $(TO_WAVE_TEST_SRC)	-o $(TO_WAVE_TEST_OBJ)
	$(CC) $(LDFLAGS) $(TO_WAVE_TEST_OBJ) $(WAVE_OBJ) -o $(TO_WAVE_TEST_BIN)
				
clean:
	rm $(BIN)/*.o

