#ifndef UNDIRECTIONALLIST_H_
#define UNDIRECTIONALLIST_H_

#include <stdbool.h>

const int EMPTY_SIZE = 0;
const char STRING_TITLE[] = "LISTA [length: %-10d]\n";
const int STRING_TITLE_LENGTH = 26;

typedef struct list_node {
	void* data;
	struct list_node* next;
} list_node;

typedef struct list list;

list* initList();
void insertElementFirst(list* l, void* new_element_data);
void insertElementLast(list* l, void* new_element_data);
void insertElementAtPosition(list* l, void* new_element_data, int pos);
void insertAllElementsLast(list* l, list* elements);
void deleteFirstElement(list* l);
void deleteLastElement(list* l);
void deleteElementAtPosition(list* l, int pos);
void deleteElementsByConditions(list* l, bool (*condition)(void*));
void* getHeadContent(list* l);
void* extractHeadContent(list* l);
static list_node* extractElementAtPosition(list* l, int pos);
void* getTailContent(list* l);
static list_node* getTail(list* l);
void* getElementContentAtPosition(list* l, int pos);
static list_node* getElementAtPosition(list* l, int pos);
int getListSize(list* l);
bool containsElement(list* l, bool (*condition)(void*));
void swapTwoElements(list* l, int pos1, int pos2);
list* cloneOrderedList(list* l, void* (*clone)(void*));
list* cloneUnorderedList(list* l, void* (*clone)(void*));
list* concatenateTwoLists(list* l1, list* l2, void* (*clone)(void*));
static list_node* getMinimumElement(list* l, int (*compare)(void*, void*));
void* getMinimumContent(list* l, int (*compare)(void*, void*));
static list_node* getMaximumElement(list* l, int (*compare)(void*, void*));
void* getMaximumContent(list* l, int (*compare)(void*, void*));
void sortByOrder(list* l, int (*compare)(void*, void*));
char* listToString(list* l, char* (*toString)(void*), int max_data_length);

#endif
