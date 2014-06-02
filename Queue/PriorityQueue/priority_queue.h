#ifndef _H_PRIORITY_QUEUE_
#define _H_PRIORITY_QUEUE_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


/* Wrapper for PriorityQueue initialization. */
#define PriorityQueue_init(p)       p = (PriorityQueue*)malloc(sizeof(PriorityQueue)); \
                                    if (p != NULL) { \
                                        bool check; \
                                        check = PQueueInit(p); \
                                        if (check == false) { \
                                            free(p); \
                                            p = NULL; \
                                        } \
                                    }


/* Wrapper for PriorityQueue deinitialization. */
#define PriorityQueue_deinit(p)     if (p != NULL) { \
                                        PQueueDeinit(p); \
                                        free(p); \
                                        p = NULL; \
                                    }


typedef struct _PriorityQueue {

    bool            (*push)        (struct _PriorityQueue*, void*);
    void*           (*pop)         (struct _PriorityQueue*);
    void*           (*top)         (struct _PriorityQueue*);
    unsigned long   (*size)        (struct _PriorityQueue*);
    void            (*set_compare) (struct _PriorityQueue*, int (*)(const void*, const void*));
    void            (*set_destroy) (struct _PriorityQueue*, void (*)(void*));
} PriorityQueue;


/* Constructor for PriorityQueue structure. */
bool PQueueInit(PriorityQueue *self);


/* Destructor for PriorityQueue structure. */
void PQueueDeinit(PriorityQueue *self);


/**
 * This function insertes an item to the appropriate position of the queue.
 *
 * @param self      The pointer to the PriorityQueue structure.
 * @param pItem     The pointer to the item which is to be inserted to the queue.
 *
 * @return          true : The item is inserted successfully.
 *                  false: The item cannot be inserted due to insufficient memory space.
 */
bool PQueuePush(PriorityQueue *self, void *pItem);

/**
 * This function retrieves and deletes the top item from the queue.
 * @param self      The pointer to the PriorityQueue structure.
 * 
 * @return          Non-NULL: The pointer to the top item.
 *                  NULL    : The queue is empty, and thus nothing is returned.
 */
void* PQueuePop(PriorityQueue *self);


/**
 * This function retrieves and deletes the top item from the queue.
 * @param self      The pointer to the PriorityQueue structure.
 * 
 * @return          Non-NULL: The pointer to the top item.
 *                  NULL    : The queue is empty, and thus nothing is returned.
 */
void* PQueueTop(PriorityQueue *self);


/**
 * This function returns the size of the queue.
 *
 * @param self      The pointer to the PriorityQueue structure.
 *
 * @return          The size;
 */
unsigned long PQueueSize(PriorityQueue *self);


/**
 * This function sets the item comparison strategy with the one defined by user.
 *
 * @param self          The pointer to the PriorityQueue structure.
 * @param pFunc         The pointer to the customized function.
 */
void PQueueSetCompare(PriorityQueue *self, int (*pFunc)(const void*, const void*));


/**
 * This function sets the item deallocation strategy with the one defined by user.
 *
 * @param self          The pointer to the PriorityQueue structure.
 * @param pFunc         The pointer to the customized function.
 */
void PQueueSetDestroy(PriorityQueue *self, void (*pFunc)(void*));

#endif
