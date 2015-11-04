# Main target

.PHONY: test

test: test.c
	gcc test.c -o test -lpthread -O3
