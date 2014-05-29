#ifndef _ORDERED_MAP_H_
#define _ORDERED_MAP_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


/* Wrapper for OrderedMap initialization. */
#define OrderedMap_init(p)      p = NULL; \
                                p = (OrderedMap*)malloc(sizeof(OrderedMap)); \
                                if (p != NULL) { \
                                    bool check; \
                                    check = OrderedMapInit(p); \
                                    if (check == false) { \
                                        free(p); \
                                        p = NULL; \
                                    } \
                                }


/* Wrapper for OrderedMap deinitialization. */
#define OrderedMap_deinit(p)    if (p != NULL) { \
                                    OrderedMapDeinit(p); \
                                    free(p); \
                                    p = NULL; \
                                }


typedef struct _KeyValePair {
    void *pKey;
    void *pValue;
} KeyValuePair;


typedef struct _OrderedMap {

    bool          (*put)      (struct _OrderedMap*, KeyValuePair*);
    KeyValuePair* (*get)      (struct _OrderedMap*, void*);
    bool          (*remove)   (struct _OrderedMap*, void*);
    unsigned long (*size)     (struct _OrderedMap*);

    void          (*set_compare) (struct _OrderedMap*, int (*) (const void*, const void*));
    void          (*set_destroy) (struct _OrderedMap*, void (*) (void*));    
} OrderedMap;


bool OrderedMapInit(OrderedMap *self);


void OrderedMapDeinit(OrderedMap *self);


bool OrderedMapPut(OrderedMap *self, KeyValuePair *pPair);


KeyValuePair* OrderedMapGet(OrderedMap *self, void *pKey);


bool OrderedMapRemove(OrderedMap *self, void *pKey);


unsigned long OrderedMapSize(OrderedMap *self);


void OrderedMapSetCompare(OrderedMap *self, int (*pFunc)(const void*, const void*));


void OrderedMapSetDestroy(OrderedMap *self, void (*pFunc)(void*));


#endif
