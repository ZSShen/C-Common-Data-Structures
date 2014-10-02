
NAME_OUT := test_program

# List the dependencies for project building.
DEPENDENCY := bin_search_tree test_program

# Specify the compilation options.
CC := gcc
FLAG :=
ifeq ($(DEBUG), true)
	FLAG := $(FLAG) -g
endif

executable: $(DEPENDENCY)
	$(CC) $(FLAG) -o $(NAME_OUT) *.o

$(DEPENDENCY):
	$(CC) $(FLAG) -c $@.c -o $@.o


#debug: test_program.o bin_search_tree.o
#	gcc -o test_program test_program.o bin_search_tree.o

#test_program.o: test_program.c bin_search_tree.h
#	gcc -g -c test_program.c

#bin_search_tree.o: bin_search_tree.c bin_search_tree.h
#	gcc -g -c bin_search_tree.c

run:
	./test_program

run_debug:
	valgrind --leak-check=yes --track-origins=yes ./test_program

build_static_lib:
	gcc -c bin_search_tree.c
	ar rcs libbstree.a bin_search_tree.o

clean:
	rm test_program.o bin_search_tree.o test_program
