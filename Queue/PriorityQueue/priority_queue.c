#include "priority_queue.h"

#define BUF_SIZE_MID    (256)


/*===========================================================================*
 *                  Simulation for private variables                         *
 *===========================================================================*/
static void *_hdleLib;
static void *_tableAPI[NUM_FUNCS];


/*===========================================================================*
 *                  Definition for internal functions                        *
 *===========================================================================*/

/**
 * This function is the default comparison strategy for a pair of queue items.
 * 
 * @param pSrc      The pointer to the source item.
 * @param pTge      The pointer to the target item.
 *
 * @return          1 : The source item goes after the target one with certain ordering criteria.
 *                  0 : The source item equals to the target one with certain ordering criteria.
 *                  -1: The source item goes before the target one with certain ordering criteria.
 */
int _PQueueItemCompare(const void *pSrc, const void *pTge);


/**
 * This function is the default deallocation strategy for a queue item.
 * 
 * @param pItem    The pointer to the item which is to be deallocated.
 */
void _PQueueItemDestroy(void *pItem);


/*===========================================================================*
 *                Implementation for exported functions                      *
 *===========================================================================*/
bool PQueueInit(PriorityQueue *self, const char *cszNameLib) {
    int  i;
    bool bInit;    
    char szPathLib[BUF_SIZE_MID];
    FPTR_INIT func_init;
    FPTR_SET_COMPARE func_set_compare;
    FPTR_SET_DESTROY func_set_destroy;

    /* Prepare the path of the designated shared library. */
    memset(szPathLib, 0, sizeof(char) * BUF_SIZE_MID);
    sprintf(szPathLib, "./lib%s.so", cszNameLib);

    /* Load the designated heap libarary. */
    _hdleLib = dlopen(szPathLib, RTLD_LAZY);
    if (_hdleLib == NULL) {
        printf("Error: %s\n", dlerror());
        return false;
    }

    /* Get the relevant function pointers. */
    for (i = 0 ; i < NUM_FUNCS ; i++) {
        _tableAPI[i] = dlsym(_hdleLib, gTableFuncName[i]);
        if (_tableAPI[i] == NULL) {
            printf("Error: %s\n", dlerror());
            return false;
        }
    }

    /* Initialize the heap structure. */
    func_init = (FPTR_INIT)_tableAPI[FUNC_INIT];
    bInit = func_init();
    if (bInit == false) {
        printf("Error: Cannot initialize library %s.\n", cszNameLib);    
        return false;
    }

    /* Specify the default item comparison and deallocation strategies for the heap. */
    func_set_compare = (FPTR_SET_COMPARE)_tableAPI[FUNC_SET_COMPARE];
    func_set_destroy = (FPTR_SET_DESTROY)_tableAPI[FUNC_SET_DESTROY];
    func_set_compare(_PQueueItemCompare);
    func_set_destroy(_PQueueItemDestroy);    
    

    /* Assign the corresponding instance of each function prototype. */
    self->push = PQueuePush;
    self->pop = PQueuePop;
    self->top = PQueueTop;
    self->size = PQueueSize;
    self->set_compare = PQueueSetCompare;
    self->set_destroy = PQueueSetDestroy;

    return true;
}


void PQueueDeinit(PriorityQueue *self) {
    FPTR_DEINIT func_deinit;   

    /* Release the resources occupied by the heap structure. */
    func_deinit = (FPTR_DEINIT)_tableAPI[FUNC_DEINIT];
    func_deinit();

    /* Release the dynamic loaded library. */
    if (_hdleLib != NULL)    
        dlclose(_hdleLib);

    return;
}


/**
 * PQueuePush(): Insert an item to the appropriate position of the queue.
 */
bool PQueuePush(PriorityQueue *self, void *pItem) {
    bool rc;    
    FPTR_INSERT func_insert;    

    /* Insert the designated item into the heap. */
    func_insert = (FPTR_INSERT)_tableAPI[FUNC_INSERT];
    rc = func_insert(pItem);    

    return rc;
}


/**
 * PQueuePop(): Retrieve and delete the top item from the queue.
 */
void* PQueuePop(PriorityQueue *self) {
    void *pItem;
    FPTR_DELETE func_delete;

    /* Delete and return the top item of the heap. */
    func_delete = (FPTR_DELETE)_tableAPI[FUNC_DELETE];
    pItem = func_delete();

    return pItem;
}


/**
 * PQueueTop(): Retrieve the top item from the queue.
 */
void* PQueueTop(PriorityQueue *self) {
    void *pItem;
    FPTR_TOP func_top;
    
    /* Return the top item of the heap. */
    func_top = (FPTR_TOP)_tableAPI[FUNC_TOP];
    pItem = func_top();

    return pItem;
}


/**
 * PQueueSize(): Return the size of the queue.
 */
unsigned long PQueueSize(PriorityQueue *self) {
    unsigned long ulSize;
    FPTR_SIZE func_size;

    /* Return the size of the heap. */
    func_size = (FPTR_SIZE)_tableAPI[FUNC_SIZE];
    ulSize = func_size();

    return ulSize;
}


/**
 * PQueueSetCompare(): Set the item comparison strategy with the one defined by user.
 */
void PQueueSetCompare(PriorityQueue *self, int (*pFunc)(const void*, const void*)) {
    FPTR_SET_COMPARE func_set_compare;

    /* Change the item comparison strategy of the heap. */
    func_set_compare = (FPTR_SET_COMPARE)_tableAPI[FUNC_SET_COMPARE];
    func_set_compare(pFunc);

    return;
}


/**
 * PQueueSetDestroy(): Set the item deallocation strategy with the one defined by user.
 */
void PQueueSetDestroy(PriorityQueue *self, void (*pFunc)(void*)) {
    FPTR_SET_DESTROY func_set_destroy;

    /* Change the item comparison strategy of the heap. */
    func_set_destroy = (FPTR_SET_DESTROY)_tableAPI[FUNC_SET_DESTROY];
    func_set_destroy(pFunc);

    return;
}


/*===========================================================================*
 *                Implementation for internal functions                      *
 *===========================================================================*/
/**
 * _PQueueItemCompare(): The default comparison strategy for a pair of queue items.
 * Note: It considers source and target items as primitive data and 
 *       gives the higher priority to the one with larger value.
 */
int _PQueueItemCompare(const void *pSrc, const void *pTge) {
    size_t nSrc, nTge;
    
    nSrc = (size_t)pSrc;
    nTge = (size_t)pTge;

    return nSrc - nTge;
}


/**
 * _PQueueItemDestroy(): The default deallocation strategy for an item.
 * Note: By default, the item is a primitive data, and thus no further operations are required.
 */
void _PQueueItemDestroy(void *pItem) {

    return;
}

