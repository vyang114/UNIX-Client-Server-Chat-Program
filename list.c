#include "list.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

static Node arr_node[LIST_MAX_NUM_NODES];	// Array of 100 struct Nodes
static List arr_list[LIST_MAX_NUM_HEADS];	// Array of 10 struct Lists

static int node_queue[LIST_MAX_NUM_NODES];	// Circular queue to keep track the position of free nodes in the array
static int list_queue[LIST_MAX_NUM_HEADS];	// Circular queue to keep track the position of of free lists in the array

static int node_front = 0, list_front = 0;
static int node_rear = -1, list_rear = -1;
static int free_node = LIST_MAX_NUM_NODES, free_list = LIST_MAX_NUM_HEADS;

static bool called_once = false;	// Flag to check if it's the first time the client's calling List_create()


// Helper function using circular queue to put a node back to the node pool
static void node_enqueue(int node_pos){

	// If all the nodes are used
	if(free_node == 0){
		node_front = 0;
		node_rear = 0;
		node_queue[node_rear] = node_pos;
		free_node++;

	// If rear is at the last spot in the queue
	} else if(node_rear == LIST_MAX_NUM_NODES-1 && node_front != 0){
		node_rear = 0;
		node_queue[node_rear] = node_pos;
		free_node++;
	
	}else{
		node_rear++;
		node_queue[node_rear] = node_pos;
		free_node++;
	}
}


// Helper function using circular queue to take a node out from the node pool
static void node_dequeue(){

	node_queue[node_front] = -1;

	if(node_front == LIST_MAX_NUM_NODES-1){
		node_front = 0;
	}else{
		node_front++;
	}

	free_node--;
}


// Helper function using circular queue to put a list head back to the head pool
static void list_enqueue(int list_pos){

	// If all the lists are used
	if(free_list == 0){
		list_front = 0;
		list_rear = 0;
		list_queue[list_rear] = list_pos;
		free_list++;

	// If rear is at the last spot in the queue
	} else if(list_rear == LIST_MAX_NUM_HEADS-1 && list_front != 0){
		list_rear = 0;
		list_queue[list_rear] = list_pos;
		free_list++;
	
	}else{
		list_rear++;
		list_queue[list_rear] = list_pos;
		free_list++;
	}
}


// Makes a new, empty list, and returns its reference on success. 
List * List_create(){

	// This O(N) initialization is only called once.
	if(!called_once){
		//printf("first time calling list create\n");

		for(int i = 0; i < LIST_MAX_NUM_NODES; i++){
			node_queue[i] = i;
		}

		for(int j = 0; j < LIST_MAX_NUM_HEADS; j++){
			list_queue[j] = j;
		}

		called_once = true;
	}

	// If all the lists are used
	if(free_list < 1){

		//printf("All the lists are used.\n");
		return NULL;

	}else{

		int list_pos = list_queue[list_front];

		arr_list[list_pos].index = list_pos;
		arr_list[list_pos].item_count = 0;
		arr_list[list_pos].head = NULL;
		arr_list[list_pos].tail = NULL;
		arr_list[list_pos].current = arr_list[list_pos].head;
		arr_list[list_pos].list_state = LIST_OOB_START;

		list_queue[list_front] = -1;

		// Take a list out form the list pool
		if(list_front == LIST_MAX_NUM_HEADS-1){
			list_front = 0;
		}else{
			list_front++;
		}

		free_list--;
		
		//printf("list_front: %d   list_rear: %d\n", list_front, list_rear);
		//printf("list index: %d\n", arr_list[list_pos].index);
		//printf("number of lists left: %d\n", free_list);
		return &arr_list[list_pos];
	}

	return NULL;
}


// Returns the number of items in pList.
int List_count(List* pList){

	assert(pList != NULL);
	return pList->item_count;
}


void* List_first(List* pList){

	assert(pList != NULL);

	// If the list is empty
	if(pList->item_count == 0){
		//printf("empty list\n");
		pList->current = NULL;
		return NULL;
	}
	
	pList->current = pList->head;	// Make the first item the current item
	pList->list_state = 2;			// Change the list state to inside the list
	//printf("current: %d  head: %d  tail: %d\n", *(int*)pList->current->item, *(int*)pList->head->item, *(int*)pList->tail->item);
	return pList->head->item;
}



void* List_curr(List* pList){

	if(pList->item_count == 0 || pList->list_state == LIST_OOB_START || pList->list_state == LIST_OOB_END){
		return NULL;
	}

	return pList->current->item;
}


//adding is dequeing for node_queue
int List_add(List* pList, void* pItem){

	assert(pList != NULL);
	//printf("number of nodes left before adding: %d\n", free_node);

	// If all the nodes are used
	if(free_node < 1){

		//printf("All the nodes are used.\n");
		return LIST_FAIL;

	}else{

		int node_pos = node_queue[node_front];
		arr_node[node_pos].position = node_pos;
		arr_node[node_pos].item = pItem;
		//printf("Node pos in array: %d\n", arr_node[node_pos].position);

		// If the list is empty
		if(pList->item_count == 0){

			pList->head = &arr_node[node_pos];
			pList->tail = &arr_node[node_pos];
			arr_node[node_pos].prev = NULL;
			arr_node[node_pos].next = NULL;
			pList->current = &arr_node[node_pos];

		// If the state of the list is before the head of the list
		}else if(pList->list_state == LIST_OOB_START){

			arr_node[node_pos].next = pList->head;
			pList->head = &arr_node[node_pos];
			arr_node[node_pos].prev = NULL;
			pList->current = &arr_node[node_pos];
		
		// If state of the list is beyong the list or current is after the last item in the pList
		}else if(pList->list_state == LIST_OOB_END || pList->current == pList->tail){

			arr_node[node_pos].prev = pList->tail;
			pList->tail->next = &arr_node[node_pos];
			pList->tail = &arr_node[node_pos];
			arr_node[node_pos].next = NULL;
			pList->current = &arr_node[node_pos];

		}else{

			arr_node[node_pos].prev = pList->current;
			arr_node[node_pos].next = pList->current->next;
			arr_node[node_pos].next->prev = &arr_node[node_pos];
			arr_node[node_pos].prev->next = &arr_node[node_pos];
			pList->current = &arr_node[node_pos];
		}

		pList->list_state = 2;		// Change the list state to inside the list
		pList->item_count++;
		node_dequeue();		// Take a node out from the the node pool

		//printf("current: %d  head: %d  tail: %d\n", *(int*)pList->current->item, *(int*)pList->head->item, *(int*)pList->tail->item);
		//printf("node_front: %d   node_rear: %d\n", node_front, node_rear);

		return LIST_SUCCESS;
	}

	return LIST_FAIL;
}




