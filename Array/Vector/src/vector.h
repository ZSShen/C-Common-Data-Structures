#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

#define SUCCESS                     (0)
#define FAIL_NO_MEMORY              (-1)
#define FAIL_OUT_OF_RANGE           (-2)

typedef const void* Item;
typedef struct _VectorData VectorData;

typedef struct _Vector {
    VectorData *pData;
    int32_t (*push_back) (struct _Vector*, Item);
    int32_t (*pop_back) (struct _Vector*);
    int32_t (*insert) (struct _Vector*, Item, uint32_t);
    int32_t (*delete) (struct _Vector*, uint32_t);
    int32_t (*resize) (struct _Vector*, uint32_t);
    uint32_t (*size) (struct _Vector*);
    uint32_t (*capacity) (struct _Vector*);
    Item (*at) (struct _Vector*, uint32_t);
    void (*set_destroy) (struct _Vector*, void (*) (Item));
} Vector;


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
 * int32_t (*push_back) (Vector *self, Item item)
 *
 * @param self          The pointer to the Vector structure.
 * @param item          The requested item.
 *
 * @return              SUCCESS
 *                      FAIL_NO_MEMORY
 *
 * This function inserts the requested item to the tail of the vector.
 * It will fail under one condition:
 *     1. Insufficient memory space.
 */

/**
 * int32_t (*pop_back) (Vector *self)
 *
 * @param self          The pointer to the Vector structure.
 *
 * @return              SUCCESS
 *                      FAIL_OUT_OF_RANGE
 *
 * This function deletes an item from the tail of the vector.
 * It will fail under one condition:
 *     1. The vector is already empty.
 */

/**
 * int32_t (*insert) (Vector *self, Item item, uint32_t uiIdx)
 *
 * @param self          The pointer to the Vector structure.
 * @param item          The requested item.
 * @param uiIdx         The designated index.
 *
 * @return              SUCCESS
 *                      FAIL_OUT_OF_RANGE
 *
 * This function inserts the requested item into the designated index of the
 * vector and shifts the trailing elements. Notice that if the cell is not
 * empty, the stored item will be removed first.
 * It will fail under one condition:
 *     1. The designated index exceeds the range of the vector.
 */

/**
 * int32_t (*delete) (Vector *self, uint32_t uiIdx)
 *
 * @param self          The pointer to the Vector structure.
 * @param uiIdx         The designated index.
 *
 * @return              SUCCESS
 *                      FAIL_OUT_OF_RANGE
 *
 * This function deletes the item from the designated index and shifts down
 * the elements.
 * It will fail under one condition:
 *     1. The designated index exceeds the range of the vector.
 */

/**
 * int32_t (*resize) (Vector *self, uint32_t uiSize)
 *
 * @param self          The pointer to the Vector structure.
 * @param uiSize        The designated size.
 *
 * @return              SUCCESS
 *                      FAIL_NO_MEMORY
 *
 * This function reshaps the vector with the designated size. Note that if
 * the new size is smaller than the old one, the trailing elements will be
 * removed.
 * It will fail under one condition:
 *     1. Insufficient memory space for vector expansion.
 */

/**
 * uint32_t (*size) (Vector *self)
 *
 * @param self          The pointer to the Vector structure.
 *
 * @return              Number of stored elements.
 *
 * This function returns the number of elements stored in the vector.
 */

/**
 * uint32_t (*capacity) (Vector *self)
 *
 * @param self          The pointer to the Vector structure.
 *
 * @return              Size of the preallocated memory space.
 *
 * This function returns the size of the preallocated memory space.
 */

/**
 * Item (*at) (Vector *self, uint32_t uiIdx)
 *
 * @param self          The pointer to the Vector structure.
 * @param uiIdx         The designated index.
 *
 * @return              The corresponding item
 *                      NULL
 *
 * This function returns the item stored in the designated index.
 * It will fail under one condition:
 *     1. The designated index exceeds the range of the vector.
 */

/**
 * void (*set_destroy) (Vector *self, void (*pFunc) (Item))
 *
 * @param self          The pointer to the Vector structure.
 * @param pFunc         The pointer to the user defined function.
 *
 * This function sets the user defined item deallocation strategy.
 */

#endif
