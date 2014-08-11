main: test_program.o binomial_heap.o
	gcc -o test_program test_program.o binomial_heap.o

debug: test_program.o binomial_heap.o
	gcc -g -o test_program test_program.o binomial_heap.o

test_program.o: test_program.c binomial_heap.h
	gcc -c test_program.c

binomial_heap.o: binomial_heap.c binomial_heap.h
	gcc -c binomial_heap.c

run:
	./test_program

run_debug:
	valgrind --leak-check=yes --track-origins=yes ./test_program

build_static_lib:
	gcc -c binomial_heap.c
	ar rcs libbinomial_heap.a binomial_heap.o

build_dynamic_lib:
	gcc -DBUILD_PLUGIN -c binomial_heap.c
	gcc binomial_heap.o -fPIC -shared -o libbinomial_heap.so

clean:
	rm test_program.o binomial_heap.o test_program
