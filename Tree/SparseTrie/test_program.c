#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "sparse_trie.h"


#define BUF_SIZE 4096


bool test_key_input(SparseTrie *pTrie, char *szFileInput);


int main(int argc, char **argv) {
    bool rc;
    char *szFileInput, *szFileAnswer;
    SparseTrie *pTrie;

    /* Prepare the input and answer datasets. */
    if (argc != 3) {
        printf("Please specify the input and answer datasets.");
        return -1;
    }
    szFileInput = argv[1];
    szFileAnswer = argv[2];

    /* Create the SparseTrie instance. */
    init_SparseTrie(pTrie);
    if (pTrie == NULL) {
        goto EXIT;
    }

    /* Run the key insertion test suit. */
    rc = test_key_input(pTrie, szFileInput);
    if (rc == false) {
        goto EXIT;
    }


    /* Release the allocated resources. */
    deinit_SparseTrie(pTrie);
EXIT:
    return 0;
}


bool test_key_input(SparseTrie *pTrie, char *szFileInput) {
    bool rc;
    char *ret;
    FILE *fpInput;
    char buf[BUF_SIZE + 1];

    /* Open the test suit. */
    fpInput = fopen(szFileInput, "r");
    if (fpInput == NULL) {
        return false;
    }

    rc = true;
    while (true) {

        /* Read single case. */
        memset(buf, 0, sizeof(char) * (BUF_SIZE + 1));
        ret = fgets(buf, BUF_SIZE, fpInput);
        if (ret == NULL) {
            break;
        }

        /* Insert the case into trie. */    
        rc = pTrie->insert(pTrie, buf);
        if (rc == false) {
            break;
        }
    }

    /* Close the test suit. */
    fclose(fpInput);

    return rc;
}
