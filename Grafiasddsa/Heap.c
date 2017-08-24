#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#include "Heap.h"

#define ARRAY_TO_HEAP(x) (x + 1)
#define HEAP_TO_ARRAY(x) (x - 1)

#ifndef MEMORY_ERROR
#	define MEMORY_ERROR printf("Error: Cannot allocate memory.\n"); exit(1)
#endif

#ifndef EMPTY_SIZE
#	define EMPTY_SIZE 0
#	define EMPTY_SIZE_ERROR(instr) printf("Error: Cannot execute \"%s\" function on empty heap.", instr )
#endif

#ifndef STRING_FORMAT_HEAP
	#define STRING_FORMAT_HEAP
	#define SPACING "----"
	#define NODE_STRING_LENGTH 30
#endif

/**
 * Libreria che permette la creazione e gestione di un max-heap binario, implementato come arraylist.
 * 
 * @author Michele Dusi <michele.dusi.it@ieee.org>
 * 
 * <i>NOTA:</i> l'heap in questione è formalizzato unicamente come max-heap.
 * Questo significa che il nodo padre ha valore maggiore del nodo figlio. Per ottenere il min-heap
 * corrispondente, è sufficiente invertire l'output della funzione di comparazione.
 * 
 * <i>NOTA:</i> La numerazione degli elementi dell'Heap inizia da 1. Questo è dovuto alla facilità di implementazione
 * che si può ottenere adottando questo particolare accorgimento.
 * Dato un nodo [i], infatti, il nodo padre avrà indice (int)[i / 2], i nodi figli [i * 2] e [i * 2 + 1].
 */

/*
typedef struct binaryheap {
	arraylist* al;
	int (*comparefunction)(void*, void*);
} binaryheap;
*/

// Initializing Heap

/**
 * Inizializza un max-heap binario e ne restituisce un puntatore.
 * Notare che la numerazione dell'heap comincia (con la radice) dal valore 1.
 */
binaryheap* bh_initHeap(int (*compare)(void*, void*)) {
	binaryheap* new_binaryheap = malloc(sizeof(binaryheap));
	if (!new_binaryheap) {
		MEMORY_ERROR;
	}
	new_binaryheap->al = al_initList();
	new_binaryheap->comparefunction = compare;
	return new_binaryheap;
}

// Size

/**
 * Restituisce il numero di elementi contenuti all'interno dell'Heap.
 */
int bh_getHeapSize(binaryheap* h) {
	return h->al->size;
}

// Cancelling Heap

/**
 * Elimina l'heap, ripulendo la memoria occupata dalla lista interna.
 * 
 * <i>NOTA</i>: Non elimina gli oggetti contenuti in esso; questo significa che sarà compito dell'utilizzatore
 * richiamare la funzione "free()" sulla porzione di memoria che li contiene. In alternativa, è possibile chamare
 * "bh_purgeHeap()" che ripulisce anche la memoria occupata dagli elementi.
 */
void bh_deleteHeap(binaryheap* h) {
	al_deleteList(h->al);
	free(h);
}

/**
 * Elimina l'heap, ripulendo la memoria occupata dalla lista interna e dagli oggetti contenuti in essa.
 * Questo significa che tutti gli elementi che compongono l'heap verranno irrimediabilmente persi, e non
 * potranno essere recuperati.
 */
void bh_purgeHeap(binaryheap* h) {
	al_purgeList(h->al);
	free(h);
}

// Inserting Elements

/**
 * Inserisce un elemento all'interno dell'heap.
 * Non è possibile definire la posizione personalizzata in cui inserirlo poiché -per stessa implementazione dell'heap-
 * è la struttura stessa a definire la posizione di un elemento.
 */
void bh_insertElement(binaryheap* h, void* new_element) {
	al_insertLastElement(h->al, new_element);
	int index = al_getListSize(h->al);
	while (index > 1) {
		if (h->comparefunction(new_element, h->al->array[HEAP_TO_ARRAY(index / 2)])) {
			al_swapTwoElements(h->al, HEAP_TO_ARRAY(index / 2), HEAP_TO_ARRAY(index));
			index /= 2;
		}
	}
}

void bh_insertAllElements(binaryheap* h, arraylist* elements); /// TODO

// Deleting Elements

/**
 * Cancella la radice dell'heap e risistema i nodi restanti.
 */
void bh_deleteRootElement(binaryheap* h) {
	bh_deleteElementAtPosition(h, 0);
}

/**
 * Cancella un elemento dell'Heap, dato come parametro il suo riferimento.
 */
