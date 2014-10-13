
# Define the constants for output binary and the dependent library.
SRC_NAME 		:= ordered_map
LIB_NAME 		:= ordmap
TEST_PROG 		:= test_program
PATH_CUR 		:= $(shell pwd)
PATH_IMPORT_LIB := $(PATH_CUR)/../../Tree/RedBlackTree
IMPORT_SRC_NAME := red_black_tree
IMPORT_LIB_NAME := rbtree
IMPORT_BIN_NAME := lib$(IMPORT_LIB_NAME).a
IMPORT_HAD_NAME := $(IMPORT_SRC_NAME).h


# List the dependencies for project building.
DEPENDENCY_LIB 	:= $(SRC_NAME)
DEPENDENCY_TEST := $(TEST_PROG)
VPATH 			:= $(PATH_CUR)


# Specify the compilation options.
CC 			:= gcc
FLAG 		:=
LIB 		:= -L. -l$(IMPORT_LIB_NAME)
SO_FLAG 	:= -shared
ARCH 		:= ar
ARCH_OPT 	:= rcs
ifeq ($(DEBUG), true)
	FLAG := $(FLAG) -g
endif
ifeq ($(COVERAGE), true)
	FLAG  := $(FLAG) -O0 --coverage
	COVER := -lgcov
endif


# List the preprosessing rules to link the dependent library.
PARAM :=
ifeq ($(DEBUG), true)
	PARAM := $(PARAM) DEBUG=true 
endif
ifeq ($(COVERAGE), true)
	PARAM := $(PARAM) COVERAGE=true
endif
MAKE_CLEAN := make clean
MAKE_IMPORT_LIB := make build_static_lib $(PARAM)
COPY_IMPORT_LIB := cp $(PATH_IMPORT_LIB)/$(IMPORT_BIN_NAME) $(PATH_IMPORT_LIB)/$(IMPORT_HAD_NAME) \
					  $(PATH_CUR)

# List the project building rules.
build_test: include_lib $(DEPENDENCY_LIB) $(DEPENDENCY_TEST)
	$(CC) $(FLAG) -o $(TEST_PROG) *.o $(LIB)

build_static_lib: FLAG := $(FLAG) -fPIC
build_static_lib: include_lib $(DEPENDENCY_LIB)
	$(ARCH) $(ARCH_OPT) lib$(LIB_NAME).a $(SRC_NAME).o

build_dynamic_lib: FLAG := $(FLAG) -fPIC
build_dynamic_lib: include_lib $(DEPENDENCY_LIB)
	$(CC) $(SO_FLAG) $(SRC_NAME).o -o lib$(LIB_NAME).so $(COVER)

$(DEPENDENCY_TEST):
	$(CC) $(FLAG) $(LIB) -c $@.c -o $@.o

$(DEPENDENCY_LIB):
	$(CC) $(FLAG) $(LIB) -c $@.c -o $@.o

include_lib:
	cd $(PATH_IMPORT_LIB) \
	&& $(MAKE_IMPORT_LIB) \
	&& $(COPY_IMPORT_LIB) \


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

