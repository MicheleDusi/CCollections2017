#ifndef ARRAYLIST_H_
	#include "ArrayList.h"
#endif

#ifndef UNIDIRECTIONALLIST_H_
	#include "UnidirectionalList.h"
#endif

/* Future implementation:
#ifndef UNIDIRECTIONALLIST_H_
	#include "BidirectionalList.h"
#endif
*/

#ifndef LIST_H_
#define LIST_H_

typedef enum l_type {ul, bl, al} l_type;

typedef struct list {
	l_type type;
	/**
	 * 'ul' -> Unidirectional Linked List
	 * 'bl' -> Bidirectional Linked List
	 * 'al' -> Array List
	 */
	union {
		ulinked_list* ul;
		//blinked_list* bl;
		arraylist* al;
	} genericlist;
} list;

list* initList(l_type chosen_type);
void cleanList(list* l);
void purgeList(list* l);
void insertElementFirst(list* l, void* new_element_data);
void insertElementLast(list* l, void* new_element_data);
void insertElementAtPosition(list* l, void* new_element_data, int pos);
void deleteFirstElement(list* l);
void deleteLastElement(list* l);
void deleteElementAtPosition(list* l, int pos);
void deleteElementsByConditions(list* l, bool (*condition)(void*));
void* getHeadContent(list* l);
void* extractHeadContent(list* l);
void* extractElementAtPosition(list* l, int pos);
void* getTailContent(list* l);
void* getElementContentAtPosition(list* l, int pos);
int getListSize(list* l);
bool containsElement(list* l, bool (*condition)(void*));
void swapTwoElements(list* l, int pos1, int pos2);
list* cloneOrderedList(list* l, void* (*clone)(void*));
list* concatenateTwoLists(list* l11, list* l22, void* (*clone)(void*));
void* getMinimumContent(list* l, int (*compare)(void*, void*));
void* getMaximumContent(list* l, int (*compare)(void*, void*));
void sortByOrder(list* l, int (*compare)(void*, void*));
void listToString(list* l, char* destination_str, char* (*toStringFunction)(void*));

#endif
