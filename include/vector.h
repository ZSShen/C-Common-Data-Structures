#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#define SUCC                     (0)
#define ERR_NOMEM              (-1)
#define ERR_IDX           (-2)

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
    int32_t (*get) (struct _Vector*, Item*, int32_t);

    /* The operator to change the internal storage. */
    int32_t (*resize) (struct _Vector*, int32_t);

    /* The operators to query the container capacity. */
    int32_t (*size) (struct _Vector*);
    int32_t (*capacity) (struct _Vector*);

    /* The operators to set the user defined resource management policy. */
    void (*set_destroy) (struct _Vector*, void (*) (Item));
} Vector;


/*===========================================================================*
 *             Definition for the exported member operations                 *
 *===========================================================================*/
/**
 * @brief The constructor for Vector.
 *
 * @param ppObj         The double pointer to the to be constructed vector
 *
 * @retval SUCC
 * @retval ERR_NOMEM    Insufficient memory for vector construction
 */
int32_t VectorInit(Vector **ppObj);

/**
 * @brief The destructor for Vector.
 *
 * @param ppObj          The double pointer to the to be destructed vector
 */
void VectorDeinit(Vector **ppObj);

/**
 * @brief Push an item into the end of the vector.
 *
 * This function pushes the requested item into the end of the vector with the
 * corresponding vector size extension. If the internal storage is full, the space
 * reallocation is automatically triggered to store the newly pushed item.
 *
 * @param self          The pointer to the Vector structure
 * @param item          The requested item
 *
 * @retval SUCC
 * @retval ERR_NOMEM    Insufficient memory for vector expansion
 */
int32_t VectorPushBack(Vector *self, Item item);

/**
 * @brief Insert an item into the designated index of the vector.
 *
 * This function inserts the requested item into the designated index of the
 * vector and shifts the trailing items one position to the end. Naturally,
 * the vector size is extended. And if the internal storage is full, the space
 * reallocation is automatically triggered to store the newly inserted item.
 *
 * @param self          The pointer to the Vector structure
 * @param item          The requested item
 * @param iIdx          The designated index
 *
 * @retval SUCC
 * @retval ERR_IDX      The index exceeding the range of the vector
 *
 * @note The designated index should be equal to or smaller than the vector
 * size and should not be negative. If the index is equal to the vector size,
 * the operation is equivalent to push_back().
 *
 */
int32_t VectorInsert(Vector *self, Item item, int32_t iIdx);

/**
 * @brief Pop and item from the end of the vector.
 *
 * This function removes an item from the end of the vector and applies the
 * configured policy to clean the resource acquired by that item. Naturally,
 * the vector size is shrunk.
 *
 * @param self          The pointer to the Vector structure
 *
 * @retval SUCC
 * @retval ERR_IDX      The vector is already empty
 */
int32_t VectorPopBack(Vector *self);

/**
 * @brief Delete an item from the designated index of the vector.
 *
 * This function removes the item from the designated index of the vector and
 * shifts the trailing items one position to the head. The resource acquired
 * by the item will also be cleaned by the configured policy. And naturally, the
 * vector size is shrunk.
 *
 * @param self          The pointer to the Vector structure
 * @param iIdx          The designated index
 *
 * @retval SUCC
 * @retval ERR_IDX      The index exceeding the range of the vector
 *
 * @note The designated index should be smaller than the vector size and
 * should not be negative. If the index is equal to the vector size minus one,
 * the operation is equivalent to pop_back().
 */
int32_t VectorDelete(Vector *self, int32_t iIdx);

/**
 * @brief Set the requested item at the designated index of the vector.
 *
 * This function puts the requested item into the designated index of the
 * vector and cleans the previously stored item.
 *
 * @param self          The pointer to the Vector structure
 * @param item          The requested item
 * @param iIdx          The designated index
 *
 * @retval SUCC
 * @retval ERR_IDX      The index exceeding the range of the vector
 *
 * @note The designated index should be smaller than the vector size and should
 * not be negative.
 */
int32_t VectorSet(Vector *self, Item item, int32_t iIdx);

/**
 * @brief Get an item from the designated index of the vector.
 *
 * @param self          The pointer to the Vector structure
 * @param item          The pointer to the returned item
 * @param iIdx          The designated index
 *
 * @retval SUCC
 * @retval ERR_IDX      The index exceeding the range of the vector
 *
 * @note The designated index should be smaller than the vector size and should
 * not be negative.
 */
int32_t VectorGet(Vector *self, Item *pItem, int32_t iIdx);

/**
 * @brief Change the container capacity.
 *
 * This function resizes the capacity of the internal storage. If the new
 * capacity is smaller than the old size. The trailing items will be cleaned.
 *
 * @param self         The pointer to the Vector structure
 * @param iCap         The designated capacity
 *
 * @retval SUCC
 * @retval ERR_NOMEM   Insufficient memory for vector expansion
 *
 * @note The designated capacity should greater than zero.
 */
int32_t VectorResize(Vector *self, int32_t iCap);

/**
 * @brief Return the number of stored items.
 *
 * @param self          The pointer to the Vector structure
 *
 * @return              The number of stored items
 */
int32_t VectorSize(Vector *self);

/**
 * @brief Return the container capacity.
 *
 * @param self          The pointer to the Vector structure
 *
 * @return              The container capacity
 */
int32_t VectorCapacity(Vector *self);

/**
 * @brief Set the user defined clean strategy for the resource hold by an item.
 *
 * @param self          The pointer to the Vector structure
 * @param pFunc         The pointer to the user defined function
 */
void VectorSetDestroy(Vector *self, void (*pFunc) (Item));

#endif
