
all: preprocessor

clean:
	$(RM) preprocessor

preprocessor: main.c
	$(CC) $< -o $@

test: preprocessor
	./preprocessor hello.druid
	diff hello.druid_l hello.druid ; cp hello.druid_l hello.druid

