#include "map/ordered_map.h"
#include "tree/balanced_tree.h"


/*===========================================================================*
 *                  Hide the private data of the tree                        *
 *===========================================================================*/
struct _OMapData {
	BalancedTree *pTree;
	int32_t (*pCompare_) (Key, Key);
	void (*pDestroy_) (Value);
};


/*===========================================================================*
 *                  Definition for internal operations                       *
 *===========================================================================*/
/**
 * @brief The default order comparison method for a pair of map entries.
 *
 * @param keySrc 		The key of the source entry
 * @param keyTge 		The key of the target entry
 *
 * @retval 1            The source entry has the larger order
 * @retval 0            Both the entries have the same order
 * @retval -1           The source entry has the smaller order
 */
int32_t _OdrMapKeyCompare(Key keySrc, Key keyTge);

/**
 * @brief The default resource clean method for the value of a map entry.
 *
 * @param value  		The designated map value
 */
void _OdrMapValueDestroy(Value value);


/*===========================================================================*
 *               Implementation for internal operations                      *
 *===========================================================================*/
int32_t OdrMapInit(OrderedMap **ppObj)
{
	return SUCC;
}

void OdrMapDeinit(OrderedMap **ppObj) {}


int32_t OdrMapPut(OrderedMap *self, Pair pair)
{
	return SUCC;
}

int32_t OdrMapGet(OrderedMap *self, Key key, Value *pValue)
{
	return SUCC;
}

int32_t OdrMapRemove(OrderedMap *self, Key key)
{
	return SUCC;
}

int32_t OdrMapSize(OrderedMap *self)
{
	return 0;
}

void OdrMapSetCompare(OrderedMap *self, int32_t (*pFunc) (Key, Key)) {}

void OdrMapSetDestroy(OrderedMap *self, void (*pFunc) (Value)) {}