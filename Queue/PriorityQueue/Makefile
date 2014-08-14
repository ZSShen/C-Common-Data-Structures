main: test_program.o priority_queue.o
	gcc -o test_program test_program.o priority_queue.o -ldl

debug: test_program.o priority_queue.o
	gcc -g -o test_program test_program.o priority_queue.o -ldl 

test_program.o: test_program.c priority_queue.h
	gcc -c test_program.c -ldl

priority_queue.o: priority_queue.c priority_queue.h heap.h
	gcc -L. -c priority_queue.c -ldl

run:
	./test_program -i $(IMPLEMENT)

run_debug:
	valgrind --leak-check=yes --track-origins=yes ./test_program -i $(IMPLEMENT)

clean:
	rm test_program.o priority_queue.o test_program
