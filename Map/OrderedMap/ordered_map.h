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


/**
 * This function inserts a key-value pair into the appropriate position of the map. 
 *
 * @param self      The pointer to the OrderedMap structure.
 * @param pPair     The pointer to the key-value pair which is to be inserted.
 * 
 * @return          true : The pair is inserted successfully.
 *                  false: The pair cannot be inserted due to unenough memory space.
 */
bool OrderedMapPut(OrderedMap *self, KeyValuePair *pPair);


/**
 * This function searches and returns a key-value pair using the specified key.
 *
 * @param self      The pointer to the OrderedMap structure.
 * @param pKey      The pointer to the specified key.
 *
 * @return          Non-NULL: The pointer to the pair with the key equal to the specified one.
 *                  NULL    : There is no pair having the same key with the specified one, and thus
 *                            nothing is returned.
 */
KeyValuePair* OrderedMapGet(OrderedMap *self, void *pKey);


/**
 * This function removes the key-value pair with the key equal to the specified one.
 *
 * @param self      The pointer to the OrderedMap structure.
 * @param pKey      The pointer to the specified key.
 *
 * @return          true : The pair is removed successfully.
 *                  false: The pair cannot be found.
 */
bool OrderedMapRemove(OrderedMap *self, void *pKey);


/**
 * This function returns the size of the map.
 *
 * @param sefl      The pointer to the OrderedMap structure.
 */
unsigned long OrderedMapSize(OrderedMap *self);


/**
 * This function sets the key-value pair comparison strategy with the one defined by user.
 *
 * @param self          The pointer to the OrderedMap structure.
 * @param pFunc         The pointer to the customized function.
 */
void OrderedMapSetCompare(OrderedMap *self, int (*pFunc)(const void*, const void*));


/**
 * This function sets the key-value pair deallocation strategy with the one defined by user.
 *
 * @param self          The pointer to the OrderedMap structure.
 * @param pFunc         The pointer to the customized function.
 */
void OrderedMapSetDestroy(OrderedMap *self, void (*pFunc)(void*));

#endif
