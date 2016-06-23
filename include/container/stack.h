/**
 * @file stack.h The LIFO stack.
 */

#ifndef _STACK_H_
#define _STACK_H_

#include "../util.h"

#ifdef __cplusplus
extern "C" {
#endif

/** StackData is the data type for the container private information. */
typedef struct _StackData StackData;

/** The implementation for stack. */
typedef struct _Stack {
    /** The container private information */
    StackData *pData;

    /** Insert an item to the top of the stack.
        @see StackPush */
    int32_t (*push) (struct _Stack*, Item);

    /** Retrieve item from the top of the stack.
        @see StackTop */
    int32_t (*top) (struct _Stack*, Item*);

    /** Delete item from the top of the stack.
        @see StackPop */
    int32_t (*pop) (struct _Stack*);

    /** Return the number of stored items.
        @see StackSize */
    int32_t (*size) (struct _Stack*);

    /** Set the custom item resource clean method.
        @see StackSetDestroy */
    int32_t (*set_destroy) (struct _Stack*, void (*) (Item));
} Stack;


/*===========================================================================*
 *             Definition for the exported member operations                 *
 *===========================================================================*/
/**
 * @brief The constructor for Stack.
 *
 * @param ppObj         The double pointer to the to be constructed stack
 *
 * @retval SUCC
 * @retval ERR_NOMEM    Insufficient memory for stack construction
 */
int32_t StackInit(Stack **ppObj);

/**
 * @brief The destructor for Stack.
 *
 * If the custom resource clean method is set, it also runs the clean method
 * for all the items.
 *
 * @param ppObj         The double pointer to the to be destructed stack
 */
void StackDeinit(Stack **ppObj);

/**
 * @brief Insert an item to the top of the stack.
 *
 * This function inserts an item to the top of the stack with the corresponding
 * stack size extension.
 *
 * @param self          The pointer to Stack structure
 * @param item          The designated item
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_NOMEM    Insufficient memory for stack extension
 */
int32_t StackPush(Stack *self, Item item);

/**
 * @brief Delete item from top of the stack.
 *
 * This function deletes item from the top of the stack. If the custom resource
 * clean method is set, it also runs the clean method for the deleted item.
 *
 * @param self          The pointer to Stack structure
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_IDX      Empty stack
 */
int32_t StackPop(Stack *self);

/**
 * @brief Retrieve item from top of the stack.
 *
 * This function retrieves item from the top of the stack. If the stack is not
 * empty, the item is returned by the second parameter. Otherwise, the error
 * code is returned and the second parameter is updated with NULL.
 *
 * @param self          The pointer to Stack structure
 * @param pItem         The pointer to the returned item
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 * @retval ERR_IDX      Empty stack
 * @retval ERR_GET      Invalid parameter to store returned item
 */
int32_t StackTop(Stack *self, Item *pItem);

/**
 * @brief Return the number of stored items.
 *
 * @param self          The pointer to Stack structure
 *
 * @return              The number of items
 * @retval ERR_NOINIT   Uninitialized container
 */
int32_t StackSize(Stack *self);

/**
 * @brief Set the custom item resource clean method.
 *
 * @param self          The pointer to Stack structure
 * @param pFunc         The function pointer to the custom method
 *
 * @retval SUCC
 * @retval ERR_NOINIT   Uninitialized container
 */
int32_t StackSetDestroy(Stack *self, void (*pFunc) (Item));

#ifdef __cplusplus
}
#endif

#endif