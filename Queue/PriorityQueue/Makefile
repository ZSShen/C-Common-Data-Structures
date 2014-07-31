main: test_program.o priority_queue.o
	gcc -o test_program test_program.o priority_queue.o -L. -ldarray

debug: test_program.o priority_queue.o
	gcc -g -ldl -o test_program test_program.o priority_queue.o 

test_program.o: test_program.c priority_queue.h
	gcc -c test_program.c

priority_queue.o: priority_queue.c priority_queue.h heap.h
	gcc -L. -ldl -c priority_queue.c

run:
	./test_program

run_debug:
	valgrind --leak-check=yes --track-origins=yes ./test_program

build_static_lib:
	gcc -L. -ldarray -c priority_queue.c
	ar rcs libpqueue.a priority_queue.o

clean:
	rm test_program.o priority_queue.o test_program
