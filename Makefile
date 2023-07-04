test: hash_test

hash_test: hash_test.o hash.o

.PHONY: clean
clean:
	rm *.o hash_test
