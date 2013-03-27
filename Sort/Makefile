VERSION = 1.0.0
CC			= gcc
CFLAGS	= -Wall -Wextra -Werror -c -g
LDFLAGS	= -lm -lpthread
OTFLAGS = -march=native -O3
DBFLAGS	= -g -D DEBUG
BIN   	= ./bin
SRC   	= ./test

SORT_SRC = ./src/sort.c
SORT_OBJ = $(BIN)/sort.o

BASIC_TEST_SRC = $(SRC)/basic-test.c
BASIC_TEST_OBJ = $(BIN)/basic-test.o
BASIC_TEST_BIN = $(BIN)/basic-test

SPEED_MACRO_VS_PTR_TEST_SRC = $(SRC)/speed-macro-vs-pointer.c
SPEED_MACRO_VS_PTR_TEST_OBJ = $(BIN)/speed-macro-vs-pointer.o
SPEED_MACRO_VS_PTR_TEST_BIN = $(BIN)/speed-macro-vs-pointer

basic-test:
	mkdir -p bin
	$(CC) $(CFLAGS) $(OTFLAGS) $(SORT_SRC)	-o $(SORT_OBJ)
	$(CC) $(CFLAGS) $(OTFLAGS) $(BASIC_TEST_SRC)	-o $(BASIC_TEST_OBJ)
	$(CC) $(LDFLAGS) $(BASIC_TEST_OBJ) $(SORT_OBJ) -o $(BASIC_TEST_BIN)

speed-macro-vs-pointer:
	make -C ../Time-Diff/	
	mkdir -p bin
	$(CC) $(CFLAGS) $(OTFLAGS) $(SORT_SRC)	-o $(SORT_OBJ)
	$(CC) $(CFLAGS) $(OTFLAGS) $(SPEED_MACRO_VS_PTR_TEST_SRC)	\
				-o $(SPEED_MACRO_VS_PTR_TEST_OBJ)
	$(CC) $(LDFLAGS) $(SPEED_MACRO_VS_PTR_TEST_OBJ) $(SORT_OBJ) 	\
				../Time-Diff/bin/time-diff.o 														\
				-o $(SPEED_MACRO_VS_PTR_TEST_BIN) 
				
clean:
	rm $(BIN)/*.o

