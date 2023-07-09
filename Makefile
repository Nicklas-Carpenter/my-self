all: self test
test: hash_test hash_table_test self_vm_code_test

self: self.o lexer.o parser.o error.o debug.o
hash_test: hash_test.o hash.o
hash_table_test: hash_table_test.o hash_table.o hash.o self_object.o self_string.o
self_vm_code_test: self_vm_code_test.o self_vm.o debug.o

.PHONY: clean
clean:
	rm *.o hash_test hash_table_test self
