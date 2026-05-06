.PHONY: usage cmake ninja clean build allin
# recipes
usage:
	cat Makefile.usage.txt
cmake:
	cmake --preset default
ninja:
	cmake --build --preset default
clean:
	rm -rf build/ && git restore build/.gitignore
build:
	make cmake && make ninja
allin:
	make clean && make build
