
# Prepare the list of implemented data structures.
PATH_CUR                := $(shell pwd)
PATH_BIN_TREE           := $(PATH_CUR)/Tree/BinarySearchTree
PATH_RB_TREE            := $(PATH_CUR)/Tree/RedBlackTree
PATH_SLLIST             := $(PATH_CUR)/List/SinglyLinkedList
PATH_DARRAY             := $(PATH_CUR)/Array/DynamicArray
PATH_ORDER_MAP          := $(PATH_CUR)/Map/OrderedMap
PATH_BINARY_HEAP        := $(PATH_CUR)/Heap/BinaryHeap
PATH_BINOMIAL_HEAP      := $(PATH_CUR)/Heap/BinomialHeap
PATH_PRIORITY_QUEUE     := $(PATH_CUR)/Queue/PriorityQueue

LIST := $(PATH_BIN_TREE)
LIST := $(LIST) $(PATH_RB_TREE)
LIST := $(LIST) $(PATH_SLLIST)
LIST := $(LIST) $(PATH_DARRAY)
LIST := $(LIST) $(PATH_ORDER_MAP)
LIST := $(LIST) $(PATH_BINARY_HEAP)
LIST := $(LIST) $(PATH_BINOMIAL_HEAP)
LIST := $(LIST) $(PATH_PRIORITY_QUEUE)


# Prepare the building rules for implemented data structures.
# Note that $(IMPLEMENT) is used only for priority queue testing.
FLAG_DEBUG := 
ifeq ($(DEBUG), true)
	FLAG_DEBUG := $(FLAG_DEBUG) DEBUG=true
endif
FLAG_COVERAGE :=
ifeq ($(COVERAGE), true)
	FLAG_COVERAGE := $(FLAG_COVERAGE) COVERAGE=true
endif
MAKE_CLEAN      := make clean
MAKE_BUILD      := make build_test $(FLAG_DEBUG) $(FLAG_COVERAGE)
MAKE_COVERAGE   := make test $(FLAG_DEBUG)


# List the project building rule.
all:
	for path in $(LIST); do \
        cd $$path; \
        $(MAKE_CLEAN); \
        $(MAKE_BUILD); \
        echo "\n\n"; \
    done

$(filter-out SomethingElse,$(VAR))

# List the coverage testing rule.
coverage: LIST := $(filter-out $(PATH_PRIORITY_QUEUE), $(LIST))
coverage:
	for path in $(LIST); do \
        cd $$path; \
        $(MAKE_COVERAGE); \
        echo "\n\n"; \
    done
	cd $(PATH_PRIORITY_QUEUE) \
    && $(MAKE_COVERAGE) IMPLEMENT=binaryheap \
    && $(MAKE_COVERAGE) IMPLEMENT=binomialheap \
