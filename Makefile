all: main.o
	clang -o main main.o

main.o: main.c
	clang -c main.c

clean:
	rm *.o main
