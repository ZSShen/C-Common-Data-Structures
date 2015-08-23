#include "container/stack.h"


/*===========================================================================*
 *                  Hide the private data of the stack                       *
 *===========================================================================*/
struct _StackData {
    int32_t iTop_;
    int32_t iSize_;
    int32_t iCapacity_;
    Item *aItem_;
    void (*pDestroy_) (Item);
};

#define DEFAULT_CAPACITY        (32)