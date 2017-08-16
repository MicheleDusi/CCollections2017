#ifndef ARRAYLIST_H_
#define ARRAYLIST_H_

#include <stdbool.h>

typedef struct arraylist {
	int size;
	int capacity;
	void** array;
} arraylist;

// Initializing List
arraylist* al_initListWithCapacity(int cap); // OK
arraylist* al_initList(); // OK

// Size
int al_getListSize(arraylist* l); // OK

// Cancelling List
void al_cleanList(arraylist* l); // OK
void al_purgeList(arraylist* l); // OK

// Inserting Elements
void al_insertFirstElement(arraylist* l, void* new_element_data); // OK
void al_insertLastElement(arraylist* l, void* new_element_data); // OK
void al_insertElementAtPosition(arraylist* l, void* new_element_data, int pos); // OK
void al_insertAllElementsLast(arraylist* l, arraylist* elements); // OK

// Deleting Elements
void al_deleteFirstElement(arraylist* l); // OK
void al_deleteLastElement(arraylist* l); // OK
void al_deleteElementAtPosition(arraylist* l, int pos); // OK
void al_deleteElementsByCondition(arraylist* l, bool (*condition)(void*)); // OK

// Purging Elements
void al_purgeFirstElement(arraylist* l); // OK
void al_purgeLastElement(arraylist* l); // OK
void al_purgeElementAtPosition(arraylist* l, int pos); // OK
void al_purgeElementsByCondition(arraylist* l, bool (*condition)(void*)); // OK

// Getting Elements
void* al_getFirstElement(arraylist* l); // OK
void* al_getLastElement(arraylist* l); // OK
void* al_getElementAtPosition(arraylist* l, int pos); // OK
arraylist* al_getElementsByCondition(arraylist* l, bool (*condition)(void*)); // OK
arraylist* al_getSubList(arraylist* l, int start_pos, int end_pos); // OK

// Extracting Elements
void* al_extractFirstElement(arraylist* l); // OK
void* al_extractLastElement(arraylist* l); // OK
void* al_extractElementAtPosition(arraylist* l, int pos); // OK
arraylist* al_extractElementsByCondition(arraylist* l, bool (*condition)(void*)); // OK

// Searching Elements
bool al_containsElement(arraylist* l, void* element_content); // OK
bool al_containsElementByCondition(arraylist* l, bool (*condition)(void*)); // OK
int al_countElementsByCondition(arraylist* l, bool (*condition)(void*)); // OK
int al_getElementPosition(arraylist* l, void* element_content); // OK

// Cloning and Managing List
arraylist* al_cloneOrderedList(arraylist* l, void* (*clone)(void*)); // OK
arraylist* al_cloneSubList(arraylist* l, int start_pos, int end_pos, void* (*clone)(void*)); // OK
arraylist* al_concatenateTwoLists(arraylist* l1, arraylist* l2, void* (*clone)(void*)); // OK

// Sorting List
void al_swapTwoElements(arraylist* l, int pos1, int pos2); // OK
void al_sortByOrder(arraylist* l, int (*compare)(void*, void*)); // OK
void* al_getMinimumContent(arraylist* l, int (*compare)(void*, void*)); // OK
void* al_getMaximumContent(arraylist* l, int (*compare)(void*, void*)); // OK

// Visualizing List
char* al_listToString(arraylist* l, char* (*toStringFunction)(void*)); // OK

#endif
