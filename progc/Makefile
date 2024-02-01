CC = gcc

CFLAGS = -Wall 

T1 = optt
T2 = opts

$(T1) : optt.o
	$(CC) $(CFLAGS) -o $@ $^

$(T2) : opts.o
	$(CC) $(CFLAGS) -o $@ $^

optt.o : opt_t.c opt_t.h
	$(CC) $(CFLAGS) -c -o $@ $<

opts.o : opt_s.c opt_s.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f optt.o opts.o $(T1) $(T2)