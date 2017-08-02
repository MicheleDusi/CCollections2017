#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define INCREASING_FACTOR 2
#define INCREASING_CHECK 0.75
#define DECREASING_FACTOR 0.5
#define DECREASING_CHECK 0.25
#define DEFAULT_CAPACITY 10
#define MINIMUM_CAPACITY DEFAULT_CAPACITY * DECREASING_FACTOR


const char STRING_TITLE[] = "LISTA [length: %-10d]\n";
const int STRING_TITLE_LENGTH = 26;

typedef struct arraylist {
	int size;
	int capacity;
	void** array;
} arraylist;


/**
 * Inizializzazione di una lista vuota con capacità iniziale personalizzata.
 */
arraylist* initArrayListWithCapacity(int cap) {
	arraylist* new_list = malloc(sizeof(arraylist));
	if (!new_list) {
		// TODO Error
	}
	new_list->size = 0;
	new_list->capacity = cap > MINIMUM_CAPACITY ? cap : MINIMUM_CAPACITY;
	void* new_array[new_list->capacity];
	new_list->array = new_array;
	return new_list;
}

/** 
 * Inizializzazione della lista vuota con capacità iniziale di default.
 */
arraylist* initArrayList() {
	return initArrayListWithCapacity(DEFAULT_CAPACITY);
}

/**
 * Controlla la dimensione dell'arraylist dopo che è stata effettuata un'aggiunta di un elemento.
 * In caso la capacità debba essere aumentata, viene re-allocato l'array in una porzione di memoria maggiore.
 */
static void checkAndIncreaseCapacity(arraylist* l) {
	if (l->size > l->capacity * INCREASING_CHECK) {
		int new_capacity = (int)(l->capacity * INCREASING_FACTOR);
		l->array = realloc(l->array, new_capacity * sizeof(void*));
		l->capacity = new_capacity;
	}
}

/**
 * Controlla la dimensione dell'arraylist dopo che è stata effettuata una rimozione.
 * In caso la capacità possa essere ridotta, viene re-allocato l'array in una porzione di memoria minore.
 */
static void checkAndDecreaseCapacity(arraylist* l) {
	if (l->size < l->capacity * DECREASING_CHECK
			&& l->capacity > MINIMUM_CAPACITY) {
		int new_capacity = (int)(l->capacity * DECREASING_FACTOR);
		l->array = realloc(l->array, new_capacity * sizeof(void*));
		l->capacity = new_capacity;
	}
}

/**
 * Controlla che la posizione inserita sia ammessa nella lista.
 */
static bool checkPositionValidity(arraylist* l, int pos) {
	return pos >= 0 && pos < l->size;
}

/** 
 * Inserimento di un elemento in testa alla lista.
 */
void insertElementFirst(arraylist* l, void* new_element_data) {
	for (int i = l->size; i > 0; i--) {
		l->array[i] = l->array[i - 1];
	}
	l->array[0] = new_element_data;
	l->size++;
	checkAndIncreaseCapacity(l);
}

/**
 * Inserimento di un elemento in coda alla lista.
 */
void insertElementLast(arraylist* l, void* new_element_data) {
	l->array[l->size] = new_element_data;
	l->size++;
	checkAndIncreaseCapacity(l);
}

void insertElementAtPosition(arraylist* l, void* new_element_data, int pos) {
	if (checkPositionValidity(l, pos)) {
		for (int i = l->size; i > pos; i--) {
			l->array[i] = l->array[i - 1];
		}
		l->array[pos] = new_element_data;
		l->size++;
		checkAndIncreaseCapacity(l);
	} else {
		// TODO errore
	}
}

void deleteFirstElement(arraylist* l) {
	for (int i = 0; i < l->size - 1; i++) {
		l->array[i] = l->array[i + 1];
	}
	l->size--;
	checkAndDecreaseCapacity(l);
}

void deleteLastElement(arraylist* l) {
	l->size--;
	checkAndDecreaseCapacity(l);
}

/**
 * Rimuove un elemento alla posizione desiderata.
 */
