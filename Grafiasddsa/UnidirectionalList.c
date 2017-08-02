#include "UnidirectionalList.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SWAP(a, b) a = a + b; b = a - b; a = a - b

/**
 * Libreria che permette la gestione di una lista.
 * 
 * @author Michele Dusi <michele.dusi.it@ieee.org>
 * 
 */

struct ulinked_list {
	int size;
	ulinked_list_node* head;
};

/** 
 * Inizializzazione della lista vuota. 
 */
ulinked_list* initULinkedList() {
	ulinked_list* new_list = malloc(sizeof(ulinked_list));
	if (new_list == NULL) {
		// TODO Errore OutOfMemory
		exit(0);
	}
	new_list->size = 0;
	new_list->head = NULL;
	return new_list;
}

/** 
 * Inserimento di un elemento in testa alla lista.
 */
void insertElementFirst(ulinked_list* l, void* new_element_data) {
	ulinked_list_node* new_element = malloc(sizeof(ulinked_list_node));
	if (new_element == NULL) {
		// TODO errore
	}
	new_element->data = new_element_data;
	new_element->next = l->head;
	l->head = new_element;
	l->size++;
}

/**
 * Inserimento di un elemento in coda alla lista.
 */
void insertElementLast(ulinked_list* l, void* new_element_data) {
	ulinked_list_node* new_element = malloc(sizeof(ulinked_list_node));
	if (new_element == NULL) {
		// TODO errore
	}
	new_element->data = new_element_data;
	new_element->next = NULL;
	
	if (l->size == EMPTY_SIZE)
		l->head = new_element;
	else
		getTail(l)->next = new_element;
	l->size++;
}

/**
 * Inserisce un elemento nella lista alla posizione desiderata.
 * Il primo elemento della lista ha posizione "0".
 * Se viene inserita una posizione superiore al numero di elementi, l'elemento viene inserito in coda.
 * Se viene inserita una posizione negativa, la funzione non viene eseguita.
 */
void insertElementAtPosition(ulinked_list* l, void* new_element_data, int pos) {
	if (pos < 0) {
		// TODO ERRORE
	} else if (pos == 0) {
		insertElementFirst(l, new_element_data);
	} else {
		ulinked_list_node* new_element = malloc(sizeof(ulinked_list_node));
		if (new_element == NULL) {
			// TODO errore
		}
		ulinked_list_node* iterator = l->head;
		for (int i = 0; i < pos - 1 && i < l->size - 1; i++) {
			iterator = iterator->next;
		}
		new_element->data = new_element_data;
		new_element->next = iterator->next;
		iterator->next = new_element;
		l->size++;
	}
}

/**
 * Inserisce tutti gli elementi in coda alla prima lista.
 * Gli elementi da aggiungere vengono passati come "ulinked_list*".
 * L'utilizzo di questa funzione provoca la cancellazione della seconda lista, pertanto è
 * sconsigliata se si vuole unire due liste differenti. In sostituzione, è possibile utilizzare
 * la funzione "concatenateTwoLists" che ricopia le liste in questione senza modificare gli originali.
 */
void insertAllElementsLast(ulinked_list* l, ulinked_list* elements) {
	getTail(l)->next = elements->head;
	l->size += elements->size;
	free(elements);
}

/**
 * Cancella il primo elemento della lista.
 */
void deleteFirstElement(ulinked_list* l) {
	if (l->size != EMPTY_SIZE) {
		ulinked_list_node* aux = l->head;
		l->head = l->head->next;
		free(aux);
		l->size--;
	}
}

/**
 * Cancella l'ultimo elemento della lista.
 */
void deleteLastElement(ulinked_list* l) {
	if (l->size == EMPTY_SIZE) {
		// TODO BOH
		// Non succede nulla?
	} else if (l->size == 1) {
		free(l->head);
		l->head = NULL;
		l->size--;
	} else {
		ulinked_list_node* iterator = l->head;
		for (int i = 1; i < l->size - 1; i++) {
			iterator->next = iterator->next->next;
		}
		free(iterator->next);
		iterator->next = NULL;
		l->size--;
	}
}

