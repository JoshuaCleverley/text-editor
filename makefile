all: main

main: src/main.c
	gcc -Wall -Wextra src/main.c -o build/main -lncurses

run: main
	./build/main

clean:
	rm -f build/main
