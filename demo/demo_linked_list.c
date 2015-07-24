#include "cds.h"


typedef struct Object_ {
    int32_t iId;
    char *szName;
} Object;


void DestroyObject(Item item)
{
    Object *pObj = (Object*)item;
    free(pObj->szName);
    free(pObj);
}


int main()
{
    LinkedList *pList;

    /* You should initialize the DS before any operations. */
    int32_t rc = ListInit(&pList);
    if (rc != SUCC)
        return rc;

    /* If you plan to delegate the resource clean task to the DS, please set the
       custom clean method. */
    pList->set_destroy(pList, DestroyObject);

    /* Push an item to the list head. */
    Object *pObj = (Object*)malloc(sizeof(Object));
    pObj->iId = 20;
    pObj->szName = (char*)malloc(sizeof(char) * 3);
    pObj->szName[0] = 'a'; pObj->szName[1] = 'b'; pObj->szName[2] = 0;
    pList->push_front(pList, (Item)pObj);

    /* Push an item to the list tail. */
    pObj = (Object*)malloc(sizeof(Object));
    pObj->iId = 40;
    pObj->szName = (char*)malloc(sizeof(char) * 3);
    pObj->szName[0] = 'c'; pObj->szName[1] = 'd'; pObj->szName[2] = 0;
    pList->push_back(pList, (Item)pObj);

    /* Insert the items to the designated indexes. */
    pObj = (Object*)malloc(sizeof(Object));
    pObj->iId = 10;
    pObj->szName = (char*)malloc(sizeof(char) * 3);
    pObj->szName[0] = 'e'; pObj->szName[1] = 'f'; pObj->szName[2] = 0;
    pList->insert(pList, (Item)pObj, 0);

    pObj = (Object*)malloc(sizeof(Object));
    pObj->iId = 50;
    pObj->szName = (char*)malloc(sizeof(char) * 3);
    pObj->szName[0] = 'g'; pObj->szName[1] = 'h'; pObj->szName[2] = 0;
    pList->insert(pList, (Item)pObj, 3);

    pObj = (Object*)malloc(sizeof(Object));
    pObj->iId = 30;
    pObj->szName = (char*)malloc(sizeof(char) * 3);
    pObj->szName[0] = 'i'; pObj->szName[1] = 'j'; pObj->szName[2] = 0;
    pList->insert(pList, (Item)pObj, 2);

    /*---------------------------------------------------------------*
     * Now the list should be: (10)<-->(20)<-->(30)<-->(40)<-->(50)  *
     *---------------------------------------------------------------*/

    /* Get the item from the list head. */
    Item item;
    pList->get_front(pList, &item);

    /* Get the item from the list tail. */
    pList->get_back(pList, &item);

    /* Get the items from the designated indexes. */
    pList->get_at(pList, &item, 2);
    pList->get_at(pList, &item, -3);

    /* Replace the item at the list head. */
    pObj = (Object*)malloc(sizeof(Object));
    pObj->iId = -1;
    pObj->szName = (char*)malloc(sizeof(char) * 3);
    pObj->szName[0] = 'k'; pObj->szName[1] = 'l'; pObj->szName[2] = 0;
    pList->set_front(pList, (Item)pObj);

    /* Replace the item at the list tail. */
    pObj = (Object*)malloc(sizeof(Object));
    pObj->iId = -5;
    pObj->szName = (char*)malloc(sizeof(char) * 3);
    pObj->szName[0] = 'm'; pObj->szName[1] = 'n'; pObj->szName[2] = 0;
    pList->set_back(pList, (Item)pObj);

    /* Replace the items at the designated indexes. */
    pObj = (Object*)malloc(sizeof(Object));
    pObj->iId = -2;
    pObj->szName = (char*)malloc(sizeof(char) * 3);
    pObj->szName[0] = 'o'; pObj->szName[1] = 'p'; pObj->szName[2] = 0;
    pList->set_at(pList, (Item)pObj, -4);

    pObj = (Object*)malloc(sizeof(Object));
    pObj->iId = -4;
    pObj->szName = (char*)malloc(sizeof(char) * 3);
    pObj->szName[0] = 'q'; pObj->szName[1] = 'r'; pObj->szName[2] = 0;
    pList->set_at(pList, (Item)pObj, -2);

    pObj = (Object*)malloc(sizeof(Object));
    pObj->iId = -3;
    pObj->szName = (char*)malloc(sizeof(char) * 3);
    pObj->szName[0] = 's'; pObj->szName[1] = 't'; pObj->szName[2] = 0;
    pList->set_at(pList, (Item)pObj, 2);

    /* Reverse the list. */
    pList->reverse(pList);

    /*---------------------------------------------------------------*
     * Now the list should be: (-5)<-->(-4)<-->(-3)<-->(-2)<-->(-1)  *
     *---------------------------------------------------------------*/

    /* Delete the item from the list head. */
    pList->pop_front(pList);

    /* Delete the item from the list tail. */
    pList->pop_back(pList);

    /* Delete the item from the designated indexes. */
    pList->delete(pList, 1);
    pList->delete(pList, -1);

    /* Get the list size. And the remaining item should be (-4). */
    int32_t iSize = pList->size(pList);

    /* You should deinitialize the DS after all the relevant tasks. */
    ListDeinit(&pList);

    return 0;
}
