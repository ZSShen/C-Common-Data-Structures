#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <getopt.h>
#include "sparse_trie.h"


#define BUF_SIZE_SMALL      64
#define BUF_SIZE_LARGE      4096
#define OPT_HELP            'h'
#define OPT_INSERT          'i'
#define OPT_SEARCH          's'
#define OPT_DELETE          'd'
#define OPT_LONG_HELP       "help"
#define OPT_LONG_INSERT     "insert"
#define OPT_LONG_SEARCH     "search"
#define OPT_LONG_DELETE     "delete"

#define EARLY_EXIT          print_usage(); \
                            rc = -1; \
                            goto EXIT;


/* The unit test function for the key insertion. */
bool test_key_insert(SparseTrie *pTrie, char *szInsert);


/* The unit test function for the key search. */
bool test_key_search(SparseTrie *pTrie, char *szSearch);


/* The unit test function for the key deletion. */
bool test_key_delete(SparseTrie *pTrie, char *szDelete);


/* Print the help message to use this program. */
void print_usage();


int main(int argc, char **argv) {
    int  rc, idxOpt;
    bool ret;
    char opt;
    char *szInsert, *szSearch, *szDelete;
    SparseTrie *pTrie;
    char szOrder[BUF_SIZE_SMALL];

    /* Retrieve the paths of test cases. */
    static struct option Options[] = {
        {OPT_LONG_INSERT, required_argument, 0, OPT_INSERT},
        {OPT_LONG_SEARCH, required_argument, 0, OPT_SEARCH},
        {OPT_LONG_DELETE, required_argument, 0, OPT_DELETE}
    };
    memset(szOrder, 0, sizeof(char) * BUF_SIZE_SMALL);
    sprintf(szOrder, "%c%c:%c:%c:", OPT_HELP, OPT_INSERT, OPT_SEARCH, OPT_DELETE);

    rc = 0;
    idxOpt = 0;
    szInsert = szSearch = szDelete = NULL;
    while ((opt = getopt_long(argc, argv, szOrder, Options, &idxOpt)) != -1) {
        switch (opt) {
            case OPT_INSERT: {
                szInsert = optarg;
                break;
            }
            case OPT_SEARCH: {
                szSearch = optarg;
                break;
            }
            case OPT_DELETE: {
                szDelete = optarg;
                break;            
            }
            case OPT_HELP: {            
                EARLY_EXIT
            }
            default: {
                EARLY_EXIT
            }
        }
    }

    if (szInsert == NULL) {
        EARLY_EXIT       
    }
    if (szSearch == NULL) {
        EARLY_EXIT
    }
    if (szDelete == NULL) {
        EARLY_EXIT
    }

    /* Create the SparseTrie instance. */
    SparseTrie_init(pTrie);
    if (pTrie == NULL) {
        rc = -1;
        goto EXIT;
    }

    /* Turn of the auto key insertion mode. */
    pTrie->set_auto_insert(pTrie, false);

    /* First round: Insert the keys and test the searching capability. */
    ret = test_key_insert(pTrie, szInsert);
    if (ret == false) {
        rc = -1;
        goto DEINIT;
    }
    ret = test_key_search(pTrie, szSearch);
    if (ret == false) {
        rc = -1;
        goto DEINIT;
    }
    
    /* Second round: Delete some keys and then search again to test the
       maintenance capability. */
    ret = test_key_delete(pTrie, szDelete);
    if (ret == false) {
        rc = -1;
        goto DEINIT;
    }
    ret = test_key_search(pTrie, szSearch);
    if (ret == false) {
        rc = -1;
        goto DEINIT;
    }

    /* Release the allocated resources. */
DEINIT:
    SparseTrie_deinit(pTrie);
EXIT:
    return rc;
}


void print_usage() {
    
    const char *cszMsg = "Usage: test_program --insert(-i) path_insert\n"
                         "       test_program --search(-s) path_search\n"
                         "       test_program --delete(-d) path_delete\n"
                         "       path_insert: The path to the key insertion test case.\n"
                         "       path_search: The path to the key searching test case.\n"
                         "       path_delete: The path to the key deletion test case.\n"
                         "-------------------------------------------------------------\n"
                         "Example: test_program --insert path_insert --search path_search"
                         " --delete path_delete\n"
                         "         test_program -i       path_insert -s       path_search"
                         " -d       path_delete\n\n";
    printf("%s", cszMsg);
    return;
}


bool test_key_insert(SparseTrie *pTrie, char *szInsert) {
    int  iLenKey;    
    bool rc;
    char *ret;
    FILE *fpInsert;
    char buf[BUF_SIZE_LARGE + 1];

    fpInsert = fopen(szInsert, "r");
    if (fpInsert == NULL) {
        return false;
    }

    /* Iteratively insert each key into the trie. */
    rc = true;
    while (true) {
        memset(buf, 0, sizeof(char) * (BUF_SIZE_LARGE + 1));
        ret = fgets(buf, BUF_SIZE_LARGE, fpInsert);
        if (ret == NULL) {
            break;
        }
        iLenKey = strlen(buf);
        buf[iLenKey - 1] = 0;
        rc = pTrie->insert(pTrie, buf);
        if (rc == false) {
            break;
        }
    }

    fclose(fpInsert);
    return rc;
}


bool test_key_search(SparseTrie *pTrie, char *szSearch) {
    int  iLenKey;    
    bool rc;
    char *ret;
    FILE *fpSearch;
    char buf[BUF_SIZE_LARGE + 1];

    fpSearch = fopen(szSearch, "r");
    if (fpSearch == NULL) {
        return false;
    }

    /* Iteratively search the trie for each given key. */
    while (true) {
        memset(buf, 0, sizeof(char) * (BUF_SIZE_LARGE + 1));
        ret = fgets(buf, BUF_SIZE_LARGE, fpSearch);
        if (ret == NULL) {
            break;
        }
        iLenKey = strlen(buf);
        buf[iLenKey - 1] = 0;
        rc = pTrie->search(pTrie, buf);
        /* printf("%s %d\n", buf, rc); */
    }
    /* printf("\n"); */

    fclose(fpSearch);
    return true;
}


bool test_key_delete(SparseTrie *pTrie, char *szDelete) {
    int  iLenKey;        
    bool rc;
    char *ret;
    FILE *fpDelete;
    char buf[BUF_SIZE_LARGE + 1];

    fpDelete = fopen(szDelete, "r");
    if (fpDelete == NULL) {
        return false;
    }

    /* Iteratively delete each key from the trie. */
    rc = false;
    while (true) {
        memset(buf, 0, sizeof(char) * (BUF_SIZE_LARGE + 1));
        ret = fgets(buf, BUF_SIZE_LARGE, fpDelete);
        if (ret == NULL) {
            break;
        }
        iLenKey = strlen(buf);
        buf[iLenKey - 1] = 0;
        rc = pTrie->delete(pTrie, buf);
    }

    fclose(fpDelete);
    return true;
}
