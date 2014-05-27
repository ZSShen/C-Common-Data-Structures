main: test_program.o red_black_tree.o
	gcc -o test_program test_program.o red_black_tree.o

debug: test_program.o red_black_tree.o
	gcc -g -o test_program test_program.o red_black_tree.o

test_program.o: test_program.c red_black_tree.h
	gcc -c test_program.c

red_black_tree.o: red_black_tree.c red_black_tree.h
	gcc -c red_black_tree.c

run:
	./test_program

run_debug:
	valgrind --leak-check=yes --track-origins=yes ./test_program

build_static_lib:
	gcc -c red_black_tree.c
	ar rcs librbtree.a red_black_tree.o

clean:
	rm test_program.o red_black_tree.o test_program
