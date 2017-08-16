#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "BidirectionalList.h"

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
 * Libreria che permette la gestione di una lista linkata doppiamente.
 * La lista porterà via (ovviamente) più spazio in memoria rispetto ad una ulinked_list, ma il fatto di
 * avere un doppio legame fra elementi adiacenti permette una maggiore velocità di esecuzione delle operazioni su di essa
 * proprio perchè spesso non è necessario occupare altra memoria temporanea nello stack.
 * Molte funzioni, inoltre, sono ottimizzate per "scorrere" la lista nel verso più opportuno a seconda delle necessità.
 * 
 * @author Michele Dusi <michele.dusi.it@ieee.org>
 * 
 */

/** Structs definitions: 

typedef struct blinked_list_node {
	void* data;
	struct blinked_list_node* next;
	struct blinked_list_node* prev;
} blinked_list_node;

typedef struct blinked_list {
	int size;
	blinked_list_node* head;
	blinked_list_node* tail;	
} blinked_list;
*/

// Initializing List

/** 
 * Inizializzazione della lista vuota. 
 */
blinked_list* bl_initList() {
	blinked_list* new_list = malloc(sizeof(blinked_list));
	if (new_list == NULL) {
		MEMORY_ERROR;
	}
	new_list->size = 0;
	new_list->head = NULL;
	new_list->tail = NULL;
	return new_list;
}

// Utility Private Functions

/**
 * Verifica l'ammissibilita di un indice come posizione di un elemento.
 */
static bool bl_checkPositionValidity(blinked_list* l, int pos) {
	return pos >= 0 && pos < l->size;
}

// Private Node Manipulation

/**
 * Restituisce l'elemento ad una data posizione.
 * Se viene inserita una posizione superiore al numero di elementi presenti o un numero negativo, viene restituito NULL.
 */
static blinked_list_node* bl_getNodeAtPosition(blinked_list* l, int pos) {
	if (bl_checkPositionValidity(l, pos)) {
		blinked_list_node* iterator = NULL;
		if (pos <= l->size / 2) {
			// L'elemento è nella prima metà della lista
			iterator = l->head;
			for (int i = 0; i < pos; i++) {
				iterator = iterator->next;
			}
		} else {
			// L'elemento è nella seconda metà della lista
			iterator = l->tail;
			for (int i = l->size - 1; i > pos; i--) {
				iterator = iterator->prev;
			}
		}
		return iterator;
	} else {
		UNVALID_POSITION_ERROR(pos);
		return NULL;
	}
}

/**
 * Estrae un nodo alla posizione desiderata, lo cancella dalla lista e lo restituisce come puntatore.
 * Può restituire NULL se la posizione inserita non è valida.
 */
static blinked_list_node* bl_extractNodeAtPosition(blinked_list* l, int pos) {
	if (bl_checkPositionValidity(l, pos)) {
		blinked_list_node* iterator;
		// Casistica sulla posizione
		if (pos == 0) { // Testa
			iterator = l->head;
			l->head = l->head->next;
			l->head->prev = NULL;
		} else if (pos == l->size - 1) { // Coda
			iterator = l->tail;
			l->tail = l->tail->prev;
			l->tail->next = NULL;
		} else if (pos <= l->size / 2) { // Prima metà
			iterator = l->head;
			for (int i = 0; i < pos; i++) {
				iterator = iterator->next;
			}
			iterator->prev->next = iterator->next;
			iterator->next->prev = iterator->prev;
		} else { // Seconda metà
			iterator = l->tail;
			for (int i = l->size - 1; i > pos; i--) {
				iterator = iterator->prev;
			}
			iterator->prev->next = iterator->next;
			iterator->next->prev = iterator->prev;
		}
		l->size--;
		return iterator;
	} else {
		// Errore di posizione
		UNVALID_POSITION_ERROR(pos);
		return NULL;
	}
}

/**
 * Inserisce un elemento in una lista vuota.
 * Pre-conditions: la lista passata come parametro non deve contenere elementi.
 */
