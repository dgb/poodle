CC=clang
LD=clang++

all: poodle

poodle: poodle.o
		$(LD) $< $(LDFLAGS) -o $@

poodle.o: poodle.c
		$(CC) $(CFLAGS) -c $<

clean:
		-rm -f poodle.o poodle
