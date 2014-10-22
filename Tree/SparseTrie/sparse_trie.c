#include "sparse_trie.h"


/* The bases to trasnform ASICC characters into children links. */
#define CHAR_INDEX_BASE_0_9     1
#define CHAR_INDEX_BASE_a_z     11
#define CHAR_INDEX_BASE_A_Z     37


/* The return state of the _STrieDeleteHelper. */
/* Key exists and the relevant node must be deleted. */
#define TRUE_NEED_DELETE        true
/* Key exists and no need to delete the node. */
#define TRUE_NONEED_DELETE      true + 1
/* Key does not exist. */
#define FALSE                   false


/*-----------------------------------------------------------*
 *            Definition for Internal Functions              *
 *-----------------------------------------------------------*/
/**
 * This function creates a new node to store the character metadata.
 *
 * @param ppNew     The pointer to the pointer of new node.
 *
 */
void _STrieCreateNode(TrieNode **ppNew);


void _STrieDeinitHelper(TrieNode *pCurr);


bool _STrieInsertHelper(TrieNode *pCurr, char *szKey, int iLenKey, int iOffsetCurr);


bool _STrieDeleteHelper(TrieNode *pCurr, char *szKey, int iLenKey, int iOffsetCurr);


/**
 * This function transforms the input character into child link index.
 * 
 * @param cChar     The to be trasformed character.
 *
 * @return          The transformed index.
 */
int _STrieCharTransform(char cChar);


/*-----------------------------------------------------------*
 *          Implementation for Exported Functions            *
 *-----------------------------------------------------------*/
bool STrieInit(SparseTrie *self) {
    int i;

    /* Create the dummy root node.  */
    _STrieCreateNode(&(self->pRoot));
    if (self->pRoot == NULL) {
        return false;
    }

    /* Assign the corresponding function pointers.  */
    self->search = STrieSearch;
    self->insert = STrieInsert;
    self->delete = STrieDelete;
    self->set_auto_insert = STrieSetAutoInsert;

    /* Turn on the auto insertion mode by default.  */
    self->bModeAutoInsert = true;

    return true;
}


bool STrieDeinit(SparseTrie *self) {

    /* Apply the helper function to delete all the trie nodes.  */
    _STrieDeinitHelper(self->pRoot);
    
    return true;
}


bool STrieSearch(SparseTrie *self, char *szKey) {
    int  iOffsetCurr, iLenKey, idxChild;
    bool rc;
    TrieNode *pCurr, *pPred;

    rc = true;
    pCurr = self->pRoot;
    iLenKey = strlen(szKey);
    for (iOffsetCurr = 0 ; iOffsetCurr < iLenKey ; iOffsetCurr++) {
        /* Transform the ASCII character into the child link index. */
        idxChild = _STrieCharTransform(szKey[iOffsetCurr]);
    
        /* Check the link. */
        pPred = pCurr;
        pCurr = pCurr->pChildren[idxChild];
    
        /* The key does not exist. */
        if (pCurr == NULL) {
            rc = false;
            break;
        }
    }

    if (rc == true) {
        /* The key does not exist. */
        if (pCurr->bKey == false) {
            rc = false;
        }
    } else {

        /* If the automatic key insertion mode is turned on. */
        if (self->bModeAutoInsert == true) {
            /* Apply the helper function to create the necessary nodes for the given key. */
            /* Note that we ignore the return value here, but the insertion may still fail*/
            /* Even if it fails, it does not influence the result of this key search.     */
            _STrieInsertHelper(pPred, szKey, iLenKey, iOffsetCurr);
        }
    }

    return rc;
}


bool STrieInsert(SparseTrie *self, char *szKey) {
    int  i, iLenKey, idxChild;
    bool rc;

    /* Apply the helper function to create the necessary nodes for the given key. */
    iLenKey = strlen(szKey);
    rc = _STrieInsertHelper(self->pRoot, szKey, iLenKey, 0); 

    return rc;
}


bool STrieDelete(SparseTrie *self, char *szKey) {
    bool rc;

    rc = true;

    return rc;
}


void STrieSetAutoInsert(SparseTrie *self, bool bMode) {

    self->bModeAutoInsert = bMode;
    return;
}


/*-----------------------------------------------------------*
 *          Implementation for Internal Functions            *
 *-----------------------------------------------------------*/
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
    pNew->bKey = false;
    pNew->iCountChildren = 0;

    return;
}


void _STrieDeinitHelper(TrieNode *pCurr) {
    int i;

    if (pCurr != NULL) {
        
        /* Recursively traverse all children.  */
        for (i = 0 ; i <= CHAR_SET ; i++) {
            if (pCurr->pChildren[i] != NULL) {
                _STrieDeinitHelper(pCurr->pChildren[i]);
            }
        }

        /* Release the current node when returning from the children. */
        free(pCurr);
    }
    return;
}


bool _STrieInsertHelper(TrieNode *pCurr, char *szKey, int iLenKey, int iOffsetCurr) {
    int  idxChild;
    bool rc;
    TrieNode *pPred;

    rc = true;
    while (iOffsetCurr < iLenKey) {
        /* Transform the ASCII character into child link index. */
        idxChild = _STrieCharTransform(szKey[iOffsetCurr]);
        pPred = pCurr;
        pCurr = pCurr->pChildren[idxChild];

        /* If the child link is NULL, create a new node for that character. */
        if (pCurr == NULL) {
            _STrieCreateNode(&pCurr);
            if (pCurr == NULL) {
                rc = false;
                break;
            }
            pPred->pChildren[idxChild] = pCurr;
            pPred->iCountChildren = pPred->iCountChildren + 1;
        }
        iOffsetCurr++;
    }

    /* Mark the node storing the last key character as key node. */
    if (iOffsetCurr == iLenKey) {
        pCurr->bKey = true;
    }
    
    return rc;
}



bool _STrieDeleteHelper(TrieNode *pCurr, char *szKey, int iLenKey, int iOffsetCurr) {
    int  idxChild;
    bool rc;

    if (pCurr != NULL) {
        if (iOffsetCurr == iLenKey) {
            /* The given key is found. */
            if (pCurr->bKey == true) {
                /* The key is the prefix of other keys. */
                if (pCurr->iCountChildren > 0) {
                    pCurr->bKey = false;
                    return TRUE_NONEED_DELETE;
                } 
                /* The key locates at the leaf and cascading deletion is started. */
                else {
                    return TRUE_NEED_DELETE; 
                }
            } else {
                return FALSE;
            }
        } else {
            idxChild = _STrieCharTransform(szKey[iOffsetCurr]);
            rc = _STrieDeleteHelper(pCurr->pChildren[idxChild], szKey, iLenKey, iOffsetCurr + 1);
            if (rc != TRUE_NEED_DELETE) {
                return rc;
            }
            
            /* Delete the child and decide whether to continue the cascading deletion. */
            free(pCurr->pChildren[idxChild]);
            pCurr->pChildren[idxChild] = NULL;
            pCurr->iCountChildren = pCurr->iCountChildren - 1;
            if ((pCurr->bKey == true) || (pCurr->iCountChildren > 0)) {
                return TRUE_NONEED_DELETE;
            } else {
                return TRUE_NEED_DELETE;
            }
        }
    }
    return FALSE;
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
