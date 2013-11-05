VERSION = 1.0.0
CC			= gcc
OTFLAGS = -march=native -O2
DBFLAGS	= -g -D DEBUG
CFLAGS	= -Wall -Wextra -Werror -c -Winline $(OTFLAGS) $(DBFLAGS)
LDFLAGS	=  -lpthread -lm
BIN   	= ./bin
SRC			= ./src
TEST	  = ./test

#
# +----------------+
# | Thread Clients |
# +----------------+
#
THREAD_CLIENTS_SRC = $(SRC)/thread-clients.c
THREAD_CLIENTS_OBJ = $(BIN)/thread-clients.o

#
# +---------------+
# | Thread Client |
# +---------------+
#
THREAD_CLIENT_SRC = $(SRC)/thread-client.c
THREAD_CLIENT_OBJ = $(BIN)/thread-client.o

#
# +------------+
# | Speed Test |
# +------------+
#
SPEED_TEST_SRC = $(TEST)/thread-client-speed-test.c
SPEED_TEST_OBJ = $(BIN)/thread-client-speed-test.o
SPEED_TEST_BIN = $(BIN)/thread-client-speed-test


thread-clients: 
	mkdir -p bin
	$(CC) $(CFLAGS) $(THREAD_CLIENTS_SRC) -o $(THREAD_CLIENTS_OBJ)

thread-client: 
	mkdir -p bin
	$(CC) $(CFLAGS) $(THREAD_CLIENT_SRC) -o $(THREAD_CLIENT_OBJ)

speed-test: thread-client
	make -C ../Time-Diff
	$(CC) $(CFLAGS) $(SPEED_TEST_SRC) -o $(SPEED_TEST_OBJ)
	$(CC) $(LDFLAGS) $(THREAD_CLIENT_OBJ) $(SPEED_TEST_OBJ) \
				../Time-Diff/bin/time-diff.o -o $(SPEED_TEST_BIN)

clean:
	@rm -rf $(BIN)