void bh_deleteElement(binaryheap* h, void* element_to_delete) {
	int pos = bh_getPositionOfElement(h, element_to_delete);
	bh_deleteElementAtPosition(h, pos);
}

/**
 * Data in ingresso la posizione <i>relativa all'heap</i> dell'elemento da eliminare, lo elimina dalla struttura.
 * L'elemento non viene eliminato dalla memoria ed è ancora raggiungibile se esistono puntatori ad esso.
 */
void bh_deleteElementAtPosition(binaryheap* h, int pos) {
	while (HEAP_TO_ARRAY(pos) < h->al->size) {
		// Controllo che non sia alla fine (aka: che abbia figli)
		if (pos * 2 + 1 > h->al->size) {
			// Non ho figli
			al_swapTwoElements(h->al, HEAP_TO_ARRAY(pos), HEAP_TO_ARRAY(h->al->size));	// L'elemento da eliminare va scambiato con l'elemento in fondo
			while (h->comparefunction(
					h->al->array[HEAP_TO_ARRAY(pos)],
					h->al->array[HEAP_TO_ARRAY(pos / 2)]) > 0) {
				al_swapTwoElements(h->al, HEAP_TO_ARRAY(pos), HEAP_TO_ARRAY(pos * 2));
			}
			break;
			
		} else if (pos * 2 + 1 == h->al->size) {
			// Ho un figlio solo
			// E' il caso più semplice, perchè non devo aggiornare ulteriormente i collegamenti.
			al_swapTwoElements(h->al, HEAP_TO_ARRAY(pos), HEAP_TO_ARRAY(pos * 2));
			break;
			
		} else {
			// Controllo i figli
			if (h->comparefunction(
					h->al->array[HEAP_TO_ARRAY(pos * 2)],				// Figlio SX (1)
					h->al->array[HEAP_TO_ARRAY(pos * 2 + 1)]) > 0) {	// Figlio DX (2)
				// Il primo figlio è maggiore
				al_swapTwoElements(h->al, HEAP_TO_ARRAY(pos), HEAP_TO_ARRAY(pos * 2));
				pos = pos * 2;
			} else {
				// Il secondo figlio è maggiore
				al_swapTwoElements(h->al, HEAP_TO_ARRAY(pos), HEAP_TO_ARRAY(pos * 2 + 1));
				pos = pos * 2 + 1;
			}
		}
	}
	al_deleteLastElement(h->al);
}

/**
 * Cancella dall'heap tutti gli elementi che soddisfano una data condizione, assicurandosi
 * che la struttura rimanga costantemente ben formattata (i.e. fa in modo che non venga infranta la
 * proprietà di heap).
 */
void bh_deleteElementsByConditions(binaryheap* h, bool (*condition)(void*)) {
	/// TODO better implementation?
	for (int i = h->al->size; i > 0; i--) {		// Scorro a partire dal fondo perché questo mi permette di eseguire un numero inferiore di istruzioni (nell'altra funzione)
		if (condition(h->al->array[HEAP_TO_ARRAY(i)])) {
			bh_deleteElementAtPosition(h, i);
		}
	}
}

// Purging Elements

void bh_purgeRootElement(binaryheap* h); /// TODO

void bh_purgeElementAtPosition(binaryheap* h, int pos); /// TODO

void bh_purgeElementsByCondition(binaryheap* h, bool (*condition)(void*)); /// TODO

// Getting Elements

void* bh_getRootElement(binaryheap* h); /// TODO

/**
 * Restituisce l'elemento dell'heap alla posizione desiderata.
 * <i>NOTA:</i> Gli indici dell'heap iniziano la numerazione dal valore 1.
 */
void* bh_getElementAtPosition(binaryheap* h, int pos) {
	return h->al->array[HEAP_TO_ARRAY(pos)];
}

arraylist* bh_getElementsByCondition(binaryheap* h, bool (*condition)(void*)); /// TODO

/**
 * Restituisce la lista interna all'heap.
 */
arraylist* bh_getListOfElements(binaryheap* h) {
	return h->al;
}

// Extracting Elements

void* bh_extractRootElement(binaryheap* h); /// TODO

/**
 * Restituisce un elemento alla posizione desiderata, eliminandolo dall'heap.
 */
void* bh_extractElementAtPosition(binaryheap* h, int pos) {
	void* aux = h->al->array[HEAP_TO_ARRAY(pos)];
	bh_deleteElementAtPosition(h, pos);
	return aux;
}

