run: build
	./gentity

build: ./gentity.c
	gcc ./gentity.c -ggdb3 -Wall -g -o gentity

valgrind: run
	valgrind --leak-check=full --show-leak-kinds=all  ./gentity

