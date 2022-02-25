all: compile run clean

compile:
	clear
	gcc -o trab client/main.c client/commandLine.c

run:
	./trab

clean:
	rm trab

val: clean compile
	clear
	valgrind --leak-check=full ./trab 

comp-ext:
	gcc -o prog1 prog1.c

	