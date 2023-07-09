all: self test
test: hash_test hash_table_test self_vm_code_test codegen_test

self: self.o lexer.o parser.o codegen.o self_vm.o error.o debug.o
hash_test: hash_test.o hash.o
hash_table_test: hash_table_test.o hash_table.o hash.o self_object.o self_string.o
self_vm_code_test: self_vm_code_test.o self_vm.o debug.o
codegen_test: codegen_test.o lexer.o parser.o self_vm.o codegen.o error.o debug.o

.PHONY: clean
clean:
	rm *.o hash_test hash_table_test self
