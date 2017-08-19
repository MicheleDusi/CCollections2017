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
 * Inserisce tutti gli elementi in coda alla prima lista. Gli elementi da aggiungere vengono passati come "blinked_list*".
 * 
 * <i>NOTA:</i> L'utilizzo di questa funzione provoca la cancellazione della seconda lista, pertanto è
 * sconsigliata se si vuole unire due liste differenti mantenendo le liste di partenza in memoria.
 * In sostituzione, è possibile utilizzare la funzione "bl_concatenateTwoLists", che ricopia le liste in questione senza modificare gli originali,
 * utilizzando una funzione di clonazione/copiatura predefinita dall'utente.
 * 
 * <i>NOTA:</i> Pre-condizione per l'utilizzo di questa funzione è che le due liste siano disgiunte: non è possibile, cioè, concatenare 
 * una lista con una sua sottolista (formata da parte dei nodi della prima lista). Allo stesso modo, non è possibile inserire tutti gli elementi
 * di una lista in se stessa per "raddoppiarli". E' possibile, però, clonare prima la lista (o una sua sottolista) e, solo in un secondo momento, 
 * unirla alla lista originale (questo funziona poiché i nodi sono entità distinte, nonostante puntino agli stessi elementi).
 */
void bl_insertAllElementsLast(blinked_list* l, blinked_list* elements) {
	if (l != elements) {
		l->tail->next = elements->head;
		elements->head->prev = l->tail;
		l->size += elements->size;
		free(elements);
	}
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
	if (condition(l->tail->data)) {
		bl_deleteLastElement(l);
	}
}

/**
 * Cancella i nodi di una sottolista identificata da una posizione iniziale (compresa) ed una finale (esclusa).
 * Gli elementi vengono mantenuti in memoria; se si desidera cancellarne il contenuto, è consigliabile
 * usare il metodo "bl_purgeSubList" al posto di questo.
 */
