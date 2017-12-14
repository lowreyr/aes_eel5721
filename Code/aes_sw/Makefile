objects = main.o galois.o aes.o

aes: $(objects)
	cc -std=c99 -o aes $(objects)

main.o: main.c
	cc -std=c99 -c main.c

galois.o: galois.c
	cc -std=c99 -c galois.c

aes.o: aes.c
	cc -std=c99 -c aes.c

clean:
	rm aes $(objects)
