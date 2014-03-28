VERSION = 1.0.0
CC			= gcc
CFLAGS	= -Wall -Wextra -Werror -c -g -march=native -O2
LDFLAGS	= -lm -lpthread
OTFLAGS = -march=native -rdynamic
DBFLAGS	= -g -D DEBUG
.PHONY: all test clean

test:
	mkdir -p bin
	$(CC) $(CFLAGS) src/string.c -o src/string.o
	$(CC) $(CFLAGS) test/base64.c -o test/base64.o
	$(CC) $(CFLAGS) ../Time-Diff/src/time-diff.c -o bin/time-diff.o
	$(CC) $(LDFLAGS) src/string.o test/base64.o bin/time-diff.o -o bin/base64
	./bin/base64

				
clean:
	rm -f bin/*.o

