CC = gcc -g
OBJ = hello_world.o compteur_seq.o fib_seq.o

all : hello_world compteur_seq fib_seq

hello_world:$(OBJ)
	$(CC) -o hello_world hello_world.c -pthread
compteur_seq:compteur_seq.c
	$(CC) -o compteur_seq compteur_seq.c -pthread
fib_seq:fib_seq.c
	$(CC) -o fib_seq fib_seq.c -pthread

clean:
	rm $(OBJ)