main: test_program.o ordered_map.o
	gcc -o test_program test_program.o ordered_map.o -L. -lrbtree

debug: test_program.o ordered_map.o
	gcc -g -o test_program test_program.o ordered_map.o -L. -lrbtree

test_program.o: test_program.c ordered_map.h
	gcc -c test_program.c

ordered_map.o: ordered_map.c ordered_map.h red_black_tree.h
	gcc -static -L. -lrbtree -c ordered_map.c

run:
	./test_program

run_debug:
	valgrind --leak-check=yes --track-origins=yes ./test_program

clean:
	rm test_program.o ordered_map.o test_program
