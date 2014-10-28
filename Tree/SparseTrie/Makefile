
# Define the constants for output binary.
SRC_NAME    := sparse_trie
LIB_NAME    := trie
TEST_PROG   := test_program


# Define the constants for test case generation.
PATH_CUR    			:= $(shell pwd)
PATH_CASE_GENERATOR 	:= $(PATH_CUR)/../../Misc/dictionary_generator.py
CASE_INSERT := insert.case
CASE_SEARCH := search.case
CASE_DELETE := delete.case
COMPARE_LOG := compare.log


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
	$(PATH_CASE_GENERATOR)
ifeq ($(DEBUG), true)
	valgrind --leak-check=yes --track-origins=yes ./$(TEST_PROG) \
    -i $(CASE_INSERT) -s $(CASE_SEARCH) -d $(CASE_DELETE) -o $(COMPARE_LOG)
else
	./$(TEST_PROG) -i $(CASE_INSERT) -s $(CASE_SEARCH) -d $(CASE_DELETE) \
    --out=$(COMPARE_LOG)
endif


# List the coverage testing rule.
coverage: test
	

# List the project cleaning rule.
.PHONY: clean
clean:
	rm -f *.o *.a *.so *.gcda *.gcno *.case *.log $(TEST_PROG)

