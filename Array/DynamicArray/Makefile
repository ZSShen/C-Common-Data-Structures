main: test_program.o dynamic_array.o
	gcc -o test_program test_program.o dynamic_array.o

debug: test_program.o dynamic_array.o
	gcc -g -o test_program test_program.o dynamic_array.o

test_program.o: test_program.c dynamic_array.h
	gcc -c test_program.c

dynamic_array.o: dynamic_array.c dynamic_array.h
	gcc -c dynamic_array.c

run:
	./test_program

run_debug:
	valgrind --leak-check=yes --track-origins=yes ./test_program

build_static_lib:
	gcc -fPIC -c dynamic_array.c
	ar rcs libdarray.a dynamic_array.o

clean:
	rm test_program.o dynamic_array.o test_program