static void bl_insertInitialElement(blinked_list* l, void* new_element_data) {
	l->head = malloc(sizeof(blinked_list_node));
	if (l->head == NULL) {
		MEMORY_ERROR;
	}
	l->tail = l->head;
	// Inizializzo il nodo
	l->head->data = new_element_data;
	l->head->next = NULL;
	l->head->prev = NULL;
	l->size++;
}

// Size

/**
 * Restituisce la quantità di elementi presenti nella lista.
 */
int bl_getListSize(blinked_list* l) {
	return l->size;
}

// Cancelling List

/**
 * Elimina la lista passata come parametro, liberando le zone di memoria occupate
 * dalla struttura UnidirectionalLinkedList e dai nodi.
 * <b>NON</b> elimina gli oggetti a cui i puntatori nella lista puntano.
 */
void bl_deleteList(blinked_list* l) {
	for (int i = 1; i < l->size; i++) {
		l->head = l->head->next;
		free(l->head->prev);
	}
	free(l->head);
	free(l);
}

/**
 * Elimina la lista passata come parametro, liberando le zone di memoria occupate
 * dalla struttura ArrayList, dal vettore interno e dagli oggetti contenuti all'interno del vettore.
 * Questo significa che essi non saranno più utilizzabili dopo la chiamta a funzione.
 */
void bl_purgeList(blinked_list* l) {
	if (l->size != EMPTY_SIZE) { // Questa funzione non utilizza memoria aggiuntiva, neanche nello stack
		for (int i = 1; i < l->size; i++) {
			l->head = l->head->next;
			free(l->head->prev->data);
			free(l->head->prev);
		}
		free(l->head->data);
		free(l->head);
	}
	free(l);
}

// Inserting Elements

/** 
 * Inserimento di un elemento in testa alla lista.
 */
void bl_insertFirstElement(blinked_list* l, void* new_element_data) {
	if (l->size == EMPTY_SIZE) {
		bl_insertInitialElement(l, new_element_data);
	} else {
		l->head->prev = malloc(sizeof(blinked_list_node));
		if (!(l->head->prev)) {
			MEMORY_ERROR;
		}
		l->head->prev->next = l->head;
		l->head = l->head->prev;
		l->head->prev = NULL;
		l->head->data = new_element_data;
		l->size++;
	}
}

/**
 * Inserimento di un elemento in coda alla lista.
 */
void bl_insertLastElement(blinked_list* l, void* new_element_data) {
	if (l->size == EMPTY_SIZE) {
		bl_insertInitialElement(l, new_element_data);
	} else {
		l->tail->next = malloc(sizeof(blinked_list_node));
		if (!(l->tail->next)) {
			MEMORY_ERROR;
		}
		l->tail->next->prev = l->tail;
		l->tail = l->tail->next;
		l->tail->next = NULL;
		l->tail->data = new_element_data;
		l->size++;
	}	
}	

/**
 * Inserisce un elemento nella lista alla posizione desiderata.
 * Il primo elemento della lista ha posizione "0".
 * Se viene inserita una posizione superiore al numero di elementi, l'elemento viene inserito in coda.
 * Se viene inserita una posizione negativa, la funzione non viene eseguita.
 */
void bl_insertElementAtPosition(blinked_list* l, void* new_element_data, int pos) {
	if (pos < 0) {
		// TODO ERRORE
	} else if (pos == 0) {
		bl_insertFirstElement(l, new_element_data);
	} else if (pos >= l->size) {
		bl_insertLastElement(l, new_element_data);
	} else {
		// Inserisco non agli estremi
		blinked_list_node* new_element = malloc(sizeof(blinked_list_node));
		if (!new_element) {
			MEMORY_ERROR;
		}
		new_element->data =  new_element_data;
		
		if (pos <= l->size / 2) { // Prima metà
			new_element->prev = l->head;
			for (int i = 0; i < pos - 1; i++) {
				new_element->prev = new_element->prev->next;
			}
			new_element->next = new_element->prev->next;
		} else { // Seconda metà
			new_element->next = l->tail;
			for (int i = l->size - 1; i > pos; i--) {
				new_element->next = new_element->next->prev;
			}
			new_element->prev = new_element->next->prev;
		}
		
		new_element->next->prev = new_element;
		new_element->prev->next = new_element;
		l->size++;
	}
}

