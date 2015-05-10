#include "list/dlinked_list.h"


/*===========================================================================*
 *                  Hide the private data of the tree                        *
 *===========================================================================*/
 typedef struct _DListNode {
 	Item item;
 	struct _DListNode *pPrev;
 	struct _DListNode *pNext;
 } DListNode;

struct _DListData {
	int32_t iSize_;
	DListNode *pHead_;
	void (*pDestroy_)(Item);
};

