all: main

main: src/main.c
	gcc src/main.c -o build/main -lncurses

run: main
	./build/main

clean:
	rm -f build/main
