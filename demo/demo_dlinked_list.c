#include "cds.h"


/* The example usage for primitive data manipulation. */
void usage_primitive();


int main()
{
    usage_primitive();

    return 0;
}


void usage_primitive()
{
    DLinkedList *pList;

    int32_t rc = DListInit(&pList);
    if (rc != SUCC)
        return;

    /* Push an item into the list front-end. */
    pList->push_front(pList, (Item)20);

    /* Push an item into the list back-end. */
    pList->push_back(pList, (Item)40);

    /* Insert the items into designated indicies. */
    pList->insert(pList, (Item)10, 0);
    pList->insert(pList, (Item)50, 3);
    pList->insert(pList, (Item)30, 2);

    /*---------------------------------------------------------------*
     * Now the list should be: (10)<-->(20)<-->(30)<-->(40)<-->(50)  *
     *---------------------------------------------------------------*/
    /* Get the item from the list front-end. */
     Item item;
     pList->get_front(pList, &item);

     /* Get the item from the list back-end. */
     pList->get_back(pList, &item);

     /* Get the items from the designated indicies. */
     pList->get_at(pList, &item, 2);
     pList->get_at(pList, &item, -3);

     /* Replace the item at the list front-end. */
     pList->set_front(pList, (Item)-1, true);

     /* Replace the item at the list back-end. */
     pList->set_back(pList, (Item)-5, true);

     /* Replace the items at the designated indices. */
     pList->set_at(pList, (Item)-2, -4, true);
     pList->set_at(pList, (Item)-4, -2, true);
     pList->set_at(pList, (Item)-3, 2, true);

     /* Reverse the list. */
     pList->reverse(pList);

    /*---------------------------------------------------------------*
     * Now the list should be: (-5)<-->(-4)<-->(-3)<-->(-2)<-->(-1)  *
     *---------------------------------------------------------------*/
    /* Delete the item from the list front-end. */
     pList->pop_front(pList, true);

    /* Delete the item from the list back-end. */
    pList->pop_back(pList, true);

    /* Delete the item from the designated indicies. */
    pList->delete(pList, 1, true);
    pList->delete(pList, -1, true);

    /* Get the list size. And the remaining item should be (-4). */
    int32_t iSize = pList->size(pList);

    DListDeinit(&pList, true);
    return;
}
