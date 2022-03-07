# Iago de Sousa Cerqueira
# José Jorge Moutinho Uliana
# Mirelly Micaella

all: clean compile run 

compile:
	clear
	gcc -o trab client/*.c

run:
	./trab

clean:
	rm trab

val: clean compile
	clear
	valgrind --leak-check=full ./trab 
