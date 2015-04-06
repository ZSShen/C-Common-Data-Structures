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
 * This function inserts the requested item to the tail of the vector.
 * It will fail when:
 *     1. Insufficient memory space.
 *
 * @param self          The pointer to the Vector structure.
 * @param item          The requested item.
 *
 * @return              SUCCESS
 *                      FAIL_NO_MEMORY
 */
int32_t VectorPushBack(Vector *self, Item item);

/**
 * This function deletes an item from the tail of the vector.
 * It will fail when:
 *     1. The vector is already empty.
 *
 * @param self          The pointer to the Vector structure.
 *
 * @return              SUCCESS
 *                      FAIL_OUT_OF_RANGE
 */
int32_t VectorPopBack(Vector *self);

/**
 * This function inserts the requested item to the designated index of the
 * vector and shifts the trailing elements one position towards the tail.
 * It will fail when:
 *     1. The designated index exceeds the range of the vector.
 *
 * @param self          The pointer to the Vector structure.
 * @param item          The requested item.
 * @param uiIdx         The designated index.
 *
 * @return              SUCCESS
 *                      FAIL_OUT_OF_RANGE
 */
int32_t VectorInsert(Vector *self, Item item, uint32_t uiIdx);

/**
 * This function deletes the item from the designated index and shifts the
 * trailing elements one position towards the head.
 * It will fail when:
 *     1. The designated index exceeds the range of the vector.
 *
 * @param self          The pointer to the Vector structure.
 * @param uiIdx         The designated index.
 *
 * @return              SUCCESS
 *                      FAIL_OUT_OF_RANGE
 */
int32_t VectorDelete(Vector *self, uint32_t uiIdx);


/**
 * This function resizes the vector. Note that if the new size is smaller
 * than the old one, the trailing elements will be removed.
 * It will fail when:
 *     1. Insufficient memory space for vector expansion.
 *
 * @param self          The pointer to the Vector structure.
 * @param uiSize        The designated size.
 *
 * @return              SUCCESS
 *                      FAIL_NO_MEMORY
 */
int32_t VectorResize(Vector *self, uint32_t uiSize);


/**
 * This function returns the number of elements stored in the vector.
 *
 * @param self          The pointer to the Vector structure.
 *
 * @return              Number of the stored elements.
 */
uint32_t VectorSize(Vector *self);

/**
 * This function returns the size of the current capacity.
 *
 * @param self          The pointer to the Vector structure.
 *
 * @return              Size of the current capacity.
 */
uint32_t VectorCapacity(Vector *self);

/**
 * This function returns the item stored in the designated index.
 * It will fail when:
 *     1. The designated index exceeds the range of the vector.
 *
 * @param self          The pointer to the Vector structure.
 * @param uiIdx         The designated index.
 *
 * @return              The corresponding item
 *                      NULL
 */
Item VectorAt(Vector *self, uint32_t uiIdx);

/**
 * This function sets the user defined item deallocation strategy.
 *
 * @param self          The pointer to the Vector structure.
 * @param pFunc         The pointer to the user defined function.
 */
void VectorSetDestroy(Vector *self, void (*pFunc) (Item));

#endif
