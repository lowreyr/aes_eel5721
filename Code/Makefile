objects = main.o galois.o aes.o

aes: $(objects)
	cc -o aes $(objects)

main.o: main.c
	cc -c main.c

galois.o: galois.c
	cc -c galois.c

aes.o: aes.c
	cc -c aes.c

clean:
	rm aes $(objects)
