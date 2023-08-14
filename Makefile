.PHONY: build

make build:
	./pre_build.sh && gcc -pedantic-errors -Wall -Wextra -Wsign-conversion -std=c99 -o ./build/dont_trust ./build/*.c
run:
	./build/dont_trust
clean:
	rm -R build
