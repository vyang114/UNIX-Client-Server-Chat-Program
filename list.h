// List data type
// You may modify this file as needed; however,
// you may *NOT* modify the function prototypes or constant names.

#ifndef _LIST_H_
#define _LIST_H_
#include <stdbool.h>

#define LIST_SUCCESS 0
#define LIST_FAIL -1


typedef struct Node_s Node;
struct Node_s {
    // TODO: You should change this
    void* item;
    int position;
    Node *prev;
    Node *next;
};

enum ListOutOfBounds {
    LIST_OOB_START,
    LIST_OOB_END
};
typedef struct List_s List;
struct List_s{
    // TODO: You should change this!
    int index;
    int item_count;
	Node *head;
	Node *tail;
	Node *current;
	enum ListOutOfBounds list_state;
};

// Maximum number of unique lists the system can support
// (You may modify for your needs) should be 10
#define LIST_MAX_NUM_HEADS 10

// Maximum total number of nodes (statically allocated) to be shared across all lists
// (You may modify for your needs)
#define LIST_MAX_NUM_NODES 100

// General Error Handling:
// Client code is assumed never to call these functions with a NULL List pointer, or 
// bad List pointer. If it does, any behaviour is permitted (such as crashing).
// HINT: Use assert(pList != NULL); just to add a nice check, but not required.

// Makes a new, empty list, and returns its reference on success. 
// Returns a NULL pointer on failure. O(N)
List* List_create();

// Returns the number of items in pList.
int List_count(List* pList);

// Returns a pointer to the first item in pList and makes the first item the current item.
// Returns NULL and sets current item to NULL if list is empty.
void* List_first(List* pList);

// Returns a pointer to the current item in pList.
void* List_curr(List* pList);

// Adds the new item to pList directly after the current item, and makes item the current item. 
// If the current pointer is before the start of the pList, the item is added at the start. If 
// the current pointer is beyond the end of the pList, the item is added at the end. 
// Returns 0 on success, -1 on failure.
int List_add(List* pList, void* pItem);

int List_append(List* pList, void* pItem);


// Return current item and take it out of pList. Make the next item the current one.
// If the current pointer is before the start of the pList, or beyond the  of the pList,
// then do not change the pList and return NULL.
void* List_remove(List* pList);

// Delete pList. pItemFreeFn is a pointer to a routine that frees an item. 
// It should be invoked (within List_free) as: (*pItemFreeFn)(itemToBeFreedFromNode);
// pList and all its nodes no longer exists after the operation; its head and nodes are 
// available for future operations. O(N)
typedef void (*FREE_FN)(void* pItem);
void List_free(List* pList, FREE_FN pItemFreeFn);

// Return last item and take it out of pList. Make the new last item the current one.
// Return NULL if pList is initially empty.

#endif