/**
 * Rimuove un elemento alla posizione desiderata.
 */
void deleteElementAtPosition(ulinked_list* l, int pos) {
	free(extractElementAtPosition(l, pos));
/*
	if (pos < 0 || pos >= l->size) { // Così sto gestendo anche il caso "lista vuota"
		// TODO Errore
	} else if (pos == 1) {
		ulinked_list_node* aux = l->head;
		l->head = l->head->next;
		free(aux);
		l->size--;
	} else {
		ulinked_list_node* iterator = l->head;
		for (int i = 0; i < pos - 1 && i < l->size - 1; i++) {
			iterator = iterator->next;
		}
		ulinked_list_node* aux = iterator->next;
		iterator->next = iterator->next->next;
		free(aux);
		l->size--;
	}
*/
}

/**
 * Rimuove tutti gli elementi che soddisfano una data condizione.
 */
void deleteElementsByConditions(ulinked_list* l, bool (*condition)(void*)) {
	ulinked_list_node* iterator = l->head;
	ulinked_list_node* aux = NULL;
	while(iterator->next != NULL) {
		if (condition(iterator->next->data)) {
			// Cancellazione di iterator-terminal char>next
			aux = iterator->next;
			iterator->next = iterator->next->next;
			free(aux);
			l->size--;
		}
		iterator = iterator->next;
	}
	if (condition(l->head)) {
		deleteFirstElement(l);
	}
}

/**
 * Restituisce il contenuto del primo elemento della lista.
 */
void* getHeadContent(ulinked_list* l) {
	return l->head->data;
}

/**
 * Restituisce il contenuto del primo elemento.
 */
void* extractHeadContent(ulinked_list* l) {
	if (l->size) {
		ulinked_list_node* aux = l->head;
		l->head = l->head->next;
		return aux->data;
	}
	return NULL;
}

/**
 * Estrae un elemento alla posizione desiderata, lo cancella dalla lista e lo restituisce come puntatore.
 */
static ulinked_list_node* extractElementAtPosition(ulinked_list* l, int pos) {
	if (pos < 0 || pos >= l->size) { // Così sto gestendo anche il caso "lista vuota"
		// TODO Errore
		return NULL;
	} else if (pos == 1) {
		ulinked_list_node* aux = l->head;
		l->head = l->head->next;
		l->size--;
		return aux;
	} else {
		ulinked_list_node* iterator = l->head;
		for (int i = 0; i < pos - 1 && i < l->size - 1; i++) {
			iterator = iterator->next;
		}
		ulinked_list_node* aux = iterator->next;
		iterator->next = iterator->next->next;
		l->size--;
		return aux;
	}
}

/**
 * Restituisce il contenuto dell'ultimo elemento della lista.
 */
void* getTailContent(ulinked_list* l) {
	if (l->size)
		return getTail(l)->data;
	return NULL;
}

/**
 * Restituisce l'ultimo elemento della lista.
 */
static ulinked_list_node* getTail(ulinked_list* l) {
	if (l->size == 0)
		return NULL; // Caso in cui la lista sia vuota
		
	ulinked_list_node* iterator = l->head;
	for (int i = 1; i < l->size; i++) {
		iterator = iterator->next;
	}
	return iterator;
}

/**
 * Restituisce il contentuto di un elemento alla posizione desiderata.
 */
void* getElementContentAtPosition(ulinked_list* l, int pos) {
	return getElementAtPosition(l, pos)->data;
}

/**
 * Restituisce l'elemento ad una data posizione.
 * Se viene inserita una posizione superiore al numero di elementi presenti o un numero negativo, viene restituito NULL.
 */
static ulinked_list_node* getElementAtPosition(ulinked_list* l, int pos) {
	if (pos < 0 || pos >= l->size)
		return NULL;
	
	ulinked_list_node* iterator = l->head;
	for (int i = 0; i < pos && i < l->size; i++) {
		iterator = iterator->next;
	}
	return iterator;
}

/**
 * Restituisce la quantità di elementi presenti nella lista.
 */