/**
 * Inserisce tutti gli elementi in coda alla prima lista.
 * Gli elementi da aggiungere vengono passati come "blinked_list*".
 * L'utilizzo di questa funzione provoca la cancellazione della seconda lista, pertanto è
 * sconsigliata se si vuole unire due liste differenti. In sostituzione, è possibile utilizzare
 * la funzione "bl_concatenateTwoLists" che ricopia le liste in questione senza modificare gli originali.
 */
void bl_insertAllElementsLast(blinked_list* l, blinked_list* elements) {
	l->tail->next = elements->head;
	elements->head->prev = l->tail;
	l->size += elements->size;
	free(elements);
}

// Deleting Elements

/**
 * Cancella il primo elemento della lista.
 * Il contenuto, però, viene mantenuto in memoria e vi si può avere accesso tramite qualunque
 * puntatore precedentemente inizializzato ad esso.
 */
void bl_deleteFirstElement(blinked_list* l) {
	if (l->size == EMPTY_SIZE) {
		EMPTY_SIZE_ERROR;
	} else if (l->size == 1) {
		free(l->head);
		l->head = NULL;
		l->tail = NULL;
		l->size--;
	} else {
		l->head = l->head->next;
		free(l->head->prev);
		l->head->prev = NULL;
		l->size--;
	}
}

/**
 * Cancella l'ultimo elemento della lista.
 * Non rimuove il contenuto dalla memoria.
 */
void bl_deleteLastElement(blinked_list* l) {
	if (l->size == EMPTY_SIZE) {
		EMPTY_SIZE_ERROR;
	} else if (l->size == 1) {
		free(l->tail);
		l->head = NULL;
		l->tail = NULL;
		l->size--;
	} else {
		l->tail = l->tail->prev;
		free(l->tail->next);
		l->tail->next = NULL;
		l->size--;
	}
}

/**
 * Rimuove un elemento dalla lista alla posizione desiderata.
 * Il contenuto di quell'elemento non viene eliminato dalla memoria.
 */
void bl_deleteElementAtPosition(blinked_list* l, int pos) {
	free(bl_extractNodeAtPosition(l, pos));
}

/**
 * Rimuove dalla lista tutti gli elementi che soddisfano una data condizione. Questo non
 * ha effetto sulla locazione in memoria dei contenuti: essi potranno continuare ad essere 
 * individuati mediante puntatori precedentemente inizializzati.
 */
