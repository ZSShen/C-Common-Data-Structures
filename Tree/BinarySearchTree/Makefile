
# Define the constants for output binary.
SRC_NAME    := bin_search_tree
LIB_NAME    := bstree
TEST_PROG   := test_program


# List the dependencies for project building.
DEPENDENCY_LIB  := $(SRC_NAME)
DEPENDENCY_TEST := $(TEST_PROG)


# Specify the compilation options.
CC          := gcc
FLAG        :=
SO_FLAG     := -shared
ARCH        := ar
ARCH_OPT    := rcs
ifeq ($(DEBUG), true)
	FLAG := $(FLAG) -g
endif
ifeq ($(COVERAGE), true)
	FLAG  := $(FLAG) -O0 --coverage
	COVER := -lgcov
endif


# List the project building rules.
build_test: $(DEPENDENCY_LIB) $(DEPENDENCY_TEST)
	$(CC) $(FLAG) -o $(TEST_PROG) *.o

build_static_lib: FLAG := $(FLAG) -fPIC
build_static_lib: $(DEPENDENCY_LIB)
	$(ARCH) $(ARCH_OPT) lib$(LIB_NAME).a $(SRC_NAME).o

build_dynamic_lib: FLAG := $(FLAG) -fPIC
build_dynamic_lib: $(DEPENDENCY_LIB)
	$(CC) $(SO_FLAG) $(SRC_NAME).o -o lib$(LIB_NAME).so $(COVER)

$(DEPENDENCY_TEST):
	$(CC) $(FLAG) -c $@.c -o $@.o

$(DEPENDENCY_LIB):
	$(CC) $(FLAG) -c $@.c -o $@.o


# List the project execution rules.
test:
ifeq ($(DEBUG), true)
	valgrind --leak-check=yes --track-origins=yes ./$(TEST_PROG)
else
	./$(TEST_PROG)
endif


# List the coverage testing rule.
coverage: test
	

# List the project cleaning rule.
.PHONY: clean
clean:
	rm -f *.o *.a *.so *.gcda *.gcno $(TEST_PROG)

