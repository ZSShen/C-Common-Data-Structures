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


bool PQueuePush(PriorityQueue *self, void *pItem);


void* PQueuePop(PriorityQueue *self);


void* PQueueTop(PriorityQueue *self);


unsigned long PQueueSize(PriorityQueue *self);


void PQueueSetCompare(PriorityQueue *self, int (*pFunc)(const void*, const void*));


void PQueueSetDestroy(PriorityQueue *self, void (*pFunc)(void*));

#endif
