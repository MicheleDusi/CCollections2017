#ifndef UNDIRECTIONALLIST_H_
#define UNDIRECTIONALLIST_H_

#include <stdbool.h>

const int EMPTY_SIZE = 0;
const char STRING_TITLE[] = "LISTA [length: %-10d]\n";
const int STRING_TITLE_LENGTH = 26;

typedef struct ulinked_list_node {
	void* data;
	struct ulinked_list_node* next;
} ulinked_list_node;

typedef struct ulinked_list ulinked_list;

ulinked_list* initList();
void insertElementFirst(ulinked_list* l, void* new_element_data);
void insertElementLast(ulinked_list* l, void* new_element_data);
void insertElementAtPosition(ulinked_list* l, void* new_element_data, int pos);
void insertAllElementsLast(ulinked_list* l, ulinked_list* elements);
void deleteFirstElement(ulinked_list* l);
void deleteLastElement(ulinked_list* l);
void deleteElementAtPosition(ulinked_list* l, int pos);
void deleteElementsByConditions(ulinked_list* l, bool (*condition)(void*));
void* getHeadContent(ulinked_list* l);
void* extractHeadContent(ulinked_list* l);
static ulinked_list_node* extractElementAtPosition(ulinked_list* l, int pos);
void* getTailContent(ulinked_list* l);
static ulinked_list_node* getTail(ulinked_list* l);
void* getElementContentAtPosition(ulinked_list* l, int pos);
static ulinked_list_node* getElementAtPosition(ulinked_list* l, int pos);
int getListSize(ulinked_list* l);
bool containsElement(ulinked_list* l, bool (*condition)(void*));
void swapTwoElements(ulinked_list* l, int pos1, int pos2);
ulinked_list* cloneOrderedList(ulinked_list* l, void* (*clone)(void*));
ulinked_list* cloneUnorderedList(ulinked_list* l, void* (*clone)(void*));
ulinked_list* concatenateTwoLists(ulinked_list* l1, ulinked_list* l2, void* (*clone)(void*));
static ulinked_list_node* getMinimumElement(ulinked_list* l, int (*compare)(void*, void*));
void* getMinimumContent(ulinked_list* l, int (*compare)(void*, void*));
static ulinked_list_node* getMaximumElement(ulinked_list* l, int (*compare)(void*, void*));
void* getMaximumContent(ulinked_list* l, int (*compare)(void*, void*));
void sortByOrder(ulinked_list* l, int (*compare)(void*, void*));
char* listToString(ulinked_list* l, char* (*toString)(void*), int max_data_length);

#endif