void bl_deleteElementsByCondition(blinked_list* l, bool (*condition)(void*)) {
	blinked_list_node* iterator = l->head;
	for (int i = 1; i < l->size - 1; i++) { // Scorro su tutti gli elementi centrali (quindi NON coda e testa).
		if (condition(iterator->next->data)) {
			iterator->next = iterator->next->next;
			free(iterator->next->prev);
			iterator->next->prev = iterator;
			l->size--;													/// TODO: Controllare che il fatto che venga cambiata size qui dentro non rompa le balle al ciclo for (o provare con size impostata come volatile).
		} else {
			iterator = iterator->next;
		}
	}
	if (condition(l->head->data)) {
		bl_deleteFirstElement(l);
	}
	if (condition(l->tail->data) {
		bl_deleteLastElement(l);
	}
}

// Purging Elements

/**
 * Cancella il primo elemento della lista, svuotandone dalla memoria il contenuto.
 */
void bl_purgeFirstElement(blinked_list* l) {
	if (l->size == EMPTY_SIZE) { // Lista vuota
		EMPTY_SIZE_ERROR;
	} else if (l->size == 1) { // Elemento singolo
		free(l->head->data);
		free(l->head);
		l->head = NULL;
		l->tail = NULL;
		l->size--;
	} else {					// Lista con almeno due elementi
		free(l->head->data);
		l->head = l->head->next;
		free(l->head->prev);
		l->head->prev = NULL;
		l->size--;
	}
}

/**
 * Cancella l'ultimo elemento della lista, rimuovendone
 * il contenuto dalla memoria. Dopo la chiamata a questa funzione, l'ultimo elemento
 * della lista non sarà più accessibile anche se sono presenti puntatori ad esso.
 */
void bl_purgeLastElement(blinked_list* l) {
	if (l->size == EMPTY_SIZE) { // Lista vuota
		EMPTY_SIZE_ERROR;
	} else if (l->size == 1) {
		free(l->tail->data);
		free(l>-tail);
		l->head = NULL;
		l->tail = NULL;
		l->size--;
	} else {
		free(l->tail->data);
		l->tail = l->tail->prev;
		free(l->tail->next);
		l->tail->next = NULL;
		l->size--;
	}
}

/**
 * Rimuove un elemento dalla lista alla posizione desiderata, liberandone
 * anche lo spazio occupato in memoria. Il contenuto, perciò, non sarà più raggiungibile
 * una volta chiamata questa funzione.
 */
void bl_purgeElementAtPosition(blinked_list* l, int pos) {
	free(bl_extractElementAtPosition(l, pos));
}

/**
 * Elimina tutti gli elementi della lista che soddisfano una data condizione.
 * Il contenuto degli elementi viene rimosso anche dalla memoria.
 */
void bl_purgeElementsByCondition(blinked_list* l, bool (*condition)(void*)) {
	bl_purgeList(bl_extractElementsByCondition(l, condition));
}

// Getting Elements

/**
 * Restituisce il primo elemento (ossia il contenuto del primo nodo) della lista.
 */
void* bl_getFirstElement(blinked_list* l) {
	if (l->size == EMPTY_SIZE) {
		return NULL;
	} else {
		return l->head->data;
	}
}

/**
 * Restituisce l'ultimo elemento (ossia il contenuto dell'ultimo nodo) della lista.
 */
void* bl_getLastElement(blinked_list* l) {
	if (l->size == EMPTY_SIZE) {
		return NULL;
	} else {
		return l->tail->data;
	}
}

/**
 * Restituisce il contentuto di un elemento alla posizione desiderata.
 */
void* bl_getElementAtPosition(blinked_list* l, int pos) {
	return bl_getNodeAtPosition(l, pos)->data;
}

///////////////////////////////// FINO A QUI

/**
 * Restituisce una sotto-lista con tutti gli elementi che soddisfano una data condizione.
 * Gli elementi sono esattamente gli stessi (i puntatori puntano agli stessi elementi della lista originale),
 * ma la "struct" lista è differente: sarà perciò sufficiente pulirla con il metodo "bl_deleteList" al termine del suo utilizzo.
 */
blinked_list* bl_getElementsByCondition(blinked_list* l, bool (*condition)(void*)) {
	blinked_list* sublist = bl_initList();
	blinked_list_node* iterator = l->head;
	// Creo un "nodo fake" per accelerare le operazioni di aggiunta in coda
	blinked_list_node* last_taken = malloc(sizeof(blinked_list_node));
	if (!last_taken) {
		MEMORY_ERROR;
	}
	sublist->head = last_taken;
	// Ciclo su tutta la lista originale
	for (int i = 0; i < l->size; i++) {
		// Se un elemento soddisfa la condizione
		if (condition(iterator->data)) {
			// Creo un nuovo nodo nella sottolista
			last_taken->next = malloc(sizeof(blinked_list_node));
			if (!(last_taken->next)) {
				MEMORY_ERROR;
			}
			last_taken = last_taken->next;
			last_taken->data = iterator->data;
			sublist->size++;
		}
		iterator = iterator->next;
	}
	last_taken->next = NULL;
	// Rimuovo il nodo fake ausiliario
	iterator = sublist->head;
	sublist->head = sublist->head->next;
	free(iterator);
	
	return sublist;	
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
blinked_list* bl_getSubList(blinked_list* l, int start_pos, int end_pos) {
	// Inizializzazione di una lista vuota
	blinked_list* sublist = malloc(sizeof(blinked_list));
	if (!sublist) {
		MEMORY_ERROR;
	}
	// Controlli sulle posizioni
	if (!bl_checkPositionValidity(l, start_pos)) {
		UNVALID_POSITION_ERROR(start_pos);
	} else if (!bl_checkPositionValidity(l, end_pos)) {
		UNVALID_POSITION_ERROR(end_pos);
	} else if (start_pos >= end_pos) {
		UNVALID_POSITION_ERROR(end_pos);
	} else {
		// Linking della sotto-lista
		sublist->size = end_pos - start_pos;
		sublist->head = bl_getNodeAtPosition(l, start_pos);
		return sublist;
	}
	return NULL;
}

// Extracting Elements

/**
 * Restituisce il contenuto del primo elemento, rimuovendolo dalla lista.
 */
void* bl_extractFirstElement(blinked_list* l) {
	if (l->size != EMPTY_SIZE) {
		blinked_list_node* aux = l->head;
		void* content = aux->data;
		l->head = l->head->next;
		free(aux);
		l->size--;
		return content;
	}
	return NULL;
}

/**
 * Estrae dalla lista l'ultimo elemento, e lo restituisce come valore di ritorno.
 */
void* bl_extractLastElement(blinked_list* l) {
	return bl_extractElementAtPosition(l, l->size - 1);
}

/**
 * Estrae un elemento alla posizione desiderata, lo cancella dalla lista e lo restituisce come puntatore.
 * Il primo elemento della lista ha posizione "0".
 */
void* bl_extractElementAtPosition(blinked_list* l, int pos) {
	if (bl_checkPositionValidity(l, pos)) {
		blinked_list_node* extracted = bl_extractNodeAtPosition(l, pos);
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
 * In pratica, "separa" la lista in due sottoliste dove nella nuova lista gli elementi soddisfano la condizione, 
 * mentre nella vecchia lista no.
 */
blinked_list* bl_extractElementsByCondition(blinked_list* l, bool (*condition)(void*)) {
	blinked_list* extracted_list = bl_initList();
	blinked_list_node* iterator = l->head;
	// Creo un "nodo fake" per accelerare le operazioni di aggiunta in coda
	bl_insertFirstElement(extracted_list, NULL);
	blinked_list_node* last_extracted = extracted_list->head;
	
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
	bl_purgeFirstElement(extracted_list);
	
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

// Searching Elements

/**
 * Verifica che all'interno della lista sia presente un elemento dato come parametro.
 */
bool bl_containsElement(blinked_list* l, void* element_content) {
	blinked_list_node* iterator = l->head;
	for (int i = 0; i < l->size; i++) {
		if (iterator->data == element_content) {
			return true;
		}
		iterator = iterator->next;
	}
	return false;
}

/**
 * Verifica che all'interno della lista sia presente almeno un elemento che soddisfi una data condizione.
 */
bool bl_containsElementByCondition(blinked_list* l, bool (*condition)(void*)) {
	blinked_list_node* iterator = l->head;
	for (int i = 0; i < l->size; i++) {
		if (condition(iterator->data))
			return true;
		iterator = iterator->next;
	}
	return false;
}

/**
 * Conta tutti gli elementi che soddisfano una data condizione.
 */
int bl_countElementsByCondition(blinked_list* l, bool (*condition)(void*)) {
	blinked_list_node* iterator = l->head;
	int count = 0;
	for (int i = 0; i < l->size; i++) {
		if (condition(iterator->data)) {
			count++;
		}
		iterator = iterator->next;
	}
	return count;
}

/**
 * Restituisce la posizione dell'elemento corrispondente a quello cercato.
 * Se l'elemento non è presente all'interno della lista, viene restituito il valore -1.
 */
int bl_getElementPosition(blinked_list* l, void* element_content) {
	blinked_list_node* iterator = l->head;
	for (int i = 0; i < l->size; i++) {
		if (iterator->data == element_content) {
			return i;
		}
		iterator = iterator->next;
	}
	return -1;
}

// Cloning and Managing List

/**
 * Clona una lista, data in ingresso una funzione per la clonazione del contenuto di un elemento.
 * Garantisce il mantenimento dell'ordine durante il processo.
 */
blinked_list* bl_cloneOrderedList(blinked_list* l, void* (*clone)(void*)) {
	// Inizializzo la nuova lista
	blinked_list* new_list = bl_initList();
	// Inizio la clonazione
	blinked_list_node* aux;
	// Clono la testa
	if (l->size) {
		aux = malloc(sizeof(blinked_list_node));
		if (!aux) {
			MEMORY_ERROR;
		}
		aux->data = clone(bl_getFirstElement(l));
		aux->next = NULL;
		new_list->head = aux;
		new_list->size++;
	}
	// Clono il resto
	blinked_list_node* iterator = l->head->next;			// Utilizzo un secondo iteratore (il primo in realtà è aux) per evitare di ciclare sulle liste più di una volta.
	blinked_list_node* new_element;
	for (int i = 1; i < l->size; i++) {
		new_element = malloc(sizeof(blinked_list_node));
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
blinked_list* bl_cloneUnorderedList(blinked_list* l, void* (*clone)(void*)) {
	// Inizializzo la nuova lista
	blinked_list* new_list = malloc(sizeof(blinked_list));
	if (!new_list) {
		MEMORY_ERROR;
	}
	new_list->size = 0;
	new_list->head = NULL;
	// Clono gli elementi della lista originaria
	blinked_list_node* iterator = l->head;
	for (int i = 0; i < l->size; i++) {
		bl_insertFirstElement(new_list, clone(iterator->data));
		iterator = iterator->next;
	}
	// Restituisco la lista creata
	return new_list;
}

/**
 * Restituisce una sottolista che parte dall'elemento di indice start_pos fino all'elemento di indice end_pos;
 * La lista originale non viene modificata, e ogni singolo elemento viene clonato dalla funzione passata come parametro.
 */
blinked_list* bl_cloneSubList(blinked_list* l, int start_pos, int end_pos, void* (*clone)(void*)) {
	// Inizializzazione di una lista vuota
	blinked_list* sublist = bl_initList();
	// Controlli sulle posizioni
	if (!bl_checkPositionValidity(l, start_pos)) {
		UNVALID_POSITION_ERROR(start_pos);
	} else if (!bl_checkPositionValidity(l, end_pos - 1)) {
		UNVALID_POSITION_ERROR(end_pos);
	} else if (start_pos >= end_pos) {
		UNVALID_POSITION_ERROR(end_pos);
	} else {
		// Clonazione della sotto-lista
		sublist->size = end_pos - start_pos - 1; // Alla fine ri-aggiungerò 1 quando inserirò la testa.
		
		blinked_list_node* iterator_l = bl_getNodeAtPosition(l, start_pos);
		bl_insertFirstElement(sublist, clone(iterator_l->data));
		blinked_list_node* iterator_sublist = sublist->head;
		
		for (int i = 1; i < sublist->size; i++) {
			iterator_sublist->next = malloc(sizeof(blinked_list_node)); // Creo un nuovo elemento successivo nella sottolista
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
 * Unisce gli elementi di due liste in un'unica nuova lista.
 * Le liste originali <b>NON</b> vengono modificate.
 * E' possibile personalizzare il processo di clonazione attraverso la funzione <i>clone</i> passata come parametro.
 */
blinked_list* bl_concatenateTwoLists(blinked_list* l1, blinked_list* l2, void* (*clone)(void*)) {
	blinked_list* new_list = bl_cloneOrderedList(l1, clone);
	bl_insertAllElementsLast(new_list, bl_cloneOrderedList(l2, clone));
	return new_list;
}

// Sorting List

/**
 * Scambia di posto due elementi della lista, date le loro posizioni.
 */
void bl_swapTwoElements(blinked_list* l, int pos1, int pos2) {
	if (!bl_checkPositionValidity(l, pos1)) {
		UNVALID_POSITION_ERROR(pos1);
	} else if (!bl_checkPositionValidity(l, pos2)) {
		UNVALID_POSITION_ERROR(pos2);
	} else if (pos1 == pos2) {
		// Dafuq?
	} else {
		if (pos1 > pos2) {
			INT_SWAP(pos1, pos2);		// Mi assicuro che pos1 < pos2
		}
		// Scambio i due elementi
		void* aux = bl_extractElementAtPosition(l, pos1);
		bl_insertElementAtPosition(l, bl_extractElementAtPosition(l, pos2 - 1), pos1);
		bl_insertElementAtPosition(l, aux, pos2);
		// TODO Better version with linkers
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
void bl_sortByOrder(blinked_list* l, int (*compare)(void*, void*)) {
	// Implementazione di un Bubble Sort per l'ordinamento della lista
	for (int i = 0; i < l->size; i++) {
		for (int j = l->size - 1; j > i; j--) {
			if (compare(bl_getElementAtPosition(l, j), bl_getElementAtPosition(l, j - 1)) < 0) {
				bl_swapTwoElements(l, j, j - 1);
			}
		}
	}
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
static blinked_list_node* bl_getMinimumNode(blinked_list* l, int (*compare)(void*, void*)) {
	blinked_list_node* iterator = l->head->next;
	blinked_list_node* minimum = l->head;
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
void* bl_getMinimumElement(blinked_list* l, int (*compare)(void*, void*)) {
	return bl_getMinimumNode(l, compare)->data;
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
static blinked_list_node* bl_getMaximumNode(blinked_list* l, int (*compare)(void*, void*)) {
	blinked_list_node* iterator = l->head->next;
	blinked_list_node* maximum = l->head;
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
void* bl_getMaximumElement(blinked_list* l, int (*compare)(void*, void*)) {
	return bl_getMaximumNode(l, compare)->data;
}

// Visualizing List

/**
 * Restituisce una stringa che rappresenta gli elementi contenuti all'interno della lista.
 * Per rappresentare ogni singolo nodo viene passata come parametro la funzione che converte in stringa il contenuto
 * di un elemento. E' necessario fornire anche la dimensione massima della stringa di un singolo elemento.
 */
char* bl_listToString(blinked_list* l, char* (*toStringFunction)(void*)) {
	char* s = malloc(sizeof(char) * (strlen(STRING_TITLE) + 5));
	if (!s) {
		MEMORY_ERROR;
	}
	sprintf(s, STRING_TITLE, l->size);
	blinked_list_node* iterator = l->head;
	char* new_elem_string;
	for (int i = 0; i < l->size; i++) {
		// Nella stringa ausiliaria alloco la descrizione dell'elemento corrente
		new_elem_string = toStringFunction(iterator->data);
		// Aumento la dimensione della stringa buffer
		s = realloc(s, strlen(s) + strlen(new_elem_string) + 4);
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









//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
	// Inizializzo la lista
	blinked_list* ll = bl_initList();
	
	// Inizializzo robe
	srand(time(NULL));
	int dim = 10;
	char* str1;
	
	for (int i = 0; i < dim; i++) {
		bl_insertFirstElement(ll, initRandomTarga());
	}
	
	// Stampo la lista
	str1 = bl_listToString(ll, stringifyMyStruct);
	printf("(1) %s\n", str1);
	free(str1);
	
	// FREE
	bl_deleteList(ll);
		
	return 0;
}

/// TESTING VARIO ///
	
/*
// Estraggo il primo elemento
targa* e = bl_getElementAtPosition(my_list, 2);
char* buffer2 = stringifyMyStruct(e);
printf("Elemento letto: %s\n", buffer2);
free(buffer2);

// Stampo la lista
char* str2 = bl_listToString(my_list, stringifyMyStruct);
printf("%s\n", str2);
free(str2);

// Leggo un elemento e lo stampo
targa* my_elem = bl_extractElementAtPosition(my_list, 0);
char* buffer = stringifyMyStruct(my_elem);
printf("Elemento letto: %s\n", buffer);
free(buffer);
free(my_elem);

// Stampo la lista
char* str3 = bl_listToString(my_list, stringifyMyStruct);
printf("%s\n", str3);
free(str3);



// Elimino dalla lista le targhe pari, mantenendole in memoria
bl_purgeElementsByCondition(my_list, hasEvenNumber);	

// Stampo la lista
str1 = bl_listToString(my_list, stringifyMyStruct);
printf("Lista originale mangiucchiata:\n%s\n", str1);
free(str1);
* 

	// Scambio due elementi
	int pos1 = 0, pos2 = 6;
	printf("Scambio gli elementi nelle posizioni %d e %d.\n", pos1, pos2);
	bl_swapTwoElements(my_list, pos1, pos2);


*/
