#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "UnidirectionalList.h"

#ifndef MEMORY_ERROR
#	define MEMORY_ERROR printf("Error: Cannot allocate memory.\n"); exit(1)
#endif

#ifndef EMPTY_SIZE_ERROR
#	define EMPTY_SIZE_ERROR printf("Error: Cannot execute this function on empty list.\n"); exit(1)
#endif

#ifndef UNVALID_POSITION_ERROR
#	define UNVALID_POSITION_ERROR(pos) printf("Error: The position %d is unvalid.\n", pos )
#endif

/**
 * Libreria che permette la gestione di una lista linkata.
 * 
 * @author Michele Dusi <michele.dusi.it@ieee.org>
 * 
 */

/** 
 * Inizializzazione della lista vuota. 
 */
ulinked_list* ul_initList() {
	ulinked_list* new_list = malloc(sizeof(ulinked_list));
	if (new_list == NULL) {
		MEMORY_ERROR;
	}
	new_list->size = 0;
	new_list->head = NULL;
	return new_list;
}

/**
 * Elimina la lista passata come parametro, liberando le zone di memoria occupate
 * dalla struttura UnidirectionalLinkedList e dai nodi.
 * <b>NON</b> elimina gli oggetti a cui i puntatori nella lista puntano.
 */
void ul_cleanList(ulinked_list* l) {
	for (int i = 0; i < l->size; i++) {
		ul_deleteFirstElement(l);
	}
	free(l);
}

/**
 * Elimina la lista passata come parametro, liberando le zone di memoria occupate
 * dalla struttura ArrayList, dal vettore interno e dagli oggetti contenuti all'interno del vettore.
 * Questo significa che essi non saranno più utilizzabili dopo la chiamta a funzione.
 */
void ul_purgeList(ulinked_list* l) {
	while (l->size != EMPTY_SIZE) {
		ul_purgeFirstElement(l);
	}
	free(l);
}

/**
 * Verifica l'ammissibilita di un indice come posizione di un elemento.
 */
static bool ul_checkPositionValidity(ulinked_list* l, int pos) {
	return pos >= 0 && pos < l->size;
}

/**
 * Restituisce l'ultimo elemento della lista.
 */
static ulinked_list_node* ul_getLastNode(ulinked_list* l) {
	if (l->size == 0)
		return NULL; // Caso in cui la lista sia vuota
		
	ulinked_list_node* iterator = l->head;
	for (int i = 1; i < l->size; i++) {
		iterator = iterator->next;
	}
	return iterator;
}

/** 
 * Inserimento di un elemento in testa alla lista.
 */
void ul_insertElementFirst(ulinked_list* l, void* new_element_data) {
	ulinked_list_node* new_element = malloc(sizeof(ulinked_list_node));
	if (!new_element) {
		MEMORY_ERROR;
	}
	new_element->data = new_element_data;
	new_element->next = l->head;
	l->head = new_element;
	l->size++;
}

/**
 * Inserimento di un elemento in coda alla lista.
 */
void ul_insertElementLast(ulinked_list* l, void* new_element_data) {
	ulinked_list_node* new_element = malloc(sizeof(ulinked_list_node));
	if (!new_element) {
		MEMORY_ERROR;
	}
	new_element->data = new_element_data;
	new_element->next = NULL;
	
	if (l->size == EMPTY_SIZE)
		l->head = new_element;
	else
		ul_getLastNode(l)->next = new_element;
	l->size++;
}

/**
 * Inserisce un elemento nella lista alla posizione desiderata.
 * Il primo elemento della lista ha posizione "0".
 * Se viene inserita una posizione superiore al numero di elementi, l'elemento viene inserito in coda.
 * Se viene inserita una posizione negativa, la funzione non viene eseguita.
 */