/**
 * Estrae dall'heap tutti gli elementi che soddisfano una data condizione, assicurandosi
 * che la struttura rimanga costantemente ben formattata (i.e. fa in modo che non venga infranta la
 * proprietà di heap).
 * Gli elementi estratti vengono inseriti all'interno di una lista e restituiti al termine dell'esecuzione.
 */
arraylist* bh_extractElementsByCondition(binaryheap* h, bool (*condition)(void*)) {
	arraylist* extracted = al_initList();
	for (int i = h->al->size; i > 0; i--) {		// Scorro a partire dal fondo perché questo mi permette di eseguire un nuemro inferiore di istruzioni (nell'altra funzione)
		if (condition(h->al->array[HEAP_TO_ARRAY(i)])) {
			void* elem_extracted = bh_extractElementAtPosition(h, i);
			al_insertLastElement(extracted, elem_extracted);
		}
	}
	return extracted;
}

// Searching Elements

/**
 * Restituisce la posizione di un elemento presente nell'heap.
 * Se assente, viene restituito il valore -1;
 */
int bh_getPositionOfElement(binaryheap* h, void* element_to_find) {
	return ARRAY_TO_HEAP(al_getPositionOfElement(h->al, element_to_find));
}

bool bh_containsElement(binaryheap* l, void* element_content); /// TODO

bool bh_containsElementByCondition(binaryheap* l, bool (*condition)(void*)); /// TODO

int bh_countElementsByCondition(binaryheap* l, bool (*condition)(void*)); /// TODO

// Cloning Heap

binaryheap* bh_cloneHeap(binaryheap* h, void* (*clone)(void*)); /// TODO

// Generic Operations On Elements

void bh_iterateOnElements(binaryheap* l, void (*procedure)(void*)); /// TODO

void* bh_cumulateElements(binaryheap* l, void* (*binaryOperation)(void*, void*)); /// TODO

/**
 * Restituisce l'elemento massimo dell'Heap, che per stessa definizione di max-heap si trova in testa.
 */
void* bh_getMaximumElement(binaryheap* h) {
	if (h->al->size > EMPTY_SIZE) {
		return h->al->array[0];
	} else {
		EMPTY_SIZE_ERROR("bh_getMaximumElement");
		return NULL;
	}
}

/**
 * Restituisce l'elemento minimo dell'heap, attraversandolo di padre in figlio.
 * Per ogni iterazione viene selezionato il figlio con valore inferiore.
 * L'algoritmo ha complessità log_2(n).
 */
void* bh_getMinimumElement(binaryheap* h) {
	/// TODO da controllare, non sono sicuro che funzioni.
	if (h->al->size == EMPTY_SIZE) {
		EMPTY_SIZE_ERROR("bh_getMinimumElement");
		return NULL;
	} else {
		int minimum_index = 1;
		while (minimum_index < h->al->size) {
			if (h->comparefunction(
					h->al->array[HEAP_TO_ARRAY(minimum_index * 2)],
					h->al->array[HEAP_TO_ARRAY(minimum_index * 2 + 1)]) > 0) {
				minimum_index = minimum_index * 2;
			} else {
				minimum_index = minimum_index * 2 + 1;
			}
		}
		return h->al->array[HEAP_TO_ARRAY(minimum_index)];
	}
}

// Visualizing Heap

/**
 * Funzione interna ricorsiva per la rappresentazione di una sotto-heap.
 */
static char* bh_getSubtreeString(binaryheap* h, char* (*toStringFunction)(void*), int index, int level) {
	int power = 1;
	for (int i = 0; i < level; i++) power *= 2; // Elevo 2 alla potenza level-esima.
	
	char* str = malloc ((int)(h->al->size / power) * NODE_STRING_LENGTH * sizeof(char)); // Calcolo dello spazio necessario
	if (!str) {
		MEMORY_ERROR;
	}
	// Figlio sx
	for (int i = 0; i < level; i++) {
		strcat(str, SPACING);
	}
	strcat(str, toStringFunction(h->al->array[HEAP_TO_ARRAY(index * 2)]));
	strcat(str, "\n");
	if (index * 2 <= h->al->size) {
		strcat(str, bh_getSubtreeString(h, toStringFunction, index * 2, level + 1));
	}
	// Figlio dx
	for (int i = 0; i < level; i++) {
		strcat(str, SPACING);
	}
	strcat(str, toStringFunction(h->al->array[HEAP_TO_ARRAY(index * 2 + 1)]));
	strcat(str, "\n");
	if (index * 2 + 1 <= h->al->size) {
		strcat(str, bh_getSubtreeString(h, toStringFunction, index * 2 + 1, level + 1));
	}
	return str;
	
}

/**
 * Restituisce la rappresentazione dell'Heap binario come stringa.
 */
