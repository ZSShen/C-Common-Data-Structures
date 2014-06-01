#include "dynamic_array.h"
#include "priority_queue.h"

/*===========================================================================*
 *                  Simulation for private variables                         *
 *===========================================================================*/
static unsigned long    _ulSize;
static DynamicArray    *_pArray;
static int  (*_pCompare) (const void*, const void*);
static void (*_pDestroy) (void*);


/*===========================================================================*
 *                  Definition for internal functions                        *
 *===========================================================================*/
int _PQueueItemCompare(const void *pSrc, const void *pTge);


void _PQueueItemDestroy(void *pItem);


/*===========================================================================*
 *                Implementation for exported functions                      *
 *===========================================================================*/
bool PQueueInit(PriorityQueue *self);


void PQueueDeinit(PriorityQueue *self);


bool PQueuePush(PriorityQueue *self, void *pItem);


void* PQueuePop(PriorityQueue *self);


void* PQueueTop(PriorityQueue *self);


void PQueueSetCompare(PriorityQueue *self, int (*pFunc)(const void*, const void*));


void PQueueSetDestroy(PriorityQueue *self, void (*pFunc)(void*));


/*===========================================================================*
 *                Implementation for internal functions                      *
 *===========================================================================*/