void deleteElementAtPosition(arraylist* l, int pos) {
	if (checkPositionValidity(l, pos)) {
		for (int i = pos; i < l->size - 1; i++) {
			l->array[i] = l->array[i + 1];
		}
		l->size--;
		checkAndDecreaseCapacity(l);
	}
}

/**
 * Rimuove tutti gli elementi che soddisfano una data condizione.
 */
void deleteElementsByConditions(arraylist* l, bool (*condition)(void*)) {
	int deleted_shift = 0; // Rappresenta sia il numero di elementi eliminati, sia il numero di spostamenti necessari per "ricompattare" l'array.
	for (int i = 0; i < l->size; i++) {
		if (condition(l->array[i])) {
			deleted_shift++;
			l->size--;
		}
		l->array[i] = l->array[i + deleted_shift];
	}
	checkAndDecreaseCapacity(l);
}

/**
 * Restituisce il contenuto del primo elemento della lista.
 */
void* getHeadContent(arraylist* l) {
	return l->array[0];
}

/**
 * Restituisce il contenuto del primo elemento.
 */
void* extractHeadContent(arraylist* l) {
	void* aux = l->array[0];
	l->size--;
	for (int i = 0; i < l->size; i++) {
		l->array[i] = l->array[i + 1];
	}
	checkAndDecreaseCapacity(l);
	return aux;
}

/**
 * Estrae un elemento alla posizione desiderata, lo cancella dalla lista e lo restituisce come puntatore.
 * Il primo elemento della lista ha posizione "0".
 */
void* extractElementAtPosition(arraylist* l, int pos) {
	if (checkPositionValidity(l, pos)) {
		void* aux = l->array[pos];
		l->size--;
		for (int i = pos; i < l->size; i++) {
			l->array[i] = l->array[i + 1];
		}
		checkAndDecreaseCapacity(l);
		return aux;
	} else {
		return NULL;
	}
}

/**
 * Restituisce il contenuto dell'ultimo elemento della lista.
 * Se la lista è vuota, restituisce NULL.
 */
void* getTailContent(arraylist* l) {
	if (l->size > 0)
		return l->array[l->size - 1];
	return NULL;
}

/**
 * Restituisce il contentuto di un elemento alla posizione desiderata.
 */
void* getElementContentAtPosition(arraylist* l, int pos) {
	if (checkPositionValidity(l, pos))
		return l->array[pos];
	return NULL;
}

/**
 * Restituisce la quantità di elementi presenti nella lista.
 */
int getListSize(arraylist* l) {
	return l->size;
}

/**
 * Verifica che all'interno della lista sia presente almeno un elemento che soddisfi una data condizione.
 */
bool containsElement(arraylist* l, bool (*condition)(void*)) {
	for (int i = 0; i < l->size; i++) {
		if (condition(l->array[i])) {
			return true;
		}
	}
	return false;
}

/**
 * Scambia di posto due elementi della lista, date le loro posizioni.
 */
void swapTwoElements(arraylist* l, int pos1, int pos2) {
	if (checkPositionValidity(l, pos1) && checkPositionValidity(l, pos2)) {
		void* aux = l->array[pos1];
		l->array[pos1] = l->array[pos2];
		l->array[pos2] = aux;
	} else {
		// TODO Errore
	}
}

/**
 * Clona una lista, data in ingresso una funzione per la clonazione del contenuto di un elemento.
 * Garantisce il mantenimento dell'ordine durante il processo.
 */
arraylist* cloneOrderedList(arraylist* l, void* (*clone)(void*)) {
	// Inizializzo la nuova lista
	arraylist* new_list = initArrayListWithCapacity(l->capacity);
	// Clonazione
	for (int i = 0; i < l->size; i++) {
		new_list->array[i] = clone(l->array[i]);
	}
	new_list->size = l->size;
	// Fine
	return new_list;
}

/**
 * Unisce gli elementi di due liste in un'unica nuova lista.
 * Le liste originali <b>NON</b> vengono modificate.
 * E' possibile personalizzare il processo di clonazione attraverso la funzione <i>clone</i> passata come parametro.
 */
