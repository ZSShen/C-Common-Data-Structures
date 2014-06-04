main: test_program.o singly_linked_list.o
	gcc -o test_program test_program.o singly_linked_list.o

debug: test_program.o singly_linked_list.o
	gcc -g -o test_program test_program.o singly_linked_list.o

test_program.o: test_program.c singly_linked_list.h
	gcc -c test_program.c

singly_linked_list.o: singly_linked_list.c singly_linked_list.h
	gcc -c singly_linked_list.c

run:
	./test_program

run_debug:
	valgrind --leak-check=yes --track-origins=yes ./test_program

build_static_lib:
	gcc -c singly_linked_list.c
	ar rcs libsllist.a singly_linked_list.o

clean:
	rm test_program.o singly_linked_list.o test_program
