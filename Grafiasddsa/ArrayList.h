#ifndef ARRAYLIST_H_
#define ARRAYLIST_H_

#include <stdbool.h>

typedef struct arraylist {
	int size;
	int capacity;
	void** array;
} arraylist;

// Initializing List
arraylist* al_initListWithCapacity(int cap);
arraylist* al_initList(); // OK

// Size
int al_getListSize(arraylist* l);

// Cancelling List
void al_cleanList(arraylist* l); // OK
void al_purgeList(arraylist* l); // OK

// Inserting Elements
void al_insertFirstElement(arraylist* l, void* new_element_data);
void al_insertLastElement(arraylist* l, void* new_element_data); // OK
void al_insertElementAtPosition(arraylist* l, void* new_element_data, int pos);

// Deleting Elements
void al_deleteFirstElement(arraylist* l);
void al_deleteLastElement(arraylist* l);
void al_deleteElementAtPosition(arraylist* l, int pos);
void al_deleteElementsByCondition(arraylist* l, bool (*condition)(void*));

// Purging Elements
void al_purgeFirstElement(arraylist* l); /// TODO
void al_purgeLastElement(arraylist* l); /// TODO
void al_purgeElementAtPosition(arraylist* l, int pos); /// TODO
void al_purgeElementsByCondition(arraylist* l, bool (*condition)(void*)); // NEW

// Getting Elements
void* al_getFirstElement(arraylist* l);
void* al_getLastElement(arraylist* l);
void* al_getElementAtPosition(arraylist* l, int pos);
arraylist* al_getElementsByCondition(arraylist* l, bool (*condition)(void*)); /// TODO
arraylist* al_getSubList(arraylist* l, int start_pos, int end_pos);

// Extracting Elements
void* al_extractFirstElement(arraylist* l);
void* al_extractLastElement(arraylist* l); /// TODO
void* al_extractElementAtPosition(arraylist* l, int pos);
arraylist* al_extractElementsByCondition(arraylist* l, bool (*condition)(void*)); // NEW

// Searching Elements
bool al_containsElement(arraylist* l, void* element_content); /// TODO
bool al_containsElementByCondition(arraylist* l, bool (*condition)(void*));
int al_countElementsByCondition(arraylist* l, bool (*condition)(void*)); /// TODO
int al_getElementPosition(arraylist* l, void* element_content);

// Cloning and Managing List
arraylist* al_cloneOrderedList(arraylist* l, void* (*clone)(void*));
arraylist* al_cloneSubList(arraylist* l, int start_pos, int end_pos, void* (*clone)(void*));
arraylist* al_concatenateTwoLists(arraylist* l1, arraylist* l2, void* (*clone)(void*));

// Sorting List
void al_swapTwoElements(arraylist* l, int pos1, int pos2);
void al_sortByOrder(arraylist* l, int (*compare)(void*, void*)); // OK
void* al_getMinimumContent(arraylist* l, int (*compare)(void*, void*));
void* al_getMaximumContent(arraylist* l, int (*compare)(void*, void*));

// Visualizing List
char* al_listToString(arraylist* l, char* (*toStringFunction)(void*)); // OK

#endif
