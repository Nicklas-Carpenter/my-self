test: hash_test hash_table_test

hash_test: hash_test.o hash.o
hash_table_test: hash_table_test.o hash_table.o hash.o self_object.o self_string.o

.PHONY: clean
clean:
	rm *.o hash_test hash_table_test