void ul_insertElementAtPosition(ulinked_list* l, void* new_element_data, int pos) {
	if (pos < 0) {
		// TODO ERRORE
	} else if (pos == 0) {
		ul_insertElementFirst(l, new_element_data);
	} else {
		ulinked_list_node* new_element = malloc(sizeof(ulinked_list_node));
		if (!new_element) {
			MEMORY_ERROR;
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
 * la funzione "ul_concatenateTwoLists" che ricopia le liste in questione senza modificare gli originali.
 */
void ul_insertAllElementsLast(ulinked_list* l, ulinked_list* elements) {
	ul_getLastNode(l)->next = elements->head;
	l->size += elements->size;
	free(elements);
}

/**
 * Cancella il primo elemento della lista.
 * Il contenuto viene mantenuto in memoria e vi si può avere accesso tramite qualunque
 * puntatore precedentemente inizializzato ad esso.
 */
void ul_deleteFirstElement(ulinked_list* l) {
	if (l->size == EMPTY_SIZE) {
		EMPTY_SIZE_ERROR;
	} else {
		ulinked_list_node* aux = l->head;
		l->head = l->head->next;
		free(aux);
		l->size--;
	}
}

/**
 * Cancella il primo elemento della lista, svuotandone dalla memoria il contenuto.
 */
void ul_purgeFirstElement(ulinked_list* l) {
	if (l->size == EMPTY_SIZE) {
		EMPTY_SIZE_ERROR;
	} else {
		ulinked_list_node* aux = l->head;
		l->head = l->head->next;
		free(aux->data);
		free(aux);
		l->size--;
	}
}

/**
 * Cancella l'ultimo elemento della lista.
 * Non rimuove il contenuto dalla memoria.
 */
void ul_deleteLastElement(ulinked_list* l) {
	if (l->size == EMPTY_SIZE) {
		EMPTY_SIZE_ERROR;
	} else if (l->size == 1) {
		free(l->head);
		l->head = NULL;
		l->size--;
	} else {
		ulinked_list_node* iterator = l->head;
		for (int i = 1; i < l->size - 1; i++) {
			iterator = iterator->next;
		}
		free(iterator->next);
		iterator->next = NULL;
		l->size--;
	}
}

/**
 * Cancella l'ultimo elemento della lista, rimuovendone
 * il contenuto dalla memoria. Dopo la chiamata a questa funzione, l'ultimo elemento
 * della lista non sarà più accessibile anche se sono presenti puntatori ad esso.
 */
void ul_purgeLastElement(ulinked_list* l) {
	free(ul_extractElementAtPosition(l, l->size - 1));
}

/**
 * Rimuove dalla lista tutti gli elementi che soddisfano una data condizione. Questo non
 * ha effetto sulla locazione in memoria dei contenuti: essi potranno continuare ad essere 
 * individuati mediante puntatori precedentemente inizializzati.
 */
void ul_deleteElementsByCondition(ulinked_list* l, bool (*condition)(void*)) {
	ulinked_list_node* iterator = l->head;
	ulinked_list_node* aux = NULL;
	while(iterator->next != NULL) {
		// L'iterator si ferma sempre un passo prima dell'elemento da eliminare
		if (condition(iterator->next->data)) {
			// Ho trovato un elemento da eliminare, aggiorno i collegamenti
			aux = iterator->next;
			iterator->next = iterator->next->next;
			free(aux);
			l->size--;
		} else {
			// Altrimenti passo al successivo
			iterator = iterator->next;
		}
	}
	// Per ultima controlla le testa, che ha bisogno di aggiornamenti specifici per quanto riguarda i puntatori
	if (condition(l->head)) {
		ul_deleteFirstElement(l);
	}
}

/**
 * Restituisce il contenuto del primo elemento della lista.
 */
void* ul_getFirstElement(ulinked_list* l) {
	return l->head->data;
}

/**
 * Restituisce il contenuto del primo elemento, rimuovendolo dalla lista.
 */
void* ul_extractFirstElement(ulinked_list* l) {
	if (l->size != EMPTY_SIZE) {
		ulinked_list_node* aux = l->head;
		void* content = aux->data;
		l->head = l->head->next;
		free(aux);
		l->size--;
		return content;
	}
	return NULL;
}

/**
 * Estrae un elemento alla posizione desiderata, lo cancella dalla lista e lo restituisce come puntatore.
 * Può restituire NULL se la posizione inserita non è valida.
 */
static ulinked_list_node* ul_extractNodeAtPosition(ulinked_list* l, int pos) {
	if (!ul_checkPositionValidity(l, pos)) {
		return NULL;
	} else if (pos == 0) {
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
 * Estrae un elemento alla posizione desiderata, lo cancella dalla lista e lo restituisce come puntatore.
 * Il primo elemento della lista ha posizione "0".
 */
void* ul_extractElementAtPosition(ulinked_list* l, int pos) {
	if (ul_checkPositionValidity(l, pos)) {
		ulinked_list_node* extracted = ul_extractNodeAtPosition(l, pos);
		void* aux = extracted->data;
		free(extracted);
		return aux;
	} else {
		UNVALID_POSITION_ERROR(pos);
		return NULL;
	}
}

/** 
 * Data una condizione booleana, estrae dalla lista tutti gli elementi che soddisfano quella condizione,
 * restituendoli all'interno di una nuova lista.
 */
ulinked_list* ul_extractElementsByCondition(ulinked_list* l, bool (*condition)(void*)) {
	ulinked_list* extracted_list = ul_initList();
	ulinked_list_node* iterator = l->head;
	// Creo un "nodo fake" per accelerare le operazioni di aggiunta in coda
	ul_insertElementFirst(extracted_list, NULL);
	ulinked_list_node* last_extracted = extracted_list->head;
	
	// Ciclo su tutti gli elementi (tranne che per la testa)
	for (int i = 0; i < l->size; i++) {
		// L'iterator si ferma sempre un passo prima dell'elemento da estrarre
		if (condition(iterator->next->data)) {
			// Ho trovato un elemento da estrarre, aggiorno i collegamenti
			last_extracted->next = iterator->next;
			iterator->next = iterator->next->next;
			last_extracted = last_extracted->next;
			l->size--;
			extracted_list->size++;
		} else {
			// Altrimenti passo al successivo
			iterator = iterator->next;
		}
	}
	
	// Rimuovo il nodo "fake" precedentemente creato.
	ul_purgeFirstElement(extracted_list);
	
	// Per ultima controllo le testa, che ha bisogno di aggiornamenti specifici sui puntatori
	if (condition(l->head->data)) {
		// Se la condizione è soddisfatta, sposto la testa della lista originaria nella lista degli elementi estratti.
		last_extracted = extracted_list->head;
		extracted_list->head = l->head;
		l->head = l->head->next;
		extracted_list->head->next = last_extracted;
		
		l->size--;
		extracted_list->size++;
	}
	return extracted_list;
}

/**
 * Elimina tutti gli elementi della lista che soddisfano una data condizione.
 * Il contenuto degli elementi viene rimosso anche dalla memoria.
 */
void ul_purgeElementsByCondition(ulinked_list* l, bool (*condition)(void*)) {
	ul_purgeList(ul_extractElementsByCondition(l, condition));
}

/**
 * Rimuove un elemento dalla lista alla posizione desiderata.
 * Il contenuto di quell'elemento non viene eliminato dalla memoria.
 */
void ul_deleteElementAtPosition(ulinked_list* l, int pos) {
	free(ul_extractNodeAtPosition(l, pos));
}

/**
 * Rimuove un elemento dalla lista alla posizione desiderata, liberandone
 * anche lo spazio occupato in memoria. Il contenuto, perciò, non sarà più raggiungibile
 * una volta chiamata questa funzione.
 */
void ul_purgeElementAtPosition(ulinked_list* l, int pos) {
	free(ul_extractElementAtPosition(l, pos));
}

/**
 * Restituisce il contenuto dell'ultimo elemento della lista.
 */
void* ul_getLastElement(ulinked_list* l) {
	if (l->size != EMPTY_SIZE) {
		return ul_getLastNode(l)->data;
	}
	return NULL;
}

void* ul_extractLastElement(ulinked_list* l) {
	return ul_extractElementAtPosition(l, l->size - 1);
}

/**
 * Restituisce l'elemento ad una data posizione.
 * Se viene inserita una posizione superiore al numero di elementi presenti o un numero negativo, viene restituito NULL.
 */
static ulinked_list_node* ul_getNodeAtPosition(ulinked_list* l, int pos) {
	if (ul_checkPositionValidity(l, pos)) {
		ulinked_list_node* iterator = l->head;
		for (int i = 0; i < pos && i < l->size; i++) {
			iterator = iterator->next;
		}
		return iterator;
	} else {
		return NULL;
	}
}

/**
 * Restituisce il contentuto di un elemento alla posizione desiderata.
 */
void* ul_getElementAtPosition(ulinked_list* l, int pos) {
	return ul_getNodeAtPosition(l, pos)->data;
}

/**
 * Restituisce la quantità di elementi presenti nella lista.
 */
int ul_getListSize(ulinked_list* l) {
	return l->size;
}

/**
 * Restituisce la posizione dell'elemento corrispondente a quello cercato.
 * Se l'elemento non è presente all'interno della lista, viene restituito il valore -1.
 */
int ul_getElementPosition(ulinked_list* l, void* element_content) {
	ulinked_list_node* iterator = l->head;
	for (int i = 0; i < l->size; i++) {
		if (iterator->data == element_content) {
			return i;
		}
		iterator = iterator->next;
	}
	return -1;
}

/**
 * Verifica che all'interno della lista sia presente almeno un elemento che soddisfi una data condizione.
 */
bool ul_containsElement(ulinked_list* l, bool (*condition)(void*)) {
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
void ul_swapTwoElements(ulinked_list* l, int pos1, int pos2) {
	if (!ul_checkPositionValidity(l, pos1)) {
		UNVALID_POSITION_ERROR(pos1);
	} else if (!ul_checkPositionValidity(l, pos2)) {
		UNVALID_POSITION_ERROR(pos2);
	} else if (pos1 == pos2) {
		// Dafuq?
	} else {
		if (pos1 > pos2) {
			INT_SWAP(pos1, pos2);		// Mi assicuro che pos1 < pos2
		}
		// Scambio i due elementi
		void* aux = ul_extractElementAtPosition(l, pos1);
		ul_insertElementAtPosition(l, ul_extractElementAtPosition(l, pos2 - 1), pos1);
		ul_insertElementAtPosition(l, aux, pos2);
		// TODO Better version with linkers
	}
}

/**
 * Clona una lista, data in ingresso una funzione per la clonazione del contenuto di un elemento.
 * Garantisce il mantenimento dell'ordine durante il processo.
 */
ulinked_list* ul_cloneOrderedList(ulinked_list* l, void* (*clone)(void*)) {
	// Inizializzo la nuova lista
	ulinked_list* new_list = ul_initList();
	// Inizio la clonazione
	ulinked_list_node* aux;
	// Clono la testa
	if (l->size) {
		aux = malloc(sizeof(ulinked_list_node));
		if (!aux) {
			MEMORY_ERROR;
		}
		aux->data = clone(ul_getFirstElement(l));
		aux->next = NULL;
		new_list->head = aux;
		new_list->size++;
	}
	// Clono il resto
	ulinked_list_node* iterator = l->head->next;			// Utilizzo un secondo iteratore (il primo in realtà è aux) per evitare di ciclare sulle liste più di una volta.
	ulinked_list_node* new_element;
	for (int i = 1; i < l->size; i++) {
		new_element = malloc(sizeof(ulinked_list_node));
		if (!new_element) {
			MEMORY_ERROR;
		}
		new_element->data = clone(iterator->data);
		// Aggiorno i puntatori/iteratori
		aux->next = new_element;			// Aux tiene in memoria il penultimo elemento aggiunto alla nuova lista
		aux = aux->next;
		iterator = iterator->next;
		new_list->size++;
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
ulinked_list* ul_cloneUnorderedList(ulinked_list* l, void* (*clone)(void*)) {
	// Inizializzo la nuova lista
	ulinked_list* new_list = malloc(sizeof(ulinked_list));
	if (!new_list) {
		MEMORY_ERROR;
	}
	new_list->size = 0;
	new_list->head = NULL;
	// Clono gli elementi della lista originaria
	ulinked_list_node* iterator = l->head;
	for (int i = 0; i < l->size; i++) {
		ul_insertElementFirst(new_list, clone(iterator->data));
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
ulinked_list* ul_concatenateTwoLists(ulinked_list* l1, ulinked_list* l2, void* (*clone)(void*)) {
	ulinked_list* new_list = ul_cloneOrderedList(l1, clone);
	ul_insertAllElementsLast(new_list, ul_cloneOrderedList(l2, clone));
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
static ulinked_list_node* ul_getMinimumNode(ulinked_list* l, int (*compare)(void*, void*)) {
	ulinked_list_node* iterator = l->head->next;
	ulinked_list_node* minimum = l->head;
	for (int i = 1; i < l->size; i++) {
		if (compare(iterator->data, minimum->data) < 0) {
			minimum = iterator;
		}
		iterator = iterator->next;
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
void* ul_getMinimumElement(ulinked_list* l, int (*compare)(void*, void*)) {
	return ul_getMinimumNode(l, compare)->data;
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
static ulinked_list_node* ul_getMaximumNode(ulinked_list* l, int (*compare)(void*, void*)) {
	ulinked_list_node* iterator = l->head->next;
	ulinked_list_node* maximum = l->head;
	for (int i = 1; i < l->size; i++) {
		if (compare(iterator->data, maximum->data) > 0) {
			maximum = iterator;
		}
		iterator = iterator->next;
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
void* ul_getMaximumElement(ulinked_list* l, int (*compare)(void*, void*)) {
	return ul_getMaximumNode(l, compare)->data;
}

/**
 * Restituisce la sottolista che parte dall'elemento di indice start_pos all'elemento di end_pos.
 * L'elemento end_pos è <emph>escluso</emph>, mentre viene incluso l'elemento start_pos.
 * 
 * <i>NOTA:</i> Viene creata una nuova lista, ma non vengono clonati gli elementi (che pertanto rimangono quelli della lista originale
 * e non subiscono modifiche).
 * Un qualsiasi cambiamento alla lista originaria può provocare effetti collaterali alla sottolista, e viceversa.
 * 
 * <i>NOTA:</i>Si consiglia di non fare operazioni sugli estremi della sottolista. Questa funzione restituisce una lista
 * comoda per operazioni di lettura o, al massimo, inserimento/rimozione <emph>al centro</emph> della sottolista.
 */
ulinked_list* ul_getSubList(ulinked_list* l, int start_pos, int end_pos) {
	// Inizializzazione di una lista vuota
	ulinked_list* sublist = malloc(sizeof(ulinked_list));
	if (!sublist) {
		MEMORY_ERROR;
	}
	// Controlli sulle posizioni
	if (!ul_checkPositionValidity(l, start_pos)) {
		UNVALID_POSITION_ERROR(start_pos);
	} else if (!ul_checkPositionValidity(l, end_pos)) {
		UNVALID_POSITION_ERROR(end_pos);
	} else if (start_pos >= end_pos) {
		UNVALID_POSITION_ERROR(end_pos);
	} else {
		// Linking della sotto-lista
		sublist->size = end_pos - start_pos;
		sublist->head = ul_getNodeAtPosition(l, start_pos);
		return sublist;
	}
	return NULL;
}

/**
 * Restituisce una sottolista che parte dall'elemento di indice start_pos fino all'elemento di indice end_pos;
 * La lista originale non viene modificata, e ogni singolo elemento viene clonato dalla funzione passata come parametro.
 */
ulinked_list* ul_cloneSubList(ulinked_list* l, int start_pos, int end_pos, void* (*clone)(void*)) {
	// Inizializzazione di una lista vuota
	ulinked_list* sublist = ul_initList();
	// Controlli sulle posizioni
	if (!ul_checkPositionValidity(l, start_pos)) {
		UNVALID_POSITION_ERROR(start_pos);
	} else if (!ul_checkPositionValidity(l, end_pos - 1)) {
		UNVALID_POSITION_ERROR(end_pos);
	} else if (start_pos >= end_pos) {
		UNVALID_POSITION_ERROR(end_pos);
	} else {
		// Clonazione della sotto-lista
		sublist->size = end_pos - start_pos - 1; // Alla fine ri-aggiungerò 1 quando inserirò la testa.
		
		ulinked_list_node* iterator_l = ul_getNodeAtPosition(l, start_pos);
		ul_insertElementFirst(sublist, clone(iterator_l->data));
		ulinked_list_node* iterator_sublist = sublist->head;
		
		for (int i = 1; i < sublist->size; i++) {
			iterator_sublist->next = malloc(sizeof(ulinked_list_node)); // Creo un nuovo elemento successivo nella sottolista
			iterator_sublist = iterator_sublist->next; // Ora l'iteratore sulla sottolista punta al nuovo elemento che sot creando.
			if (!iterator_sublist) {
				MEMORY_ERROR;
			}
			iterator_l = iterator_l->next;
			iterator_sublist->data = clone(iterator_l->data);
		}

		iterator_sublist->next = NULL; // L'ultimo elemento della sottolista ha il successivo nullo.
		return sublist;
	}
	return NULL;
}

/**
 * Ordina una lista in modo <i>crescente</i> secondo una relazione d'ordine definita dall'utente e passata come parametro.
 * La relazione deve essere implementata come una funzione che prende in ingresso il contenuto di due nodi, 
 * e li confronta restituendo:
 * - un numero negativo se il primo dato è "minore" del secondo (stando alla relazione).
 * - 0 se i due dati sono considerati uguali dalla relazione d'ordine.
 * - un numero positivo se il primo dato è "maggiore" del secondo (stando alla relazione).
 */
void ul_sortByOrder(ulinked_list* l, int (*compare)(void*, void*)) {
	// Implementazione di un Bubble Sort per l'ordinamento della lista
	for (int i = 0; i < l->size; i++) {
		for (int j = l->size - 1; j > i; j--) {
			if (compare(ul_getElementAtPosition(l, j), ul_getElementAtPosition(l, j - 1)) < 0) {
				ul_swapTwoElements(l, j, j - 1);
			}
		}
	}
}

/**
 * Restituisce una stringa che rappresenta gli elementi contenuti all'interno della lista.
 * Per rappresentare ogni singolo nodo viene passata come parametro la funzione che converte in stringa il contenuto
 * di un elemento. E' necessario fornire anche la dimensione massima della stringa di un singolo elemento.
 */
char* ul_listToString(ulinked_list* l, char* (*toStringFunction)(void*)) {
	char* s = malloc(sizeof(char) * (strlen(STRING_TITLE) + 5));
	if (!s) {
		MEMORY_ERROR;
	}
	sprintf(s, STRING_TITLE, l->size);
	ulinked_list_node* iterator = l->head;
	char* new_elem_string;
	for (int i = 0; i < l->size; i++) {
		// Nella stringa ausiliaria alloco la descrizione dell'elemento corrente
		new_elem_string = toStringFunction(iterator->data);
		// Aumento la dimensione della stringa buffer
		s = realloc(s, strlen(s) + strlen(new_elem_string) + 5);
		// Concateno la descrizione al buffer
		strcat(s, new_elem_string);
		strcat(s, "   ");
		// Libero la memoria
		free(new_elem_string);
		// Aggiorno il puntatore dell'iteratore
		iterator = iterator->next;
	}
	return s;
}

//////////////////////////////////////////////////////
// TESTING //


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
	char* created_string = malloc(sizeof(char) * (strlen(TARGA_STRING_FORMAT) *2));
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

#include <time.h>

int main(void) {
	// Inizializzo robe
	ulinked_list* my_list = ul_initList();
	srand(time(NULL));
	int dim = 10;
	char* str1;
	
	// Popolo la lista
	for (int i = 0; i < dim; i++) {
		ul_insertElementFirst(my_list, initRandomTarga());
	}
	
	// Stampo la lista
	str1 = ul_listToString(my_list, stringifyMyStruct);
	printf("(1) %s\n", str1);
	free(str1);
	
	ulinked_list* sublist = ul_cloneSubList(my_list, 0, 10, cloneMyStruct);
	
	// Stampo la lista
	str1 = ul_listToString(sublist, stringifyMyStruct);
	printf("(3)%s\n", str1);
	free(str1);
	
	// FREE
	ul_purgeList(my_list);
	ul_purgeList(sublist);
		
	return 0;
}

/// TESTING VARIO ///
	
/*
// Estraggo il primo elemento
targa* e = ul_getElementAtPosition(my_list, 2);
char* buffer2 = stringifyMyStruct(e);
printf("Elemento letto: %s\n", buffer2);
free(buffer2);

// Stampo la lista
char* str2 = ul_listToString(my_list, stringifyMyStruct);
printf("%s\n", str2);
free(str2);

// Leggo un elemento e lo stampo
targa* my_elem = ul_extractElementAtPosition(my_list, 0);
char* buffer = stringifyMyStruct(my_elem);
printf("Elemento letto: %s\n", buffer);
free(buffer);
free(my_elem);

// Stampo la lista
char* str3 = ul_listToString(my_list, stringifyMyStruct);
printf("%s\n", str3);
free(str3);



// Elimino dalla lista le targhe pari, mantenendole in memoria
ul_purgeElementsByCondition(my_list, hasEvenNumber);	

// Stampo la lista
str1 = ul_listToString(my_list, stringifyMyStruct);
printf("Lista originale mangiucchiata:\n%s\n", str1);
free(str1);
* 

	// Scambio due elementi
	int pos1 = 0, pos2 = 6;
	printf("Scambio gli elementi nelle posizioni %d e %d.\n", pos1, pos2);
	ul_swapTwoElements(my_list, pos1, pos2);


*/

