all: compile run clean

compile:
	clear
	gcc -o trab client/main.c

run:
	./trab

clean:
	rm trab

val: clean compile
	clear
	valgrind --leak-check=full ./trab 



	