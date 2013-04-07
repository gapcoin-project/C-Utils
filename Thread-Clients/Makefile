VERSION = 1.0.0
CC			= gcc
OTFLAGS = -march=native -O2
DBFLAGS	= -g -D DEBUG
CFLAGS	= -Wall -Wextra -Werror -c $(OTFLAGS) $(DBFLAGS)
LDFLAGS	= 
BIN   	= ./bin
SRC			= ./src

#
# +----------------+
# | Thread Clients |
# +----------------+
#
THREAD_CLIENTS_SRC = $(SRC)/thread-clients.c
THREAD_CLIENTS_OBJ = $(BIN)/thread-clients.o


thread-clients: LDFLAGS += -lpthread -lm
thread-clients: 
	mkdir -p bin
	$(CC) $(CFLAGS) $(THREAD_CLIENTS_SRC) -o $(THREAD_CLIENTS_OBJ)

