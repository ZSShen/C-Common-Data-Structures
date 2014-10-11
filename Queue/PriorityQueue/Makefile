
# Define the constants for output binary and the dependent library.
SRC_NAME            := priority_queue
LIB_NAME            := pqueue
TEST_PROG           := test_program
PATH_CUR            := $(shell pwd)

HEAP_INTERFACE 		:= heap.h
PATH_INTERFACE_GEN 	:= $(PATH_CUR)/../../Misc/interface_generator.py
PATH_BINARY_HEAP    := $(PATH_CUR)/../../Heap/BinaryHeap
PATH_BINOMIAL_HEAP  := $(PATH_CUR)/../../Heap/BinomialHeap

BINARY_HEAP_SRC     := binary_heap
BINARY_HEAP_LIB     := binaryheap
BINARY_HEAP_BIN_S   := lib$(BINARY_HEAP_LIB).a
BINARY_HEAP_BIN_D 	:= lib$(BINARY_HEAP_LIB).so
BINARY_HEAP_HAD     := $(BINARY_HEAP_SRC).h
BINOMIAL_HEAP_SRC   := binomial_heap
BINOMIAL_HEAP_LIB   := binomialheap
BINOMIAL_HEAP_BIN_S := lib$(BINOMIAL_HEAP_LIB).a
BINOMIAL_HEAP_BIN_D := lib$(BINOMIAL_HEAP_LIB).so
BINOMIAL_HEAP_HAD 	:= $(BINOMIAL_HEAP_SRC).h


# List the dependencies for project building.
DEPENDENCY_LIB := $(SRC_NAME)
DEPENDENCY_TEST := $(TEST_PROG)
VPATH := $(PATH_CUR)


# Specify the compilation options.
CC := gcc
FLAG :=
LIB := -L. -ldl
SO_FLAG := -shared
ARCH := ar
ARCH_OPT := rcs
ifeq ($(DEBUG), true)
	FLAG := $(FLAG) -g
endif
ifeq ($(COVERAGE), true)
	FLAG := $(FLAG) -O0 --coverage
endif


# List the preprosessing rules to link the dependent library.
PARAM :=
ifeq ($(DEBUG), true)
	PARAM := $(PARAM) DEBUG=true 
endif
ifeq ($(COVERAGE), true)
	PARAM := $(PARAM) COVERAGE=true
endif

# $(1): For the root folder of the designated data structure.
# $(2): For its header file.
# $(3): For its shared object file.
define preprocess
	make build_dynamic_lib $(PARAM)
	$(PATH_INTERFACE_GEN) $(1)/$(2) $(PATH_CUR)/$(HEAP_INTERFACE)
	cp $(1)/$(3) $(PATH_CUR)
endef


# List the project building rules.
build_test: include_dynamic_lib $(DEPENDENCY_LIB) $(DEPENDENCY_TEST)
	$(CC) $(FLAG) -o $(TEST_PROG) *.o $(LIB)

#build_static_lib: include_lib $(DEPENDENCY_LIB)
#	$(ARCH) $(ARCH_OPT) lib$(LIB_NAME).a $(SRC_NAME).o

#build_dynamic_lib: FLAG := $(FLAG) -fPIC
#build_dynamic_lib: include_lib $(DEPENDENCY_LIB)
#	$(CC) $(SO_FLAG) $(SRC_NAME).o -o lib$(LIB_NAME).so

$(DEPENDENCY_TEST):
	$(CC) $(FLAG) $(LIB) -c $@.c -o $@.o

$(DEPENDENCY_LIB):
	$(CC) $(FLAG) $(LIB) -c $@.c -o $@.o

include_dynamic_lib:
	cd $(PATH_BINARY_HEAP) && \
    $(call preprocess, $(PATH_BINARY_HEAP),$(BINARY_HEAP_HAD),$(BINARY_HEAP_BIN_D))
	cd $(PATH_BINOMIAL_HEAP) && \
    $(call preprocess, $(PATH_BINOMIAL_HEAP),$(BINOMIAL_HEAP_HAD),$(BINOMIAL_HEAP_BIN_D))


# List the project execution rules.
test:
ifeq ($(DEBUG), true) 
	valgrind --leak-check=yes --track-origins=yes ./$(TEST_PROG) -i $(IMPLEMENT)
else
	./$(TEST_PROG) -i $(IMPLEMENT)
endif


# List the coverage testing rule.
coverage: test
	

# List the project cleaning rule.
.PHONY: clean
clean:
	rm -f *.o *.a *.so *.gcda *.gcno $(TEST_PROG) $(HEAP_INTERFACE)