int getListSize(ulinked_list* l) {
	return l->size;
}

/**
 * Verifica che all'interno della lista sia presente almeno un elemento che soddisfi una data condizione.
 */
bool containsElement(ulinked_list* l, bool (*condition)(void*)) {
	ulinked_list_node* iterator = l->head;
	for (int i = 0; i < l->size; i++) {
		if (condition(iterator->data))
			return true;
		iterator = iterator->next;
	}
	return false;
}

/**
 * Scambia di posto due elementi della lista, date le loro posizioni.
 */
void swapTwoElements(ulinked_list* l, int pos1, int pos2) {
	if (pos1 == pos2 || pos1 >= l->size || pos2 >= l->size) {
		// DAFUQ
	} else {
		if (pos1 > pos2)
			SWAP(pos1, pos2);		// Mi assicuro che pos1 < pos2
		// Scambio i due elementi
		ulinked_list_node* aux = extractElementAtPosition(l, pos1);
		insertElementAtPosition(l, extractElementAtPosition(l, pos2 - 1)->data, pos1);
		insertElementAtPosition(l, aux->data, pos2);
	}
}

/**
 * Clona una lista, data in ingresso una funzione per la clonazione del contenuto di un elemento.
 * Garantisce il mantenimento dell'ordine durante il processo.
 */
ulinked_list* cloneOrderedList(ulinked_list* l, void* (*clone)(void*)) {
	// Inizializzo la nuova lista
	ulinked_list* new_list = initULinkedList();
	// Inizio la clonazione
	ulinked_list_node* aux;
	// Clono la testa
	if (l->size) {
		aux = malloc(sizeof(ulinked_list_node));
		aux->data = clone(getHeadContent(l));
		aux->next = NULL;
		new_list->head = aux;
	}
	// Clono il resto
	ulinked_list_node* iterator = l->head;			// Utilizzo un secondo iteratore (il primo in realtà è aux) per evitare di ciclare sulle liste più di una volta.
	ulinked_list_node* new_element;
	for (int i = 0; i < l->size; i++) {
		new_element = malloc(sizeof(ulinked_list_node));
		new_element->data = clone(iterator->data);
		// Aggiorno i puntatori/iteratori
		aux->next = new_element;			// Aux tiene in memoria il penultimo elemento aggiunto alla nuova lista
		aux = aux->next;
		iterator = iterator->next;
	}
	new_element->next = NULL; // Coda
	// Fine
	return new_list;
}

/**
 * Clona una lista, data in ingresso una funzione per la clonazione del contenuto di un elemento.
 * Non garantisce il mantenimento dell'ordine durante il processo, ma permette una maggiore velocità (FORSE). // TODO CONTROLLARE
 * In particolare, a fine esecuzione la lista ottenuta avrà ordine inverso.
 * 
 * @deprecated Use cloneOrderedList instead.
 */
ulinked_list* cloneUnorderedList(ulinked_list* l, void* (*clone)(void*)) {
	// Inizializzo la nuova lista
	ulinked_list* new_list = malloc(sizeof(ulinked_list));
	new_list->size = 0;
	new_list->head = NULL;
	// Clono gli elementi della lista originaria
	ulinked_list_node* iterator = l->head;
	for (int i = 0; i < l->size; i++) {
		insertElementFirst(new_list, clone(iterator->data));
		iterator = iterator->next;
	}
	// Restituisco la lista creata
	return new_list;
}

/**
 * Unisce gli elementi di due liste in un'unica nuova lista.
 * Le liste originali <b>NON</b> vengono modificate.
 * E' possibile personalizzare il processo di clonazione attraverso la funzione <i>clone</i> passata come parametro.
 */
ulinked_list* concatenateTwoLists(ulinked_list* l1, ulinked_list* l2, void* (*clone)(void*)) {
	ulinked_list* new_list = cloneOrderedList(l1, clone);
	insertAllElementsLast(new_list, cloneOrderedList(l2, clone));
	return new_list;
}

