main: test_program.o binary_heap.o
	gcc -o test_program test_program.o binary_heap.o -L. -ldarray

debug: test_program.o binary_heap.o
	gcc -g -o test_program test_program.o binary_heap.o -L. -ldarray

test_program.o: test_program.c binary_heap.h
	gcc -c test_program.c

binary_heap.o: binary_heap.c binary_heap.h dynamic_array.h
	gcc -static -L. -ldarray -c binary_heap.c

run:
	./test_program

run_debug:
	valgrind --leak-check=yes --track-origins=yes ./test_program

build_static_lib:
	gcc -L. -ldarray -c binary_heap.c
	ar rcs libpqueue.a binary_heap.o

build_dynamic_lib:
	gcc -L. -ldarray -c binary_heap.c
	gcc binary_heap.o -fPIC -shared -o libbinary_heap.so

clean:
	rm test_program.o binary_heap.o test_program