arraylist* concatenateTwoLists(arraylist* l1, arraylist* l2, void* (*clone)(void*)) {
	// Inizializzo la lista vuota pronta ad accogliere gli elementi clonati
	arraylist* new_list = initArrayListWithCapacity(l1->capacity + l2->capacity);
	new_list->size = l1->size + l2->size;
	// Clono gli elementi
	int i;
	for (i = 0; i < l1->size; i++) {
		new_list->array[i] = clone(l1->array[i]); // Clono la prima lista
	}
	for (i = l1->size; i < new_list->size; i++) {
		new_list->array[i] = clone(l2->array[i - l1->size]); // Clono la seconda lista
	}
	// Fine
	return new_list;
}

/**
 * Data una lista e una relazione d'ordine definita sui suoi elementi, la funzione
 * restitutisce l'elemento "minimo" (sotto forma di puntatore ad esso).
 * La relazione deve essere implementata come una funzione che prende in ingresso il contenuto di due nodi, 
 * e li confronta restituendo:
 * - un numero negativo se il primo dato è "minore" del secondo (stando alla relazione).
 * - 0 se i due dati sono considerati uguali dalla relazione d'ordine.
 * - un numero positivo se il primo dato è "maggiore" del secondo (stando alla relazione).
 */
void* getMinimumContent(arraylist* l, int (*compare)(void*, void*)) {
	if (l->size > 0) {
		void* minimum = l->array[0];
		for (int i = 1; i < l->size; i++) {
			if (compare(l->array[i], minimum) < 0) {
				minimum = l->array[i];
			}
		}
		return minimum;
	} else {
		return NULL;
	}
}

/**
 * Data una lista e una relazione d'ordine definita sui suoi elementi, la funzione
 * restitutisce l'elemento "massimo".
 * La relazione deve essere implementata come una funzione che prende in ingresso il contenuto di due nodi, 
 * e li confronta restituendo:
 * - un numero negativo se il primo dato è "minore" del secondo (stando alla relazione).
 * - 0 se i due dati sono considerati uguali dalla relazione d'ordine.
 * - un numero positivo se il primo dato è "maggiore" del secondo (stando alla relazione).
 */
void* getMaximumContent(arraylist* l, int (*compare)(void*, void*)) {
	if (l->size > 0) {
		void* maximum = l->array[0];
		for (int i = 1; i < l->size; i++) {
			if (compare(l->array[i], maximum) > 0) {
				maximum = l->array[i];
			}
		}
		return maximum;
	} else {
		return NULL;
	}
}

/**
 * Ordina una lista in modo <i>crescente</i> secondo una relazione d'ordine definita dall'utente e passata come parametro.
 * La relazione deve essere implementata come una funzione che prende in ingresso il contenuto di due nodi, 
 * e li confronta restituendo:
 * - un numero negativo se il primo dato è "minore" del secondo (stando alla relazione).
 * - 0 se i due dati sono considerati uguali dalla relazione d'ordine.
 * - un numero positivo se il primo dato è "maggiore" del secondo (stando alla relazione).
 */
void sortByOrder(arraylist* l, int (*compare)(void*, void*)) {
	// TODODODODODODODODODODODODODODODODOD
}

/**
 * Restituisce una stringa che rappresenta gli elementi contenuti all'interno della lista.
 * Per rappresentare ogni singolo elemento viene passata come parametro la funzione che converte in stringa il contenuto
 * di un elemento. E' necessario fornire anche la dimensione massima della stringa di un singolo elemento.
 */
char* listToString(arraylist* l, char* (*toString)(void*), int max_data_length) {
	char* s;
	// STRING_TITLE_LENGTH + max_data_length * l->size
	s = malloc(sizeof(STRING_TITLE_LENGTH + max_data_length * l->size));
	sprintf(s, STRING_TITLE, l->size);
	for (int i = 0; i < l->size; i++) {
		strcat(s, toString(l->array[i]));
	}
	return s;
}
