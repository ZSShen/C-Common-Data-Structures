#include "sparse_trie.h"


#define CHAR_INDEX_BASE_0_9     1
#define CHAR_INDEX_BASE_a_z     11
#define CHAR_INDEX_BASE_A_Z     37


/*---------------------------------------------------*
 *        Definition for Internal Functions          *
 *---------------------------------------------------*/
void _STrieCreateNode(TrieNode **pNew);


void _STrieDeinitHelper(TrieNode *pCurr);


int _STrieCharTransform(char cChar);


/*---------------------------------------------------*
 *      Implementation for Exported Functions        *
 *---------------------------------------------------*/
bool STrieInit(SparseTrie *pTrie) {
    int i;

    /* Create the dummy root node.  */
    _STrieCreateNode(&(pTrie->pRoot));
    if (pTrie->pRoot == NULL) {
        return false;
    }

    /* Assign the corresponding function pointers.  */
    pTrie->search = STrieSearch;
    pTrie->insert = STrieInsert;
    pTrie->delete = STrieDelete;
    pTrie->set_auto_insert = STrieSetAutoInsert;

    /* Turn on the auto insertion mode by default.  */
    pTrie->bModeAutoInsert = true;

    return true;
}


bool STrieDeinit(SparseTrie *pTrie) {

    /* Apply the helper function to delete all the trie nodes.  */
    _STrieDeinitHelper(pTrie->pRoot);
    
    return true;
}


bool STrieSearch(SparseTrie *pTrie, char *szKey) {
    bool rc;

    rc = true;

    return rc;
}


bool STrieInsert(SparseTrie *pTrie, char *szKey) {
    int  i, iLenKey, idxChild;
    bool rc;
    TrieNode *pCurr, *pPred;

    rc = true;
    
    pCurr = pTrie->pRoot;
    iLenKey = strlen(szKey);
    for (i = 0 ; i < iLenKey ; i++) {
        
        /* Transform the ascii character to the child index. */
        idxChild = _STrieCharTransform(szKey[i]);
        
        /* Go through the designated child.  */
        if (pCurr->bLeaf == true) {
            pCurr->bLeaf = false;
        }
        pPred = pCurr;   
        pCurr = pCurr->pChildren[idxChild];
        
        /* If the child is empty, Create a new node for it. */
        if (pCurr == NULL) {
            _STrieCreateNode(&pCurr);            
            if (pCurr == NULL) {
                rc = false;
                break;
            }
            pPred->pChildren[idxChild] = pCurr;
        }
    }

    return rc;
}


bool STrieDelete(SparseTrie *pTrie, char *szKey) {
    bool rc;

    rc = true;

    return rc;
}


void STrieSetAutoInsert(SparseTrie *pTrie, bool bMode) {

    pTrie->bModeAutoInsert = bMode;
    return;
}


/*---------------------------------------------------*
 *      Implementation for Internal Functions        *
 *---------------------------------------------------*/
void _STrieCreateNode(TrieNode **ppNew) {
    int i;
    TrieNode *pNew;

    *ppNew = (TrieNode*)malloc(sizeof(TrieNode));
    if (*ppNew == NULL) {
        return;
    }
    pNew = *ppNew;

    for (i = 0 ; i <= CHAR_SET ; i++) {
        pNew->pChildren[i] = NULL;
    }   
    pNew->bLeaf = true;

    return;
}


void _STrieDeinitHelper(TrieNode *pCurr) {
    int i;

    if (pCurr != NULL) {
        
        /* Recursively traverse all non-NULL children.  */
        for (i = 0 ; i <= CHAR_SET ; i++) {
            if (pCurr->pChildren[i] != NULL) {
                _STrieDeinitHelper(pCurr->pChildren[i]);
            }
        }

        /* Release the current node upon returning from the children. */
        free(pCurr);
    }
    return;
}


int _STrieCharTransform(char cChar) {

    if ((cChar >= '0') && (cChar <= '9')) {
        return cChar - '0' + CHAR_INDEX_BASE_0_9;
    } else if ((cChar >= 'a') && (cChar <= 'z')) {
        return cChar - 'a' + CHAR_INDEX_BASE_a_z;
    } else if ((cChar >= 'A') && (cChar <= 'Z')) {
        return cChar - 'A' + CHAR_INDEX_BASE_A_Z; 
    } else {
        return 0;
    }
}
