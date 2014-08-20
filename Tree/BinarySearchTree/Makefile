main: test_program.o bin_search_tree.o
	gcc -o test_program test_program.o bin_search_tree.o

debug: test_program.o bin_search_tree.o
	gcc -o test_program test_program.o bin_search_tree.o

test_program.o: test_program.c bin_search_tree.h
	gcc -g -c test_program.c

bin_search_tree.o: bin_search_tree.c bin_search_tree.h
	gcc -g -c bin_search_tree.c

run:
	./test_program

run_debug:
	valgrind --leak-check=yes --track-origins=yes ./test_program

build_static_lib:
	gcc -c bin_search_tree.c
	ar rcs libbstree.a bin_search_tree.o

clean:
	rm test_program.o bin_search_tree.o test_program
