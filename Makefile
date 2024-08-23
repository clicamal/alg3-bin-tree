CC=gcc -o main main.c

run: compile
	./main

compile:
	$(CC)

compile-debug:
	$(CC) -g -Wall
