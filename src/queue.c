#include "container/queue.h"


/*===========================================================================*
 *                  Hide the private data of the queue                       *
 *===========================================================================*/
struct _QueueData {
    int32_t iFront_;
    int32_t iBack_;
    int32_t iSize_;
    int32_t iCapacity_;
    Item *aItem_;
    void (*pDestroy_) (Item);
};

#define DEFAULT_CAPACITY        (32)