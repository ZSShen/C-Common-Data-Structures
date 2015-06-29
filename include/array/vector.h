/**
 * @file vector.h The dynamically growable array.
 */

#ifndef _VECTOR_H_
#define _VECTOR_H_

#include "../util.h"

/** VectorData is the data type for the container private information. */
typedef struct _VectorData VectorData;

/** The implementation for dynamic array. */
typedef struct _Vector {
    /** The container private information */
    VectorData *pData;

    /** Push an item to the tail of the vector.
        @see VectorPushBack */
    int32_t (*push_back) (struct _Vector*, Item);

    /** Insert an item to the designated index of the vector.
        @see VectorInsert */
    int32_t (*insert) (struct _Vector*, Item, int32_t);

    /** Pop an item from the tail of the vector.
        @see VectorPopBack */
    int32_t (*pop_back) (struct _Vector*);

    /** Delete an item from the designated index of the vector.
        @see VectorDelete */
    int32_t (*delete) (struct _Vector*, int32_t);

    /** Set an item at the designated index of the vector.
        @see VectorSet */
    int32_t (*set) (struct _Vector*, Item, int32_t);

    /** Get an item from the designated index of the vector.
        @see VectorGet */
    int32_t (*get) (struct _Vector*, Item*, int32_t);

    /** Change the container capacity.
        @see VectorResize */
    int32_t (*resize) (struct _Vector*, int32_t);

    /** Return the number of stored items.
        @see VectorSize */
    int32_t (*size) (struct _Vector*);

    /** Return the container capacity.
        @see VectorCapacity */
    int32_t (*capacity) (struct _Vector*);

    /** Set the user defined item clean method.
        @see VectorSetDestroy */
    int32_t (*set_destroy) (struct _Vector*, void (*) (Item));
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
 * If user defined destroy func is set, it also runs the resource clean method 
 * for all the items.
 *
 * @param ppObj         The double pointer to the to be destructed vector
 */
void VectorDeinit(Vector **ppObj);

/**
 * @brief Push an item to the tail of the vector.
 *
 * This function pushes an item to the tail of the vector with the corresponding
 * vector size extension. If the storage is full, the space reallocation is
 * automatically triggered to store the newly pushed item.
 *
 * @param self          The pointer to the Vector structure
 * @param item          The designated item
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_NOMEM    Insufficient memory for vector extension
 */
int32_t VectorPushBack(Vector *self, Item item);

/**
 * @brief Insert an item to the designated index of the vector.
 *
 * This function inserts an item to the designated index of the vector and shifts
 * the trailing items one position to the tail. Naturally, the vector size is extended.
 * And if the storage is full, the space reallocation is automatically triggered
 * to store the newly inserted item.
 *
 * @param self          The pointer to the Vector structure
 * @param item          The designated item
 * @param iIdx          The designated index
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_IDX      Illegal index
 *
 * @note The designated index should be equal to or smaller than the vector
 * size and should not be negative. If the index is equal to the vector size,
 * the effect is equivalent to push_back().
 */
int32_t VectorInsert(Vector *self, Item item, int32_t iIdx);

/**
 * @brief Pop an item from the tail of the vector.
 *
 * This function removes an item from the tail of the vector. If user defined
 * destroy func is set, it also runs the resource clean method for the removed 
 * item. Naturally, the vector size is shrunk.
 *
 * @param self          The pointer to the Vector structure
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_IDX      Empty vector
 */
int32_t VectorPopBack(Vector *self);

/**
 * @brief Delete an item from the designated index of the vector.
 *
 * This function removes an item from the designated index of the vector and shifts
 * the trailing items one position to the head. If user defined destroy func is
 * set, it also runs the resource clean method for the removed item. Naturally, 
 * the vector size is shrunk.
 *
 * @param self          The pointer to the Vector structure
 * @param iIdx          The designated index
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_IDX      Illegal index
 *
 * @note The designated index should be smaller than the vector size and
 * should not be negative. If the index is equal to the vector size minus one,
 * the operation is equivalent to pop_back().
 */
int32_t VectorDelete(Vector *self, int32_t iIdx);

/**
 * @brief Set an item at the designated index of the vector.
 *
 * This function replaces an item at the designated index of the vector. If 
 * user defined destroy func is set, it also runs the resource clean method 
 * for the replaced item.
 *
 * @param self          The pointer to the Vector structure
 * @param item          The designated item
 * @param iIdx          The designated index
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_IDX      Illegal index
 *
 * @note The designated index should be smaller than the vector size and should
 * not be negative.
 */
int32_t VectorSet(Vector *self, Item item, int32_t iIdx);

/**
 * @brief Get an item from the designated index of the vector.
 *
 * This function retrieves an item from the designated index of the vector. If
 * the index is illegal, the error code is returned and the second parameter is
 * also updated with NULL.
 *
 * @param self          The pointer to the Vector structure
 * @param pItem         The pointer to the returned item
 * @param iIdx          The designated index
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_IDX      Illegal index
 *
 * @note The designated index should be smaller than the vector size and should
 * not be negative.
 */
int32_t VectorGet(Vector *self, Item *pItem, int32_t iIdx);

/**
 * @brief Change the container capacity.
 *
 * This function resizes the storage capacity. If the new capacity is smaller
 * than the old size. The trailing items will be removed. If user defined 
 * destroy func is set, it also runs the resource clean method for the removed 
 * items.
 *
 * @param self          The pointer to the Vector structure
 * @param iCap          The designated capacity
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_NOMEM   Insufficient memory for vector extension
 *
 * @note The designated capacity should greater than zero.
 */
int32_t VectorResize(Vector *self, int32_t iCap);

/**
 * @brief Return the number of stored items.
 *
 * @param self          The pointer to the Vector structure
 *
 * @retval ERR_NOINIT   Uninitialized container
 * @return              The number of stored items
 */
int32_t VectorSize(Vector *self);

/**
 * @brief Return the container capacity.
 *
 * @param self          The pointer to the Vector structure
 *
 * @retval ERR_NOINIT   Uninitialized container
 * @return              The container capacity
 */
int32_t VectorCapacity(Vector *self);

/**
 * @brief Set the user defined item clean method.
 *
 * @param self          The pointer to the Vector structure
 * @param pFunc         The function pointer to the custom method
 *
 * @retval ERR_NOINIT   Uninitialized container
 */
int32_t VectorSetDestroy(Vector *self, void (*pFunc) (Item));

#endif
