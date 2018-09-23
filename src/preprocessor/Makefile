
all: preprocessor

clean:
	$(RM) preprocessor

preprocessor: preprocessor.c preprocessor.h
	$(CC) $< -o $@

test: preprocessor
	./preprocessor hello.druid
	diff hello.druid_l hello.druid ; cp hello.druid_l hello.druid