/**
 * Data una lista e una relazione d'ordine definita sui suoi elementi, la funzione
 * restitutisce il nodo con contenuto "massimo".
 * La relazione deve essere implementata come una funzione che prende in ingresso il contenuto di due nodi, 
 * e li confronta restituendo:
 * - un numero negativo se il primo dato è "minore" del secondo (stando alla relazione).
 * - 0 se i due dati sono considerati uguali dalla relazione d'ordine.
 * - un numero positivo se il primo dato è "maggiore" del secondo (stando alla relazione).
 */
static ulinked_list_node* getMinimumElement(ulinked_list* l, int (*compare)(void*, void*)) {
	ulinked_list_node* iterator = l->head->next;
	ulinked_list_node* minimum = l->head;
	for (int i = 1; i < l->size; i++) {
		if (compare(iterator->data, minimum->data) < 0) {
			minimum = iterator;
		}
	}
	return minimum;
}

/**
 * Data una lista e una relazione d'ordine definita sui suoi elementi, la funzione
 * restitutisce l'elemento "minimo".
 * La relazione deve essere implementata come una funzione che prende in ingresso il contenuto di due nodi, 
 * e li confronta restituendo:
 * - un numero negativo se il primo dato è "minore" del secondo (stando alla relazione).
 * - 0 se i due dati sono considerati uguali dalla relazione d'ordine.
 * - un numero positivo se il primo dato è "maggiore" del secondo (stando alla relazione).
 */
void* getMinimumContent(ulinked_list* l, int (*compare)(void*, void*)) {
	return getMinimumElement(l, compare)->data;
}

/**
 * Data una lista e una relazione d'ordine definita sui suoi elementi, la funzione
 * restitutisce il nodo con contenuto "massimo".
 * La relazione deve essere implementata come una funzione che prende in ingresso il contenuto di due nodi, 
 * e li confronta restituendo:
 * - un numero negativo se il primo dato è "minore" del secondo (stando alla relazione).
 * - 0 se i due dati sono considerati uguali dalla relazione d'ordine.
 * - un numero positivo se il primo dato è "maggiore" del secondo (stando alla relazione).
 */
static ulinked_list_node* getMaximumElement(ulinked_list* l, int (*compare)(void*, void*)) {
	ulinked_list_node* iterator = l->head->next;
	ulinked_list_node* maximum = l->head;
	for (int i = 1; i < l->size; i++) {
		if (compare(iterator->data, maximum->data) > 0) {
			maximum = iterator;
		}
	}
	return maximum;
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
void* getMaximumContent(ulinked_list* l, int (*compare)(void*, void*)) {
	return getMaximumElement(l, compare)->data;
}

/**
 * Ordina una lista in modo <i>crescente</i> secondo una relazione d'ordine definita dall'utente e passata come parametro.
 * La relazione deve essere implementata come una funzione che prende in ingresso il contenuto di due nodi, 
 * e li confronta restituendo:
 * - un numero negativo se il primo dato è "minore" del secondo (stando alla relazione).
 * - 0 se i due dati sono considerati uguali dalla relazione d'ordine.
 * - un numero positivo se il primo dato è "maggiore" del secondo (stando alla relazione).
 */
void sortByOrder(ulinked_list* l, int (*compare)(void*, void*)) {
	// TODODODODODODODODODODODODODODODOD
}

/**
 * Restituisce una stringa che rappresenta gli elementi contenuti all'interno della lista.
 * Per rappresentare ogni singolo nodo viene passata come parametro la funzione che converte in stringa il contenuto
 * di un elemento. E' necessario fornire anche la dimensione massima della stringa di un singolo elemento.
 */
char* listToString(ulinked_list* l, char* (*toString)(void*), int max_data_length) {
	char* s;
	// STRING_TITLE_LENGTH + max_data_length * l->size
	s = malloc(sizeof(STRING_TITLE_LENGTH + max_data_length * l->size));
	sprintf(s, STRING_TITLE, l->size);
	ulinked_list_node* iterator = l->head;
	while (iterator) {
		strcat(s, toString(iterator->data));
		iterator = iterator->next;
	}
	return s;
}
