#include "sparse_trie.h"


/* The bases to trasnform ASICC characters into children links. */
#define CHAR_INDEX_BASE_0_9     1
#define CHAR_INDEX_BASE_a_z     11
#define CHAR_INDEX_BASE_A_Z     37


/* The return state of the _STrieDeleteHelper. */
/* Key exists and the relevant node must be deleted. */
#define TRUE_NEED_DELETE        1
/* Key exists and no need to delete the node. */
#define TRUE_NONEED_DELETE      2
/* Key does not exist. */
#define FALSE                   0


/*-----------------------------------------------------------*
 *            Definition for Internal Functions              *
 *-----------------------------------------------------------*/
/**
 * This function creates a new node to store the character metadata.
 *
 * @param ppNew     The pointer to the pointer of to be created new node.
 *
 */
void _STrieCreateNode(TrieNode **ppNew);


/**
 * This function helps to delete all the allocated trie node.
 * 
 * @param pCurr     The pointer to the root of trie. (The root of subtrie in recursive view.)
 */
void _STrieDeinitHelper(TrieNode *pCurr);


/**
 * This function helps to insert the relevant nodes of a given key.
 *
 * @param pCurr         The pointer to the root of subtrie which collects all the
 *                      possible suffixes of the given key prefix.
 * @param szKey         The designated search key.
 * @param iLenKey       The length of the search key.
 * @param iOffsetCurr   The offset to the search key.
 *
 * @return         true : Insertion is done successfully.
 *                 false: The nodes cannot be allocated due to insufficient memory.
 */
bool _STrieInsertHelper(TrieNode *pCurr, char *szKey, int iLenKey, int iOffsetCurr);


/**
 * This function helps to adjust the node attribute or to delete the relevant nodes as
 * the maintenance tasks for key deletion.
 *
 * @param pCurr         The pointer to the root of subtrie which collects all the
 *                      possible suffixes of the given key prefix.
 * @param szKey         The designated search key.
 * @param iLenKey       The length of the search key.
 * @param iOffsetCurr   The offset to the search key.
 *
 * @return         TRUE_NEED_DELETE   : The child node should be deleted.
 *                 TRUE_NONEED_DELETE : The child node should not be deleted.
 *                 FALSE              : The key cannot be found.
 */
int _STrieDeleteHelper(TrieNode *pCurr, char *szKey, int iLenKey, int iOffsetCurr);


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


/**
 * STrieSearch(): Search for the given key.
 */
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


/**
 * STrieInsert(): Insert the given key into the trie.
 */
bool STrieInsert(SparseTrie *self, char *szKey) {
    int  i, iLenKey;
    bool rc;

    /* Apply the helper function to create the necessary nodes for the given key. */
    iLenKey = strlen(szKey);
    rc = _STrieInsertHelper(self->pRoot, szKey, iLenKey, 0); 

    return rc;
}


/**
 * STrieDelete(): Delete the given key from the trie.
 */
bool STrieDelete(SparseTrie *self, char *szKey) {
    int  i, iLenKey;    
    bool rc;

    /* Apply the helper function to change the node attribute or to delete the 
       relevant nodes as maintenance task. */
    iLenKey = strlen(szKey);
    rc = _STrieDeleteHelper(self->pRoot, szKey, iLenKey, 0);

    return rc;
}


/**
 * STrieSetAutoInsert(): Control the mode of auto insertion for key search.
 */
void STrieSetAutoInsert(SparseTrie *self, bool bMode) {

    self->bModeAutoInsert = bMode;
    return;
}


/*-----------------------------------------------------------*
 *          Implementation for Internal Functions            *
 *-----------------------------------------------------------*/
/**
 * _STrieCreateNode(): Create a new node to store the character metadata.
 */
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


/**
 * _STrieDeinitHelper(): Help to delete all the allocated trie node.
 */
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


/**
 * _STrieInsertHelper(): Help to insert the relevant nodes of a given key.
 */
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


/**
 * _STrieDeleteHelper(): Help to adjust the node attribute or to delete the 
 * relevant nodes as the maintenance tasks for key deletion.
 */
int _STrieDeleteHelper(TrieNode *pCurr, char *szKey, int iLenKey, int iOffsetCurr) {
    int rc, idxChild;

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
            
            /* Delete the child and determine whether to continue the cascading deletion. */
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


/**
 * _STrieCharTransform(): Transform the input character into child link index.
 */
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
