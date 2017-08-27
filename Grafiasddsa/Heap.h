#ifndef HEAP_H_
#define HEAP_H_

#include "ArrayList.h"

typedef struct binaryheap {
	arraylist* al;
	int (*comparefunction)(void*, void*);
} binaryheap;

// Initializing Heap
binaryheap* bh_initHeap(int (*compare)(void*, void*)); // OK

// Size
int bh_getHeapSize(binaryheap* h); // OK

// Cancelling Heap
void bh_deleteHeap(binaryheap* h); // OK
void bh_purgeHeap(binaryheap* h); // OK

// Inserting Elements
void bh_insertElement(binaryheap* h, void* new_element); // OK
void bh_insertAllElements(binaryheap* h, arraylist* elements); // OK

// Deleting Elements
void bh_deleteRootElement(binaryheap* h); // OK
void bh_deleteElement(binaryheap* h, void* element_to_delete);
void bh_deleteElementAtPosition(binaryheap* h, int pos); // OK
void bh_deleteElementsByCondition(binaryheap* h, bool (*condition)(void*));

// Purging Elements
void bh_purgeRootElement(binaryheap* h); // OK
void bh_purgeElementAtPosition(binaryheap* h, int pos); // OK
void bh_purgeElementsByCondition(binaryheap* h, bool (*condition)(void*)); /// TODO

// Getting Elements
void* bh_getRootElement(binaryheap* h); // OK
void* bh_getElementAtPosition(binaryheap* h, int pos); // OK
arraylist* bh_getElementsByCondition(binaryheap* h, bool (*condition)(void*)); /// TODO
arraylist* bh_getListOfElements(binaryheap* h);

// Extracting Elements
void* bh_extractRootElement(binaryheap* h); // OK
void* bh_extractElementAtPosition(binaryheap* h, int pos); // OK
arraylist* bh_extractElementsByCondition(binaryheap* h, bool (*condition)(void*));

// Searching Elements
bool bh_containsElement(binaryheap* h, void* element_content); // OK
bool bh_containsElementByCondition(binaryheap* h, bool (*condition)(void*)); /// TODO
int bh_countElementsByCondition(binaryheap* h, bool (*condition)(void*)); /// TODO
int bh_getPositionOfElement(binaryheap* h, void* element_to_find);

// Cloning Heap
binaryheap* bh_cloneHeap(binaryheap* h, void* (*clone)(void*)); /// TODO

// Generic Operations On Elements
void bl_iterateOnElements(binaryheap* h, void (*procedure)(void*)); /// TODO
void* bl_cumulateElements(binaryheap* h, void* (*binaryOperation)(void*, void*)); /// TODO
void* bh_getMaximumElement(binaryheap* h);
void* bh_getMinimumElement(binaryheap* h);

// Visualizing Heap
char* bh_heapToString(binaryheap* h, char* (*toStringFunction)(void*));

#endif
