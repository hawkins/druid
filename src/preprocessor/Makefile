
all: preprocessor

clean:
	$(RM) preprocessor

preprocessor: main.c
	$(CC) $< -o $@

test: preprocessor
	./preprocessor hello.druid

