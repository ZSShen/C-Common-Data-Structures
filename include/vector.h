#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#define SUCCESS                     (0)
#define FAIL_NO_MEMORY              (-1)
#define FAIL_OUT_OF_RANGE           (-2)

typedef const void* Item;
typedef struct _VectorData VectorData;

typedef struct _Vector {
    VectorData *pData;

    /* The operators for data insertion. */
    int32_t (*push_back) (struct _Vector*, Item);
    int32_t (*insert) (struct _Vector*, Item, int32_t);

    /* The operators for data deletion. */
    int32_t (*pop_back) (struct _Vector*);
    int32_t (*delete) (struct _Vector*, int32_t);

    /* The operators for direct index. */
    int32_t (*set) (struct _Vector*, Item, int32_t);
    int32_t (*get) (struct _Vector*, *Item, int32_t);

    /* The operator to change the internal storage. */
    int32_t (*resize) (struct _Vector*, uint32_t);

    /* The operators to query the container capacity. */
    uint32_t (*size) (struct _Vector*);
    uint32_t (*capacity) (struct _Vector*);

    /* The operators to set the user defined resource management policy. */
    void (*set_destroy) (struct _Vector*, void (*) (Item));
} Vector;


/*===========================================================================*
 *             Definition for the exported member operations                 *
 *===========================================================================*/
/**
 * The constructor for Vector.
 *
 * @param ppObj         The double pointer to the to be constructed vector.
 *
 * @return              SUCCESS
 *                      FAIL_NO_MEMORY
 */
int32_t VectorInit(Vector **ppObj);

/**
 * The destructor for Vector.
 *
 * @param ppObj          The double pointer to the to be destructed vector.
 */
void VectorDeinit(Vector **ppObj);

/**
 * This function inserts the requested item into the tail of the vector with the
 * corresponding vector size extension. If the internal storage is full, the space
 * reallocation is automatically triggered to store the newly inserted item.
 *
 * It will fail when:
 *   (1) Insufficient memory space for vector expansion.
 *
 * @param self          The pointer to the Vector structure.
 * @param item          The requested item.
 *
 * @return              SUCCESS
 *                      FAIL_NO_MEMORY
 */
int32_t VectorPushBack(Vector *self, Item item);

/**
 * This function inserts the requested item into the designated index of the
 * vector and shifts the trailing items one position towards the tail. Naturally,
 * the vector size is extended. And if the internal storage is full, the space
 * reallocation is automatically triggered to store the newly inserted item.
 *
 * Note that the designated index should be equal to or smaller than the vector
 * size and should not be negative. If the index is equal to the vector size,
 * the operation is equivalent to the push_back().
 *
 * It will fail when:
 *   (1) The designated index exceeds the range of the vector.
 *
 * @param self          The pointer to the Vector structure.
 * @param item          The requested item.
 * @param iIdx          The designated index.
 *
 * @return              SUCCESS
 *                      FAIL_OUT_OF_RANGE
 */
int32_t VectorInsert(Vector *self, Item item, int32_t iIdx);

/**
 * This function removes an item from the end of the vector and applies the
 * configured policy to clean the resource acquired by that item. Naturally,
 * the vector size is shrunk.
 *
 * It will fail when:
 *   (1) The vector is already empty.
 *
 * @param self          The pointer to the Vector structure.
 *
 * @return              SUCCESS
 *                      FAIL_OUT_OF_RANGE
 */
int32_t VectorPopBack(Vector *self);

/**
 * This function removes the item from the designated index of the vector and
 * shifts the trailing items one position towards the head. The resource acquired
 * by the item will also be cleaned by the configured policy. And naturally, the
 * vector size is shrunk.
 *
 * Note that the designated index should be smaller than the vector size and
 * should not be negative. If the index is equal to the vector size minus one,
 * the operation is equivalent to the pop_back().
 *
 * It will fail when:
 *   (1) The designated index exceeds the range of the vector.
 *
 * @param self          The pointer to the Vector structure.
 * @param iIdx          The designated index.
 *
 * @return              SUCCESS
 *                      FAIL_OUT_OF_RANGE
 */
int32_t VectorDelete(Vector *self, int32_t iIdx);

/**
 * This function inserts the requested item into the designated index of the
 * vector and cleans the previously stored item.
 *
 * Note that the designated index should be smaller than the vector size.
 *
 * It will fail when:
 *   (1) The designated index exceeds the range of the vector.
 *
 * @param self          The pointer to the Vector structure.
 * @param item          The requested item.
 * @param iIdx          The designated index.
 *
 * @return              SUCCESS
 *                      FAIL_OUT_OF_RANGE
 */
int32_t VectorSet(Vector *self, Item item, int32_t iIdx);

/**
 * This function returns the item stored in the designated index.
 *
 * Note that the designated index should be smaller than the vector size.
 *
 * It will fail when:
 *   (1) The designated index exceeds the range of the vector.
 *
 * @param self          The pointer to the Vector structure.
 * @param item          The pointer to the returned item.
 * @param iIdx          The designated index.
 */
int32_t VectorGet(Vector *self, Item *pItem, int32_t iIdx);

/**
 * This function resizes the capacity of the internal storage. If the new
 * capacity is smaller than the old size. The trailing items will be cleaned.
 *
 * Note that the designated capacity should greater than zero.
 *
 * It will fail when:
 *   (1) Insufficient memory space for vector expansion.
 *
 * @param self         The pointer to the Vector structure.
 * @param iCap         The designated capacity.
 *
 * @return              SUCCESS
 *                      FAIL_NO_MEMORY
 */
int32_t VectorResize(Vector *self, int32_t iCap);

/**
 * This function returns the number of items stored in the vector.
 *
 * @param self          The pointer to the Vector structure.
 *
 * @return              Number of the stored items.
 */
int32_t VectorSize(Vector *self);

/**
 * This function returns the capacity of the internal storage.
 *
 * @param self          The pointer to the Vector structure.
 *
 * @return              Size of the current capacity.
 */
int32_t VectorCapacity(Vector *self);

/**
 * This function sets the user defined item resource clean strategy.
 *
 * @param self          The pointer to the Vector structure.
 * @param pFunc         The pointer to the user defined function.
 */
void VectorSetDestroy(Vector *self, void (*pFunc) (Item));

#endif
