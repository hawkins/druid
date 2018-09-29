
all: lexer

.PHONY: clean
clean:
	rm lexer test

lexer: lexer.c
	gcc -g $^ -o $@

.PHONY: test
test: lexer.c
	gcc -g $^ -o $@ -D _TEST -g