void bl_deleteSubList(blinked_list* l, int start_pos, int end_pos) {
	bl_deleteList(bl_extractSubList(l, start_pos, end_pos));
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
		free(l->tail);
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

void bl_purgeSubList(blinked_list* l, int start_pos, int end_pos) {
	bl_purgeList(bl_extractSubList(l, start_pos, end_pos));
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

/**
 * Restituisce una sotto-lista con tutti gli elementi che soddisfano una data condizione.
 * Gli elementi sono esattamente gli stessi (i puntatori puntano agli stessi elementi della lista originale),
 * ma la "struct" lista è differente: sarà perciò sufficiente pulirla con il metodo "bl_deleteList" al termine del suo utilizzo.
 */
blinked_list* bl_getElementsByCondition(blinked_list* l, bool (*condition)(void*)) {
	blinked_list* sublist = bl_initList();
	blinked_list_node* iterator = l->head;
	// Ciclo su tutta la lista originale
	for (int i = 0; i < l->size; i++) {
		// Se un elemento soddisfa la condizione
		if (condition(iterator->data)) {
			bl_insertLastElement(sublist, iterator->data); // Inserisco l'elemento in coda
		}
		iterator = iterator->next;
	}	
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
	// Controlli sulle posizioni
	if (!bl_checkPositionValidity(l, start_pos)) {
		UNVALID_POSITION_ERROR(start_pos);
	} else if (!bl_checkPositionValidity(l, end_pos)) {
		UNVALID_POSITION_ERROR(end_pos);
	} else if (start_pos >= end_pos) {
		UNVALID_POSITION_ERROR(end_pos);
	} else {
		// Inizializzazione di una lista vuota
		blinked_list* sublist = malloc(sizeof(blinked_list));
		if (!sublist) {
			MEMORY_ERROR;
		}
		// Linking della sotto-lista
		sublist->size = end_pos - start_pos;
		sublist->head = bl_getNodeAtPosition(l, start_pos);
		sublist->tail = sublist->head;
		for (int i = 1; i < sublist->size; i++) {
			sublist->tail = sublist->tail->next;
		}
		return sublist;
	}
	return NULL;
}

// Extracting Elements

/**
 * Restituisce il contenuto del primo elemento, rimuovendolo dalla lista.
 */
void* bl_extractFirstElement(blinked_list* l) {
	if (l->size == EMPTY_SIZE) { // Lista vuota
		EMPTY_SIZE_ERROR;
		return NULL;
	} else if (l->size == 1) { // Lista con un singolo elemento
		void* aux = l->head->data;
		free(l->head);
		l->head = NULL;
		l->tail = NULL;
		l->size--;
		return aux;
	} else {					// Lista con almeno due elementi
		void* aux = l->head->data;
		l->head = l->head->next;
		free(l->head->prev);
		l->head->prev = NULL;
		l->size--;
		return aux;
	}
}

/**
 * Estrae dalla lista l'ultimo elemento, e lo restituisce come valore di ritorno.
 */
void* bl_extractLastElement(blinked_list* l) {
	if (l->size == EMPTY_SIZE) { // Lista vuota
		EMPTY_SIZE_ERROR;
		return NULL;
	} else if (l->size == 1) {	// Lista con un singolo elemento
		void* aux = l->tail->data;
		free(l->tail);
		l->tail = NULL;
		l->head = NULL;
		l->size--;
		return aux;
	} else {					// Lista con almeno due elementi
		void* aux = l->tail->data;
		l->tail = l->tail->prev;
		free(l->tail->next);
		l->tail->next = NULL;
		l->size--;
		return aux;
	}
}

/**
 * Estrae un elemento alla posizione desiderata, lo cancella dalla lista e lo restituisce come puntatore.
 * Il primo elemento della lista ha posizione "0".
 */
void* bl_extractElementAtPosition(blinked_list* l, int pos) {
	blinked_list_node* extracted = bl_extractNodeAtPosition(l, pos);
	if (extracted == NULL) {
		return NULL;
	} else {
		void* aux = extracted->data;
		free(extracted);
		return aux;
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
	bl_insertInitialElement(extracted_list, NULL); // NODO FAKE per agevolare le operazioni
	// Ciclo su tutti gli elementi NON estremi
	blinked_list_node* iterator = l->head;
	int size = l->size;
	for (int i = 1; i < size - 1; i++) {
		if (condition(iterator->next->data)) {
			extracted_list->tail->next = iterator->next;
			iterator->next = iterator->next->next;
			iterator->next->prev = iterator;
			extracted_list->tail->next->prev = extracted_list->tail;
			extracted_list->tail = extracted_list->tail->next;
			extracted_list->size++;
			l->size--;
		} else {
			iterator = iterator->next;
		}
	}
	// Rimuovo il nodo fake
	bl_deleteFirstElement(extracted_list);
	// Controllo testa e coda
	if (condition(l->head->data)) {
		bl_insertFirstElement(extracted_list, bl_extractFirstElement(l));
	}
	if (condition(l->tail->data)) {
		bl_insertLastElement(extracted_list, bl_extractLastElement(l));
	}
	// Ritorno
	return extracted_list;
}

char* stringifyMyStruct(void* obj);

blinked_list* bl_extractSubList(blinked_list* l, int start_pos, int end_pos) {
	if (!bl_checkPositionValidity(l, start_pos)) {
		UNVALID_POSITION_ERROR(start_pos);
	} else if (!bl_checkPositionValidity(l, end_pos)) {
		UNVALID_POSITION_ERROR(end_pos);
	} else if (start_pos == end_pos) {
		UNVALID_POSITION_ERROR(end_pos);
	} else {
		blinked_list* sublist = bl_initList();
		
		if (start_pos > end_pos) {
			INT_SWAP(start_pos, end_pos);		// Mi assicuro che pos1 < pos2
		}
		
		// Seleziono i due nodi
		if (2 * start_pos >= end_pos && start_pos + end_pos >= l->size) { 
			// Scorro dalla fine per entrambi
			sublist->tail = l->tail;
			for (int i = l->size - 1; i > end_pos - 1; i--) {
				sublist->tail = sublist->tail->prev;
			}
			sublist->head = sublist->tail;
			for (int i = end_pos - 1; i > start_pos; i--) {
				sublist->head = sublist->head->prev;
			}
		} else if (2 * end_pos < l->size + start_pos && l->size > start_pos + end_pos) { 
			// Scorro dall'inizio per entrambi
			sublist->head = l->head;
			for (int i = 0; i < start_pos; i++) {
				sublist->head = sublist->head->next;
			}
			sublist->tail = sublist->head;
			for (int i = start_pos; i < end_pos - 1; i++) {
				sublist->tail = sublist->tail->next;
			}
		} else { 
			// Scorro ciascuno dal rispettivo estremo
			sublist->head = l->head;
			for (int i = 0; i < start_pos; i++) {
				sublist->head = sublist->head->next;
			}
			sublist->tail = l->tail;
			for (int i = l->size - 1; i > end_pos - 1; i--) {
				sublist->tail = sublist->tail->prev;
			}
		}
				
		if (start_pos == 0) {
			l->head = sublist->tail->next;
		} else {
			sublist->head->prev->next = sublist->tail->next;
		}
		if (end_pos == l->size) {
			l->tail = sublist->head->prev;
		} else {
			sublist->tail->next->prev = sublist->head->prev;
		}
		
		sublist->head->prev = NULL;
		sublist->tail->next = NULL;
		sublist->size = end_pos - start_pos;
		l->size -= sublist->size;
		return sublist;
	}
	return NULL;
}

// Searching Elements

/**
 * Verifica che all'interno della lista sia presente un elemento dato come parametro.
 * Poiché non si hanno elementi per sapere se l'elemento ha più probabilità di trovarsi in testa o in coda, 
 * viene scelto arbitrariamente di scorrere la lista a partire dalla testa.
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
		if (condition(iterator->data)) {
			return true;
		}
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
int bl_getPositionOfElement(blinked_list* l, void* element_content) {
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
	// Clono la lista
	if (l->size != EMPTY_SIZE) {
		blinked_list_node* iterator = l->head;
		for (int i = 0; i < l->size; i++) {
			bl_insertLastElement(new_list, clone(iterator->data));
			iterator = iterator->next;
		}
	}
	// Fine
	return new_list;
}

/**
 * Restituisce una sottolista che parte dall'elemento di indice start_pos fino all'elemento di indice end_pos;
 * La lista originale non viene modificata, e ogni singolo elemento viene clonato dalla funzione passata come parametro.
 */
blinked_list* bl_cloneSubList(blinked_list* l, int start_pos, int end_pos, void* (*clone)(void*)) {
	// Controlli sulle posizioni
	if (!bl_checkPositionValidity(l, start_pos)) {
		UNVALID_POSITION_ERROR(start_pos);
	} else if (!bl_checkPositionValidity(l, end_pos - 1)) {
		UNVALID_POSITION_ERROR(end_pos);
	} else if (start_pos >= end_pos) {
		UNVALID_POSITION_ERROR(end_pos);
	} else {
		// Inizializzazione di una lista vuota
		blinked_list* sublist = bl_initList();
		// Clonazione della sotto-lista
		blinked_list_node* iterator = bl_getNodeAtPosition(l, start_pos);
		for (int i = start_pos; i < end_pos; i++) {
			bl_insertLastElement(sublist, clone(iterator->data));
			iterator = iterator->next;
		}
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

void bl_mapList(blinked_list* l, void* (*map)(void*)) {
	/// TODO
	// DECIDERE COSA FARE CON GLI ELEMENTI ORIGINALI
	// Cioè, una volta applicata la mappa modificherò l'elemento originale: devo eliminarlo?
}

// Managing and Modifying List

/**
 * Scambia di posto due elementi della lista, date le loro posizioni.
 * Attenzione: la funzione NON scambia di posto i nodi contenenti gli elementi; pertanto, è possibile
 * utilizzarla in combinazione con le sotto-liste, poiché non dovrebbe (!) generare side-effects non previsti.
 */
void bl_swapTwoElements(blinked_list* l, int pos1, int pos2) {
	if (!bl_checkPositionValidity(l, pos1)) {
		UNVALID_POSITION_ERROR(pos1);
	} else if (!bl_checkPositionValidity(l, pos2)) {
		UNVALID_POSITION_ERROR(pos2);
	} else if (pos1 == pos2) {
		UNVALID_POSITION_ERROR(pos2);
	} else {
		blinked_list_node* aux1;
		blinked_list_node* aux2; // Punterranno rispettivamente ai due nodi con gli elementi da scambiare.
		
		if (pos1 > pos2) {
			INT_SWAP(pos1, pos2);		// Mi assicuro che pos1 < pos2
		}
		
		// Ora, sulla base delle posizioni, scelgo il metodo migliore per raggiungere e selezionare i nodi desiderati
		if (2 * pos1 >= pos2 && pos1 + pos2 >= l->size) { // Scorro dalla fine per entrambi
			aux2 = l->tail;
			for (int i = l->size - 1; i > pos2; i--) {
				aux2 = aux2->prev;
			}
			aux1 = aux2;
			for (int i = pos2; i > pos1; i--) {
				aux1 = aux1->prev;
			}
		} else if (2 * pos2 < l->size + pos1 && l->size > pos1 + pos2) { // Scorro dall'inizio per entrambi
			aux1 = l->head;
			for (int i = 0; i < pos1; i++) {
				aux1 = aux1->next;
			}
			aux2 = aux1;
			for (int i = pos1; i < pos2; i++) {
				aux2 = aux2->next;
			}
		} else { // Scorro ciascuno dal rispettivo estremo
			aux1 = l->head;
			for (int i = 0; i < pos1; i++) {
				aux1 = aux1->next;
			}
			aux2 = l->tail;
			for (int i = l->size - 1; i > pos2; i--) {
				aux2 = aux2->prev;
			}
		}
		
		// Scambio i contenuti
		void* content_aux = aux1->data;
		aux1->data = aux2->data;
		aux2->data = content_aux;
	}
}

/**
 * Inverte l'ordine dei nodi, senza modificare la dimensione della lista.
 */
void bl_reverseList(blinked_list* l) {
	if (l->size == EMPTY_SIZE) {
		EMPTY_SIZE_ERROR;
	} else if (l->size != 1) { // In caso di elemento singolo non ha senso invertire la lista
		blinked_list_node* iterator = l->head;
		blinked_list_node* aux = NULL;
		for (int i = 0; i < l->size; i++) {
			aux = iterator->prev;
			iterator->prev = iterator->next;
			iterator->next = aux;
			
			iterator = iterator->prev;
		}
		l->tail = l->head;
		l->head = aux->prev;
	}
}

/**
 * Funzione d'utilità.
 */
static void bl_makeListCircular(blinked_list* l) {
	if (l->size == EMPTY_SIZE) {
		EMPTY_SIZE_ERROR;
	} else {
		l->head->prev = l->tail;
		l->tail->next = l->head;
	}
}

/**
 * "Shifta" tutti gli elementi della lista di un numero di posizioni arbitrario.
 * Durante l'esecuzione di questa funzione, la lista viene momentaneamente convertita in un anello,
 * e il puntatore all'elemento iniziale si sposta in avanti o all'indietro a seconda della variabile "shift".
 * 
 * Per valori di shift positivi, gli elementi verranno spostati "avanti" (ossia, il loro indice verrà aumentato di una
 * quantità pari a shift). Per valori negativi, gli elementi verranno invece spostati "indietro".
 * Il tutto, ovviamente, viene effettuato in modulo (size); pertanto, con un numero di iterazioni sufficiente, è possibile
 * ottenere il medesimo effetto sia con valori positivi che negativi.
 * La funzione, però, è formata in modo da scegliere autonomamente la direzione per la quale lo spostamento risulti più
 * efficiente e veloce (ossia quello che richiede meno iterazioni).
 */
void bl_shiftListBy(blinked_list* l, int shift) {
	// Modifico la lista in "anello"
	bl_makeListCircular(l);
	shift = ((shift % l->size) + l->size) % l->size; // Questo mi assicura che il valore di shift sia tra (0) e (size-1) compresi.
	if (shift <= l->size / 2) { 
		for (int i = 0; i < shift; i++) {
			l->head = l->head->prev; // Scorro in avanti
		}
	} else {
		for (int i = 0; i < l->size - shift; i++) {
			l->head = l->head->next; // Scorro all'indietro
		}
	}
	l->tail = l->head->prev;
	// Apro l'anello e torno alla configurazione "lista"
	l->tail->next = NULL;
	l->head->prev = NULL;
}

// Sorting List

/**
 * Ordina una lista in modo <i>crescente</i> secondo una relazione d'ordine definita dall'utente e passata come parametro.
 * La relazione deve essere implementata come una funzione che prende in ingresso il contenuto di due nodi, 
 * e li confronta restituendo:
 * - un numero negativo se il primo dato è "minore" del secondo (stando alla relazione).
 * - 0 se i due dati sono considerati uguali dalla relazione d'ordine.
 * - un numero positivo se il primo dato è "maggiore" del secondo (stando alla relazione).
 * 
 * L'algoritmo di ordinamento implementato è un Insertion Sort.
 * Poichè lo spostamento di elementi agisce sul campo "data" delle strutture "blinked_list_node", è possibile
 * utilizzare questa funzione su sottoliste ottenute da "bl_getSubList". Questo permette di ordinare solamente
 * una porzione della lista originale.
 */
void bl_sortByOrder(blinked_list* l, int (*compare)(void*, void*)) {
	if (l->size > 1) {
		blinked_list_node* iterator = l->head->next;
		blinked_list_node* sub_iterator = NULL;
		void* aux = NULL;
		for (int i = 1; i < l->size; i++) {
			aux = iterator->data; // Elemento da inserire
			sub_iterator = iterator;
			while (sub_iterator != l->head && compare(aux, sub_iterator->prev->data) < 0) {
				sub_iterator->data = sub_iterator->prev->data;
				sub_iterator = sub_iterator->prev;
			}
			sub_iterator->data = aux;
			iterator = iterator->next;
		}
	}
}

void bl_sortByHash(blinked_list* l, int (*hashingFunction)(void*)) {
	/// TODO
	// Questa è una futura (molto futura) implementazione.
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
	blinked_list_node* minimum = l->head;
	blinked_list_node* iterator = l->head->next;
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
 * 
 * La differenza principale con la ricerca del minimo è che, in questo caso, la lista viene attraversata
 * dalla coda alla testa. Questo perchè, in caso sia stata recentemente ordinata, sarà più veloce trovare l'elemento massimo.
 * Questa è solo una considerazione probabilistica, non è nulla di certo. La ricerca in una lista ordinata richiede, nel caso peggiore,
 * sempre O(n) passaggi.
 */
static blinked_list_node* bl_getMaximumNode(blinked_list* l, int (*compare)(void*, void*)) {
	blinked_list_node* maximum = l->tail;
	blinked_list_node* iterator = l->tail->prev;
	for (int i = 1; i < l->size; i++) {
		if (compare(iterator->data, maximum->data) > 0) {
			maximum = iterator;
		}
		iterator = iterator->prev;
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
		bl_insertLastElement(ll, initRandomTarga());
	}
	
	// Stampo la lista
	str1 = bl_listToString(ll, stringifyMyStruct);
	printf("(1) %s\n", str1);
	free(str1);
	
	blinked_list* extr = bl_extractElementsByCondition(ll, hasEvenNumber);
	
	// Stampo la lista
	str1 = bl_listToString(ll, stringifyMyStruct);
	printf("(1) %s\n", str1);
	free(str1);
	
	// Stampo la lista
	str1 = bl_listToString(extr, stringifyMyStruct);
	printf("(1) %s\n", str1);
	free(str1);
	
	// FREE
	bl_purgeList(ll);
	bl_purgeList(extr);
		
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

// Scambio avanzato di elementi (con controllo saltelli)

for(int a = 0; a < dim; a++) {
		for (int b = 0; b < dim; b++) {
			printf("%d %d - ", a, b);
			bl_swapTwoElements(ll, a, b);
		}
	}
	
	// Stampo la lista
	str1 = bl_listToString(ll, stringifyMyStruct);
	printf("(1) %s\n", str1);
	free(str1);


// Ordinamento di una sottolista

	
	blinked_list* sublist = bl_getSubList(ll, 2, 8);
	
	
	str1 = bl_listToString(sublist, stringifyMyStruct);
	printf("(1) %s\n", str1);
	free(str1);
	
	bl_sortByOrder(sublist, compareMyStruct);
	
	
	str1 = bl_listToString(sublist, stringifyMyStruct);
	printf("(1) %s\n", str1);
	free(str1);
	
	// Stampo la lista
	str1 = bl_listToString(ll, stringifyMyStruct);
	printf("(1) %s\n", str1);
	free(str1);
	
	free(sublist);

*/
