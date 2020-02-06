CC = gcc
OBJ = Exo1.o sousPref.o sousPrefecture.o sommeTab.o

all : Exo1 sousPref sousPrefecture sommeTab

Exo1:$(OBJ)
	$(CC) -o Exo1 Exo1.c -pthread
sousPref:$(OBJ)
	$(CC) -o sousPref sousPref.c -pthread
sousPrefecture:$(OBJ)
	$(CC) -o sousPrefecture sousPrefecture.c -pthread
sommeTab:$(OBJ)
	$(CC) -o sommeTab sommeTab.c -pthread

clean:
	rm $(OBJ)