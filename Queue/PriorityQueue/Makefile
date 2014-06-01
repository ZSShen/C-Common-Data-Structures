main: test_program.o priority_queue.o
	gcc -o test_program test_program.o priority_queue.o -L. -ldarray

debug: test_program.o priority_queue.o
	gcc -g -o test_program test_program.o priority_queue.o -L. -ldarray

test_program.o: test_program.c priority_queue.h
	gcc -c test_program.c

priority_queue.o: priority_queue.c priority_queue.h dynamic_array.h
	gcc -static -L. -ldarray -c priority_queue.c

run:
	./test_program

run_debug:
	valgrind --leak-check=yes --track-origins=yes ./test_program

clean:
	rm test_program.o priority_queue.o test_program
