#ifndef _HEAP_H_
#define _HEAP_H_

/*----------------------------------------------------*
 *      The common interface for heap structure.      *
 *----------------------------------------------------*/


/* The index of supported heap APIs. */
enum {
    FUNC_INIT,
    FUNC_DEINIT,
    FUNC_INSERT,
    FUNC_DELETE,
    FUNC_TOP,
    FUNC_SIZE,
    FUNC_SET_COMPARE,
    FUNC_SET_DESTROY,
    NUM_FUNCS
};


/* The mapping table for the API name. */
#define gTableFuncName  ((char const *[]) {"init",\
                                           "deinit",\
                                           "insert",\
                                           "delete",\
                                           "top",\
                                           "size",\
                                           "set_compare",\
                                           "set_destroy" })


/* The pointer type of each API. */
typedef void          (*FPTR_INIT)   ();
typedef void          (*FPTR_DEINIT) ();
typedef void*         (*FPTR_DELETE) ();
typedef bool          (*FPTR_INSERT) (void*);
typedef void*         (*FPTR_TOP)    ();
typedef unsigned long (*FPTR_SIZE)   (); 
typedef void (*FPTR_SET_COMPARE)     (int(*)(const void*, const void*));
typedef void (*FPTR_SET_DESTROY)     (void(*)(void*));

#endif
