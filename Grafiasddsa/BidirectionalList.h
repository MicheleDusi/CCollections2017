#ifndef BIDIRECTIONALLIST_H_
#define BIDIRECTIONALLIST_H_

#include <stdbool.h>

#ifndef SIZE_OPERATIONS
#	define SIZE_OPERATIONS
#	define INT_SWAP(a, b) a = a + b; b = a - b; a = a - b
#	define EMPTY_SIZE 0
#endif

#ifndef STRING_FORMATTING
#	define STRING_FORMATTING
	const char* STRING_TITLE = "LISTA [size: %d]\n";
#endif

typedef struct blinked_list_node {
	void* data;
	struct blinked_list_node* next;
	struct blinked_list_node* prev;
} blinked_list_node;

typedef struct blinked_list {
	int size;
	blinked_list_node* head;
	blinked_list_node* tail;	
} blinked_list;

// Initializing List
blinked_list* bl_initList(); // OK

// Size
int bl_getListSize(blinked_list* l); 

// Cancelling List
void bl_deleteList(blinked_list* l); // OK
void bl_purgeList(blinked_list* l); // OK

// Inserting Elements
void bl_insertFirstElement(blinked_list* l, void* new_element_data); 
void bl_insertLastElement(blinked_list* l, void* new_element_data); 
void bl_insertElementAtPosition(blinked_list* l, void* new_element_data, int pos); 
void bl_insertAllElementsLast(blinked_list* l, blinked_list* elements); 

// Deleting Elements
void bl_deleteFirstElement(blinked_list* l); 
void bl_deleteLastElement(blinked_list* l); 
void bl_deleteElementAtPosition(blinked_list* l, int pos); 
void bl_deleteElementsByCondition(blinked_list* l, bool (*condition)(void*)); 

// Purging Elements
void bl_purgeFirstElement(blinked_list* l); // OK
void bl_purgeLastElement(blinked_list* l);  
void bl_purgeElementAtPosition(blinked_list* l, int pos);  
void bl_purgeElementsByCondition(blinked_list* l, bool (*condition)(void*));  

// Getting Elements
void* bl_getFirstElement(blinked_list* l); 
void* bl_getLastElement(blinked_list* l); 
void* bl_getElementAtPosition(blinked_list* l, int pos); 
blinked_list* bl_getElementsByCondition(blinked_list* l, bool (*condition)(void*));  
blinked_list* bl_getSubList(blinked_list* l, int start_pos, int end_pos); 

// Extracting Elements
void* bl_extractFirstElement(blinked_list* l); 
void* bl_extractLastElement(blinked_list* l); 
void* bl_extractElementAtPosition(blinked_list* l, int pos); 
blinked_list* bl_extractElementsByCondition(blinked_list* l, bool (*condition)(void*));  

// Searching Elements
bool bl_containsElement(blinked_list* l, void* element_content); 
bool bl_containsElementByCondition(blinked_list* l, bool (*condition)(void*)); 
int bl_countElementsByCondition(blinked_list* l, bool (*condition)(void*)); 
int bl_getElementPosition(blinked_list* l, void* element_content); 

// Cloning and Managing List
blinked_list* bl_cloneOrderedList(blinked_list* l, void* (*clone)(void*)); 
blinked_list* bl_cloneSubList(blinked_list* l, int start_pos, int end_pos, void* (*clone)(void*)); 
blinked_list* bl_concatenateTwoLists(blinked_list* l1, blinked_list* l2, void* (*clone)(void*)); 

// Sorting List
void bl_swapTwoElements(blinked_list* l, int pos1, int pos2); 
void bl_sortByOrder(blinked_list* l, int (*compare)(void*, void*)); 
void* bl_getMinimumElement(blinked_list* l, int (*compare)(void*, void*)); 
void* bl_getMaximumElement(blinked_list* l, int (*compare)(void*, void*)); 

// Visualizing List
char* bl_listToString(blinked_list* l, char* (*toStringFunction)(void*)); 

#endif
