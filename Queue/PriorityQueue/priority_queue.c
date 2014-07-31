#include "dynamic_array.h"
#include "priority_queue.h"

#define BUF_SIZE_MID    (256)


/*===========================================================================*
 *                  Simulation for private variables                         *
 *===========================================================================*/
static void *_hdleLib;
static void *_tableAPI[NUM_FUNCS];

static int  (*_pCompare) (const void*, const void*);
static void (*_pDestroy) (void*);


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
    char szPathLib[BUF_SIZE_MID];

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

    /* Let the function pointers point to the corresponding functions. */
    /*
    _pCompare = _PQueueItemCompare;
    _pDestroy = _PQueueItemDestroy;    

    self->push = PQueuePush;
    self->pop = PQueuePop;
    self->top = PQueueTop;
    self->size = PQueueSize;
    
    self->set_compare = PQueueSetCompare;
    self->set_destroy = PQueueSetDestroy;
    */

    return true;
}


void PQueueDeinit(PriorityQueue *self) {
   
    /* Release the resources occupied by the heap structure. */
    ((FPTR_DEINIT)_tableAPI[FUNC_DEINIT])();

    /* Release the dynamic loaded library. */
    if (_hdleLib != NULL)    
        dlclose(_hdleLib);

    return;
}


/**
 * PQueuePush(): Insert an item to the appropriate position of the queue.
 */


/**
 * PQueuePop(): Retrieve and delete the top item from the queue.
 */



/**
 * PQueueTop(): Retrieve the top item from the queue.
 */


/**
 * PQueueSize(): Return the size of the queue.
 */


/**
 * PQueueSetCompare(): Set the item comparison strategy with the one defined by user.
 */


/**
 * PQueueSetDestroy(): Set the item deallocation strategy with the one defined by user.
 */


/*===========================================================================*
 *                Implementation for internal functions                      *
 *===========================================================================*/
/**
 * _PQueueItemCompare(): The default comparison strategy for a pair of queue items.
 * Note: It considers source and target items as primitive data and 
 *       gives the higher priority to the one with larger value.
 */



/**
 * _PQueueItemDestroy(): The default deallocation strategy for an item.
 * Note: By default, the item is a primitive data, and thus no further operations are required.
 */