int List_append(List* pList, void* pItem){

	assert(pList != NULL);
	//printf("number of nodes left before inserting: %d\n", free_node);

	// If all the nodes are used
	if(free_node < 1){

		///printf("All the nodes are used.\n");
		return LIST_FAIL;

	}else{

		int node_pos = node_queue[node_front];
		arr_node[node_pos].position = node_pos;
		arr_node[node_pos].item = pItem;
		//printf("Node pos in array: %d\n", arr_node[node_pos].position);
		
		// If the list is empty
		if(pList->item_count == 0){

			pList->head = &arr_node[node_pos];
			pList->tail = &arr_node[node_pos];
			arr_node[node_pos].prev = NULL;
			arr_node[node_pos].next = NULL;
			pList->current = &arr_node[node_pos];

			//printf("current: %d  head: %d  tail: %d\n", *(int*)pList->current->item, *(int*)pList->head->item, *(int*)pList->tail->item);
		
		}else{

			arr_node[node_pos].prev = pList->tail;
			pList->tail->next = &arr_node[node_pos];
			pList->tail = &arr_node[node_pos];
			arr_node[node_pos].next = NULL;
			pList->current = &arr_node[node_pos];
		}

		pList->list_state = 2;		// Change the list state to inside the list
		pList->item_count++;
		node_dequeue();		// Take a node out from the the node pool

		//printf("current: %d  head: %d  tail: %d\n", *(int*)pList->current->item, *(int*)pList->head->item, *(int*)pList->tail->item);
		//printf("node_front: %d   node_rear: %d\n", node_front, node_rear);
	
		return LIST_SUCCESS;
	}

	return LIST_FAIL;
}



void* List_remove(List* pList){

	assert(pList != NULL);

	// If the list is empty or no nodes are being used
	if(pList->item_count == 0 || free_node == LIST_MAX_NUM_NODES){
		//printf("the list is empty or no nodes are used, nothing to remove\n");
		pList->list_state = LIST_OOB_START;
		return NULL;
	}

	// If the state of the list is before or beyond the list
	if(pList->list_state == LIST_OOB_START || pList->list_state == LIST_OOB_END){
		return NULL;
	}

	int node_pos = pList->current->position;

	// If there is only one item in the list
	if(pList->head == pList->tail){
	
		pList->head = NULL;
		pList->tail = NULL;
		pList->list_state = LIST_OOB_END;

	// If the current item is the head of the list
	}else if(pList->current == pList->head){
		
		//printf("current node to remove: %d\n",  *(int*)pList->current->item);
		pList->current = pList->head;
		pList->head = pList->head->next;
		pList->head->prev = NULL;

		//printf("current: %d  head: %d  tail: %d\n", *(int*)pList->current->item, *(int*)pList->head->item, *(int*)pList->tail->item);

	// If the current item is the last item in the list
	}else if(pList->current == pList->tail){
		
		//printf("current node to remove: %d \n",  *(int*)pList->current->item);
		pList->tail = pList->tail->prev;
		pList->tail->next = NULL;
		pList->list_state = LIST_OOB_END;
		//printf("current: %d  head: %d  tail: %d\n", *(int*)pList->current->item, *(int*)pList->head->item, *(int*)pList->tail->item);

	}else{
		Node * temp = pList->current;
		
		//printf("current node to remove: %d\n",  *(int*)pList->current->item);
		pList->current->prev->next = pList->current->next;
		pList->current->next->prev = pList->current->prev;
		pList->current = pList->current->next;
		pList->item_count--;
		node_enqueue(node_pos);
		return temp->item;
		//printf("current: %d  head: %d  tail: %d\n", *(int*)pList->current->item, *(int*)pList->head->item, *(int*)pList->tail->item);
	}

	pList->item_count--;
	node_enqueue(node_pos);		// Put the node back to the node pool

	//printf("number of nodes left after removing: %d\n", free_node);
	return pList->current->item;
}



void List_free(List* pList, FREE_FN pItemFreeFn){

	// If the list is empty, no node to be freed
	if(pList->item_count == 0){

		int list_pos = pList->index;
		list_enqueue(list_pos);		// Put the list back to the list pool
	
	}else{

		Node * temp = pList->head;

		while(temp != NULL){

			int node_pos = temp->position;
			//printf("%d ", *(int*)temp->item);
			node_enqueue(node_pos);		// Put the node back to the node pool

			if(pItemFreeFn != NULL){
				pItemFreeFn(temp->item);
			}
	
			temp = temp->next;
		}

		int list_pos = pList->index;
		list_enqueue(list_pos);		// Put the list back to the list pool
	}

	pList->head = NULL;
	pList->tail = NULL;
	pList->current = NULL;
	//printf("Inside list_free   free nodes after list_free: %d\n", free_node);
}


