VERSION = 1.0.0
CC			= gcc
CFLAGS	= -Wall -Wextra -Werror -c -g
LDFLAGS	= -lm -lpthread
OTFLAGS = -march=native -rdynamic
DBFLAGS	= -g -D DEBUG

unit:
	mkdir -p bin
	$(CC) $(CFLAGS) $(OTFLAGS) -O2 ../String/src/string.c -o bin/string.o
	$(CC) $(CFLAGS) $(OTFLAGS) -O2 ../Time-Diff/src/time-diff.c -o bin/time-diff.o
	$(CC) $(CFLAGS) $(OTFLAGS) -O2 ../Arg-Parse/src/arg-parse.c -o bin/arg-parse.o

basic: unit
	$(CC) $(CFLAGS) $(OTFLAGS) -O2 test/basic.c -o bin/basic.o
	$(CC) $(LDFLAGS) bin/string.o bin/time-diff.o bin/arg-parse.o bin/basic.o -o bin/basic

working: unit
	$(CC) $(CFLAGS) $(OTFLAGS) -O2 test/working.c -o bin/working.o
	$(CC) $(LDFLAGS) bin/string.o bin/time-diff.o bin/arg-parse.o bin/working.o -o bin/working


signal: unit
	$(CC) $(CFLAGS) $(OTFLAGS) -O2 test/signal.c -o bin/signal.o
	$(CC) $(LDFLAGS) bin/string.o bin/time-diff.o bin/arg-parse.o bin/signal.o -o bin/signal

bevor-after: unit
	$(CC) $(CFLAGS) $(OTFLAGS) -O2 test/bevor-after.c -o bin/bevor-after.o
	$(CC) $(LDFLAGS) bin/string.o bin/time-diff.o bin/arg-parse.o bin/bevor-after.o -o bin/bevor-after

run: basic working signal bevor-after
	./bin/basic
	./bin/working
	./bin/signal
	./bin/bevor-after

				
clean:
	rm -f bin/*.o

