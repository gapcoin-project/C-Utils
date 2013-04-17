VERSION = 1.0.0
CC			= gcc
CFLAGS	= -Wall -Wextra -Werror -c -g
LDFLAGS	= -lm -lpthread
OTFLAGS = -march=native
DBFLAGS	= -g -D DEBUG
BIN   	= ./bin
SRC   	= ./test

BMP_SRC = ./src/bmp.c
BMP_OBJ = $(BIN)/bmp.o

TEST_SRC = $(SRC)/basic-test.c
TEST_OBJ = $(BIN)/basic-test.o
TEST_BIN = $(BIN)/basic-test

TO_BMP_TEST_SRC = $(SRC)/to-bmp.c
TO_BMP_TEST_OBJ = $(BIN)/to-bmp.o
TO_BMP_TEST_BIN = $(BIN)/to-bmp


Test:
	$(CC) $(CFLAGS) $(OTFLAGS) -O2 $(BMP_SRC)	-o $(BMP_OBJ)
	$(CC) $(CFLAGS) $(OTFLAGS) -O2 $(TEST_SRC)	-o $(TEST_OBJ)
	$(CC) $(LDFLAGS) $(TEST_OBJ) $(BMP_OBJ) -o $(TEST_BIN)

To-BMP-Test:
	$(CC) $(CFLAGS) $(OTFLAGS) -O2 $(BMP_SRC)	-o $(BMP_OBJ)
	$(CC) $(CFLAGS) $(OTFLAGS) -O2 $(TO_BMP_TEST_SRC)	-o $(TO_BMP_TEST_OBJ)
	$(CC) $(LDFLAGS) $(TO_BMP_TEST_OBJ) $(BMP_OBJ) -o $(TO_BMP_TEST_BIN)
				
clean:
	rm $(BIN)/*.o