char* bh_heapToString(binaryheap* h, char* (*toStringFunction)(void*)) {
	return bh_getSubtreeString(h, toStringFunction, 1, 0);
}





///////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// TEST

#ifndef TARGA_TESTING
#define TARGA_TESTING 

typedef struct {
	char first_letters[2];
	int three_numbers;
	char last_letters[2];
} targa;

static char getRandomChar() {
	return (char)((rand() % ('Z' - 'A')) + 'A');
}

targa* initRandomTarga() {
	targa* new = malloc(sizeof(targa));
	if (!new) {
		printf("Memory Error: cannot create new \"targa\".\n");
	}
	new->first_letters[0] = getRandomChar();
	new->first_letters[1] = getRandomChar();
	new->three_numbers = rand() % 1000;
	new->last_letters[0] = getRandomChar();
	new->last_letters[1] = getRandomChar();
	return new;
}

#define TARGA_STRING_FORMAT "[%c%c %03d %c%c]"

char* stringifyMyStruct(void* my_object) {
	targa* aux_targa = (targa*)my_object;
	char* created_string = malloc(sizeof(char) * (strlen(TARGA_STRING_FORMAT) + 1));
	if (!created_string) {
		printf("Memory Error: cannot create new \"targa\".\n");
	}
	sprintf(created_string, TARGA_STRING_FORMAT, 
		(aux_targa)->first_letters[0],
		(aux_targa)->first_letters[1],
		(aux_targa)->three_numbers,
		(aux_targa)->last_letters[0],
		(aux_targa)->last_letters[1]);
	return created_string;
}

int compareMyStruct(void* obj1, void* obj2) {
	char* targa1 = stringifyMyStruct((targa*)obj1);
	char* targa2 = stringifyMyStruct((targa*)obj2);
	int result = strcmp(targa1, targa2);
	free(targa1);
	free(targa2);
	return result;
}

bool hasEvenNumber(void* obj) {
	targa* my_targa = (targa*) obj;
	if (my_targa->three_numbers % 2 == 0) {
		return true;
	} else {
		return false;
	}
}

void* cloneMyStruct(void* obj) {
	targa* my_targa = (targa*)obj;
	targa* new = malloc(sizeof(targa));
	if (!new) {
		printf("Memory Error: cannot create new \"targa\".\n");
	}
	new->first_letters[0] = my_targa->first_letters[0];
	new->first_letters[1] = my_targa->first_letters[1];
	new->three_numbers = my_targa->three_numbers;
	new->last_letters[0] = my_targa->last_letters[0];
	new->last_letters[1] = my_targa->last_letters[1];
	return new;
}

void printMyStruct(void* obj) {
	char* str = stringifyMyStruct(obj);
	printf("%s\n", str);
	free(str);
}

void* mediateTwoTarghe(void* obj1, void* obj2) {
	targa* result = malloc(sizeof(targa));
	if (!result) {
		printf("Memory Error: cannot create new \"targa\".\n");
	}
	targa* t1 = (targa*) obj1;
	targa* t2 = (targa*) obj2;
	result->first_letters[0] = (t1->first_letters[0] + t2->first_letters[0]) / 2;
	result->first_letters[1] = (t1->first_letters[1] + t2->first_letters[1]) / 2;
	result->three_numbers = (t1->three_numbers + t2->three_numbers) / 2;
	result->last_letters[0] = (t1->last_letters[0] + t2->last_letters[0]) / 2;
	result->last_letters[1] = (t1->last_letters[1] + t2->last_letters[1]) / 2;
	
	return result;
}

typedef struct {
	int value;
} number;

void* mapToInteger(void* obj) {
	number* num = malloc(sizeof(number));
	targa* trg = (targa*)obj;
	num->value = trg->three_numbers;
	free(trg);
	return num;
}

char* stringifyMyNumber(void* obj) {
	char* str = malloc(sizeof(char) * 6);
	number* num = (number*)obj;
	sprintf(str, "%5d", num->value);
	return str;
}

#endif

///////////////////////// MAIN //////////////////////////////

int main(void) {
	
	binaryheap* hh = bh_initHeap(compareMyStruct);
	char* str1;
	
	srand(time(NULL));
	
	int dim = 10;
	
	for (int i = 0; i < dim; i++) {
		bh_insertElement(hh, initRandomTarga());
	}
	
	str1 = al_listToString(hh->al, stringifyMyStruct);
	printf("%s", str1);
	free(str1);
	
	// Cleaning
	bh_purgeHeap(hh);
	
	return 0;
	
}

