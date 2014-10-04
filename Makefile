
# Prepare the list of implemented data structures.
PATH_CUR := $(shell pwd)
PATH_BIN_TREE := $(PATH_CUR)/Tree/BinarySearchTree
LIST := $(PATH_BIN_TREE)


# Prepare the building rules for implemented data structures.
FLAG_DEBUG := 
ifeq ($(DEBUG), true)
	FLAG_DEBUG := $(FLAG_DEBUG) DEBUG=true
endif
FLAG_COVERAGE :=
ifeq ($(COVERAGE), true)
	FLAG_COVERAGE := $(FLAG_COVERAGE) COVERAGE=true
endif

MAKE_CLEAN := make clean
MAKE_BUILD := make build_test $(FLAG_DEBUG) $(FLAG_COVERAGE)
MAKE_COVERAGE := make test $(FLAG_DEBUG)


# List the project building rule.
all:
	for path in $(LIST); do \
        cd $$path; \
        $(MAKE_CLEAN); \
        $(MAKE_BUILD); \
    done


# List the coverage testing rule.
coverage:
	for path in $(LIST); do \
        cd $$path; \
        $(MAKE_COVERAGE); \
    done

