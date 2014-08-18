main: normal_test normal_queue
	gcc -o test_program test_program.o priority_queue.o -ldl

debug: debug_test debug_queue
	gcc -g -o test_program test_program.o priority_queue.o -ldl 

perf: perf_test normal_queue
	gcc -o test_program test_program.o priority_queue.o -ldl

normal_test: test_program.c priority_queue.h
	gcc -c test_program.c -ldl

debug_test: test_program.c priority_queue.h
	gcc -g -c test_program.c -ldl

perf_test: test_program.c priority_queue.h
	gcc -DELAPSE -c test_program.c

normal_queue: priority_queue.c priority_queue.h heap.h
	gcc -L. -c priority_queue.c -ldl 

debug_queue: priority_queue.c priority_queue.h heap.h
	gcc -L. -g -c priority_queue.c -ldl 

run:
	./test_program -i $(IMPLEMENT)

run_debug:
	valgrind --leak-check=yes --track-origins=yes ./test_program -i $(IMPLEMENT)

clean:
	rm test_program.o priority_queue.o test_program
