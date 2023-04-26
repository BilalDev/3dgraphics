.PHONY: build

build:
	rm -rf ./build && mkdir ./build && gcc -Wall -std=c99 ./src/*.c -I /opt/homebrew/include -L /opt/homebrew/lib -lSDL2 -o ./build/renderer

run:
	./build/renderer

clean:
	rm -rf build