VERSION = 1.0.0
CC			= gcc
OTFLAGS = -march=native -O2
DBFLAGS	= -g -D DEBUG
CFLAGS	= -Wall -Wextra -Werror -c -Winline $(DBFLAGS)
LDFLAGS	=  -lpthread -lm

prepare:
	@mkdir -p bin

speed-test: prepare
	$(CC) $(CFLAGS) $(OTFLAGS) ../Time-Diff/src/time-diff.c -o bin/time-diff.o
	$(CC) $(CFLAGS) $(OTFLAGS) ../Thread-Clients/src/thread-client.c -o bin/tc.o
	$(CC) $(CFLAGS) $(OTFLAGS) test/speed-test.c -o bin/speed-test.o
	$(CC) $(LDFLAGS) bin/time-diff.o bin/tc.o bin/speed-test.o -o bin/speed-test

clean:
	@rm -rf $(BIN)
