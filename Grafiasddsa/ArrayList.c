#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include "ArrayList.h"

#define INCREASING_FACTOR 2
#define INCREASING_CHECK 0.75
#define DECREASING_FACTOR 0.5
#define DECREASING_CHECK 0.25
#define DEFAULT_CAPACITY 10
#define MINIMUM_CAPACITY DEFAULT_CAPACITY * DECREASING_FACTOR

#ifndef MEMORY_ERROR
#	define MEMORY_ERROR printf("Error: Cannot allocate memory.\n"); exit(1)
#endif

#ifndef UNVALID_POSITION_ERROR
#	define UNVALID_POSITION_ERROR(pos) printf("Error: The position %d is unvalid.\n", pos )
#endif

#ifndef EMPTY_SIZE
#	define EMPTY_SIZE 0
#	define EMPTY_SIZE_ERROR(instr) printf("Error: Cannot execute \"%s\" function on empty list.", instr )
#endif

#ifndef STRING_FORMATTING
#	define STRING_FORMATTING
	const char* STRING_TITLE = "LISTA [size: %d]\n";
	const int STRING_TITLE_LENGTH = 26;
#endif

/**
 * Libreria che implementa una lista come array e fornisce numerose funzioni che
 * ne permettono un utilizzo semplificato.
 * 
 * @author Michele Dusi <michele.dusi.it@ieee.org>
 * 
 */

///// Initializing List

/**
 * Inizializzazione di una lista vuota con capacità iniziale personalizzata.
 */
arraylist* al_initListWithCapacity(int cap) {
	arraylist* new_list = malloc(sizeof(arraylist));
	if (!new_list) {
		MEMORY_ERROR;
	}
	new_list->size = 0;
	new_list->capacity = cap > MINIMUM_CAPACITY ? cap : MINIMUM_CAPACITY;
	new_list->array = malloc(new_list->capacity * sizeof(void*));
	if (!(new_list->array)) {
		MEMORY_ERROR;
	}
	return new_list;
}

/** 
 * Inizializzazione della lista vuota con capacità iniziale di default.
 */
arraylist* al_initList() {
	return al_initListWithCapacity(DEFAULT_CAPACITY);
}

// Size

/**
 * Restituisce la quantità di elementi presenti nella lista.
 */
int al_getListSize(arraylist* l) {
	return l->size;
}

// Cancelling List

/**
 * Elimina la lista passata come parametro, liberando le zone di memoria occupate
 * dalla struttura ArrayList e dal vettore interno.
 * <b>NON</b> elimina gli oggetti a cui i puntatori nel vettore puntano.
 */
void al_cleanList(arraylist* l) {
	free(l->array);
	free(l);
}

/**
 * Elimina la lista passata come parametro, liberando le zone di memoria occupate
 * dalla struttura ArrayList, dal vettore interno e dagli oggetti contenuti all'interno del vettore.
 * Questo significa che essi non saranno più utilizzabili dopo la chiamta a funzione.
 */
void al_purgeList(arraylist* l) {
	for (int i = 0; i < l->size; i++) {
		free(l->array[i]);
	}
	free(l->array);
	free(l);
}

// Static Utility Functions

/**
 * Controlla la dimensione dell'arraylist dopo che è stata effettuata un'aggiunta di un elemento.
 * In caso la capacità debba essere aumentata, viene re-allocato l'array in una porzione di memoria maggiore.
 */
static void al_checkAndIncreaseCapacity(arraylist* l) {
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
static void al_checkAndDecreaseCapacity(arraylist* l) {
	if (l->size < l->capacity * DECREASING_CHECK
			&& l->capacity > MINIMUM_CAPACITY) {
		int new_capacity = (int)(l->capacity * DECREASING_FACTOR);
		l->array = realloc(l->array, new_capacity * sizeof(void*));
		l->capacity = new_capacity;
	}
}

/**
 * Controlla che la posizione inserita sia ammissibile nella lista.
 */
static bool al_checkPositionValidity(arraylist* l, int pos) {
	return pos >= 0 && pos < l->size;
}

// Inserting Elements

/** 
 * Inserimento di un elemento in testa alla lista.
 */
void al_insertFirstElement(arraylist* l, void* new_element_data) {
	for (int i = l->size; i > 0; i--) {
		l->array[i] = l->array[i - 1];
	}
	l->array[0] = new_element_data;
	l->size++;
	al_checkAndIncreaseCapacity(l);
}

/**
 * Inserimento di un elemento in coda alla lista.
 */
void al_insertLastElement(arraylist* l, void* new_element_data) {
	l->array[l->size] = new_element_data;
	l->size++;
	al_checkAndIncreaseCapacity(l);
}

/**
 * Inserisce un elemento alla posizione desiderata nella lista.
 * Tutti gli altri elementi successivi vengono spostati in avanti di una posizione.
 */
void al_insertElementAtPosition(arraylist* l, void* new_element_data, int pos) {
	if (!al_checkPositionValidity(l, pos)) {
		UNVALID_POSITION_ERROR(pos);
	} else {
		for (int i = l->size; i > pos; i--) {
			l->array[i] = l->array[i - 1];
		}
		l->array[pos] = new_element_data;
		l->size++;
		al_checkAndIncreaseCapacity(l);
	}
}

// Deleting Elements

/**
 * Elimina l'elemento in prima posizione.
 * Se la lista è vuota, viene generato un messaggio a video e non viene effettuata alcuna eliminazione.
 */
void al_deleteFirstElement(arraylist* l) {
	if (l->size == EMPTY_SIZE) {
		EMPTY_SIZE_ERROR("deleteFirstElement");
	} else {
		for (int i = 0; i < l->size - 1; i++) {
			l->array[i] = l->array[i + 1];
		}
		l->size--;
		al_checkAndDecreaseCapacity(l);
	}
}

/**
 * Elimina l'elemento in ultima posizione.
 * Se la lista è vuota, viene generato un messaggio a video e non viene effettuata alcuna eliminazione.
 */
void al_deleteLastElement(arraylist* l) {
	if (l->size == EMPTY_SIZE) {
		EMPTY_SIZE_ERROR("deleteLastElement");
	} else {
		l->size--;
		al_checkAndDecreaseCapacity(l);
	}
}

/**
 * Rimuove un elemento alla posizione desiderata.
 */
void al_deleteElementAtPosition(arraylist* l, int pos) {
	if (l->size == EMPTY_SIZE) {
		EMPTY_SIZE_ERROR("deleteElementAtPosition");
	} else if (!al_checkPositionValidity(l, pos)) {
		UNVALID_POSITION_ERROR(pos);
	} else {
		for (int i = pos; i < l->size - 1; i++) {
			l->array[i] = l->array[i + 1];
		}
		l->size--;
		al_checkAndDecreaseCapacity(l);
	}
}

/**
 * Rimuove tutti gli elementi che soddisfano una data condizione.
 * Attenzione: gli elementi vengono rimossi dalla lista, ma non dalla memoria. In caso non esistano puntatori ad essi,
 * questi verrebbero persi irrimediabilmente e la memoria verrebbe inutilmente sprecata.
 * Se si vogliono eliminare gli elementi stessi, si consiglia di utilizzare il metodo "al_purgeElementsByCondition".
 * Se invece si vogliono ottenere gli elementi cancellati dalla lista, utilizzare il metodo "al_extractElementsByCondition".
 */
void al_deleteElementsByCondition(arraylist* l, bool (*condition)(void*)) {
	for (int i = 0; i < l->size;) {
		if (condition(l->array[i])) {
			l->size--;
			for (int j = i; j < l->size; j++) {
				l->array[j] = l->array[j + 1];
			}
		} else {
			i++;
		}
	}
	al_checkAndDecreaseCapacity(l);
}

// Purging Elements

/**
 * Elimina il primo elemento dalla lista e dalla memoria.
 * I puntatori eventualmente inizializzati in precedenza alla porzione di memoria relativa
 * non potranno più essere utilizzati dopo la chiamata a questa funzione.
 */
void al_purgeFirstElement(arraylist* l) {
	if (l->size == EMPTY_SIZE) {
		EMPTY_SIZE_ERROR("purgeFirstElement");
	} else {
		free(l->array[0]);
		l->size--;
		for (int i = 0; i < l->size; i++) {
			l->array[i] = l->array[i + 1];
		}
		al_checkAndDecreaseCapacity(l);
	}
}

/**
 * Elimina l'ultimo elemento dalla lista e dalla memoria.
 * I puntatori eventualmente inizializzati in precedenza alla porzione di memoria relativa
 * non potranno più essere utilizzati dopo la chiamata a questa funzione.
 */
void al_purgeLastElement(arraylist* l) {
	if (l->size == EMPTY_SIZE) {
		EMPTY_SIZE_ERROR("purgeLastElement");
	} else {
		free(l->array[l->size - 1]);
		l->size--;
		al_checkAndDecreaseCapacity(l);
	}
}

/**
 * Elimina l'elemento alla posizione desiderata dalla lista e dalla memoria.
 * I puntatori eventualmente inizializzati in precedenza alla porzione di memoria relativa
 * non potranno più essere utilizzati dopo la chiamata a questa funzione.
 */
void al_purgeElementAtPosition(arraylist* l, int pos) {
	if (l->size == EMPTY_SIZE) {
		EMPTY_SIZE_ERROR("purgeElementAtPosition");
	} else if (!al_checkPositionValidity(l, pos)) {
		UNVALID_POSITION_ERROR(pos);
	} else {
		free(l->array[pos]);
		l->size--;
		for (int i = pos; i < l->size; i++) {
			l->array[i] = l->array[i + 1];
		}
		al_checkAndDecreaseCapacity(l);
	}
}

/**
 * Elimina tutti gli elementi della lista che soddisfano una data condizione.
 * Il contenuto degli elementi viene rimosso anche dalla memoria.
 */
void al_purgeElementsByCondition(arraylist* l, bool (*condition)(void*)) {
	al_purgeList(al_extractElementsByCondition(l, condition));
}

// Getting Elements

/**
 * Restituisce il contenuto del primo elemento della lista.
 */
void* al_getFirstElement(arraylist* l) {
	if (l->size == EMPTY_SIZE) {
		EMPTY_SIZE_ERROR("getFirstElement");
		return NULL;
	} else {
		return l->array[0];
	}
}

/**
 * Restituisce il contenuto dell'ultimo elemento della lista.
 * Se la lista è vuota, restituisce NULL.
 */
void* al_getLastElement(arraylist* l) {
	if (l->size == EMPTY_SIZE) {
		EMPTY_SIZE_ERROR("getLastElement");
		return NULL;
	} else {
		return l->array[l->size - 1];
	}
}

/**
 * Restituisce il contentuto di un elemento alla posizione desiderata.
 */
void* al_getElementAtPosition(arraylist* l, int pos) {
	if (!al_checkPositionValidity(l, pos)) {
		UNVALID_POSITION_ERROR(pos);
		return NULL;
	} else {
		return l->array[pos];
	}
}

/**
 * Restituisce una lista con i puntatori agli elementi che soddisfano la condizione
 * passata come parametro. L'operazione non ha effetto sulla lista originaria; nonostante
 * ciò, un'eventuale rimozione in memoria avrà effetto su entrambe le liste (poichè gli oggetti
 * contenuti in esse non sono clonati, ma sono gli stessi).
 */
arraylist* al_getElementsByCondition(arraylist* l, bool (*condition)(void*)) {
	arraylist* sublist = al_initListWithCapacity(l->size + 1);
	for (int i = 0; i < l->size; i++) {
		if (condition(l->array[i])) {
			sublist->array[sublist->size] = l->array[i];
			sublist->size++;
		}
	}
	al_checkAndDecreaseCapacity(sublist);
	return sublist;
}

/**
 * Restituisce la sottolista che parte dall'elemento di indice start_pos all'elemento di end_pos.
 * L'elemento end_pos è <emph>escluso</emph>, mentre viene incluso l'elemento start_pos.
 * 
 * <i>NOTA:</i> Non viene creata una nuova lista, gli elementi della lista originaria non vengono modificati.
 * Pertanto un qualsiasi cambiamento alla lista originaria può provocare modifiche alla sottolista, e viceversa.
 * 
 * <i>NOTA:</i>Si consiglia di NON fare operazioni sugli elementi della sottolista: aggiunta o rimozione di elementi potrebbe comportare la corruzione
 * dell'intera lista originaria. Questa funzione restituisce una lista comoda unicamente per operazioni di lettura.
 */
arraylist* al_getSubList(arraylist* l, int start_pos, int end_pos) {
	if (!al_checkPositionValidity(l, start_pos)) {
		UNVALID_POSITION_ERROR(start_pos);
	} else if (!al_checkPositionValidity(l, end_pos)) {
		UNVALID_POSITION_ERROR(end_pos);
	} else if (end_pos <= start_pos) {
		UNVALID_POSITION_ERROR(end_pos);
	} else {
		arraylist* sublist = malloc(sizeof(arraylist));
		if (!sublist) {
			MEMORY_ERROR;
		}
		sublist->size = end_pos - start_pos;
		sublist->capacity = l->capacity - start_pos;
		sublist->array = &(l->array[start_pos]);
		return sublist;
	}
	return NULL;
}

// Extracting Elements

/**
 * Restituisce il contenuto del primo elemento, eliminandolo dalla lista.
 */
void* al_extractFirstElement(arraylist* l) {
	void* aux = l->array[0];
	l->size--;
	for (int i = 0; i < l->size; i++) {
		l->array[i] = l->array[i + 1];
	}
	al_checkAndDecreaseCapacity(l);
	return aux;
}

/**
 * Restituisce il contenuto dell'ultimo elemento, eliminandolo dalla lista.
 */
void* al_extractLastElement(arraylist* l) {
	if (l->size == EMPTY_SIZE) {
		EMPTY_SIZE_ERROR("extractLastElement");
		return NULL;
	} else {
		l->size--;
		al_checkAndDecreaseCapacity(l); // TODO Controllare che questo non corrompa MAI l'ultimo elemento (che ormai non è più considerato nella size).
		return l->array[l->size];
	}
}

/**
 * Estrae un elemento alla posizione desiderata, lo cancella dalla lista e lo restituisce come puntatore.
 * Il primo elemento della lista ha posizione "0".
 */
void* al_extractElementAtPosition(arraylist* l, int pos) {
	if (!al_checkPositionValidity(l, pos)) {
		UNVALID_POSITION_ERROR(pos);
		return NULL;
	} else {
		void* aux = l->array[pos];
		l->size--;
		for (int i = pos; i < l->size; i++) {
			l->array[i] = l->array[i + 1];
		}
		al_checkAndDecreaseCapacity(l);
		return aux;
	}
}

/** 
 * Data una condizione booleana, estrae dalla lista tutti gli elementi che soddisfano quella condizione,
 * restituendoli all'interno di una nuova lista.
 */
arraylist* al_extractElementsByCondition(arraylist* l, bool (*condition)(void*)) {
	arraylist* extracted_list = al_initListWithCapacity(l->size);
	for (int i = 0; i < l->size; i++) {
		if (condition(l->array[i])) {
			void* aux = al_extractElementAtPosition(l, i);
			al_insertLastElement(extracted_list, aux);
		}
	}
	al_checkAndDecreaseCapacity(extracted_list);
	return extracted_list;
}

// Searching Elements

/**
 * Restituisce TRUE se la lista contiene il puntatore passato come parametro.
 */
bool al_containsElement(arraylist* l, void* element_content) {
	for (int i = 0; i < l->size; i++) {
		if (l->array[i] == element_content) {
			return true;
		}
	}
	return false;
}

/**
 * Verifica che all'interno della lista sia presente almeno un elemento che soddisfi una data condizione.
 */
bool al_containsElementByCondition(arraylist* l, bool (*condition)(void*)) {
	for (int i = 0; i < l->size; i++) {
		if (condition(l->array[i])) {
			return true;
		}
	}
	return false;
}

/**
 * Restituisce il numero di elementi della lista che soddisfano una data condizione.
 */
int al_countElementsByCondition(arraylist* l, bool (*condition)(void*)) {
	int count = 0;
	for (int i = 0; i < l->size; i++) {
		if (condition(l->array[i])) {
			count++;
		}
	}
	return count;
}

/**
 * Restituisce la posizione dell'elemento corrispondente a quello cercato.
 * Se l'elemento non è presente all'interno della lista, viene restituito il valore -1.
 */
int al_getElementPosition(arraylist* l, void* element_content) {
	for (int i = 0; i < l->size; i++) {
		if (element_content == l->array[i]) {
			return i;
		}
	}
	return -1;
}

// Cloning and Managing List

/**
 * Clona una lista, data in ingresso una funzione per la clonazione del contenuto di un elemento.
 * Garantisce il mantenimento dell'ordine durante il processo.
 */
arraylist* al_cloneOrderedList(arraylist* l, void* (*clone)(void*)) {
	// Inizializzo la nuova lista
	arraylist* new_list = al_initListWithCapacity(l->capacity);
	// Clonazione
	for (int i = 0; i < l->size; i++) {
		new_list->array[i] = clone(l->array[i]);
	}
	new_list->size = l->size;
	// Fine
	return new_list;
}

/**
 * Restituisce una sottolista che parte dall'elemento di indice start_pos fino all'elemento di indice end_pos;
 * La lista originale non viene modificata, e ogni singolo elemento viene clonato dalla funzione passata come parametro.
 */
arraylist* al_cloneSubList(arraylist* l, int start_pos, int end_pos, void* (*clone)(void*)) {
	return al_cloneOrderedList(al_getSubList(l, start_pos, end_pos), clone);
}

/**
 * Unisce gli elementi di due liste in un'unica nuova lista.
 * Le liste originali <b>NON</b> vengono modificate.
 * E' possibile personalizzare il processo di clonazione attraverso la funzione <i>clone</i> passata come parametro.
 */
arraylist* al_concatenateTwoLists(arraylist* l1, arraylist* l2, void* (*clone)(void*)) {
	// Inizializzo la lista vuota pronta ad accogliere gli elementi clonati
	arraylist* new_list = al_initListWithCapacity(l1->capacity + l2->capacity);
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

// Sorting List

/**
 * Scambia di posto due elementi della lista, date le loro posizioni.
 */
void al_swapTwoElements(arraylist* l, int pos1, int pos2) {
	if (!al_checkPositionValidity(l, pos1)) {
		UNVALID_POSITION_ERROR(pos1);
	} else if (!al_checkPositionValidity(l, pos2)) {
		UNVALID_POSITION_ERROR(pos2);
	} else if (pos1 >= pos2) {
		UNVALID_POSITION_ERROR(pos2);
	} else {
		void* aux = malloc(sizeof(void*));
		if (!aux) {
			MEMORY_ERROR;
		}
		aux = l->array[pos1];
		l->array[pos1] = l->array[pos2];
		l->array[pos2] = aux;
		free(aux);
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
void al_sortByOrder(arraylist* l, int (*compare)(void*, void*)) {
	// Implemento un BubbleSort
	for (int i = 0; i < l->size; i++) {
		for (int j = l->size - 1; j > i; j--) {
			if (compare(l->array[j], l->array[j - 1]) < 0) {
				void* aux = l->array[j];
				l->array[j] = l->array[j - 1];
				l->array[j -1] = aux;
			}
		}
	}
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
void* al_getMinimumContent(arraylist* l, int (*compare)(void*, void*)) {
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
void* al_getMaximumContent(arraylist* l, int (*compare)(void*, void*)) {
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

// Visualizing List

/**
 * Restituisce una stringa che rappresenta gli elementi contenuti all'interno della lista.
 * Per rappresentare ogni singolo elemento viene passata come parametro la funzione che converte in stringa il contenuto
 * di un elemento.
 */
char* al_listToString(arraylist* l, char* (*toStringFunction)(void*)) {
	// Alloco lo spazio necessario nella stringa di destinazione
	char* final_str = malloc((STRING_TITLE_LENGTH + 1) * sizeof(char));
	if (!final_str) {
		MEMORY_ERROR;
	}
	sprintf(final_str, STRING_TITLE, l->size);
	char* aux_string = " ";
	for (int i = 0; i < l->size; i++) {
		aux_string = toStringFunction(l->array[i]);
		int new_length = strlen(final_str) + strlen(aux_string) + 2;
		final_str = realloc(final_str, new_length * sizeof(char));
		strcat(final_str, aux_string);
		free(aux_string);
	}
	strcat(final_str, "\n");
	return final_str;
}

//////////////////////////////////////////////////////
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

int main(void) {
	arraylist* arr = al_initList();
	
	srand(time(NULL));
	
	int dim = 10;
	
	// Popolo la lista
	for (int i = 0; i < dim; i++) {
		al_insertLastElement(arr, initRandomTarga());
	}
	
	// Stampo la lista
	char* str1 = al_listToString(arr, stringifyMyStruct);
	printf("%s\n", str1);
	free(str1);
	
	
	// Ordino la lista
	al_sortByOrder(arr, compareMyStruct);
	printf("Ho ordinato la lista\n\n");
	
	
	// Stampo la lista
	char* str2 = al_listToString(arr, stringifyMyStruct);
	printf("%s\n", str2);
	free(str2);
	
	
	// Elimino le targhe pari
	al_purgeElementsByCondition(arr, hasEvenNumber);
	printf("Elimino le targhe pari.\n\n");
	
	
	// Stampo la lista
	char* str3 = al_listToString(arr, stringifyMyStruct);
	printf("%s\n", str3);
	free(str3);
	
	
	// FREE
	al_purgeList(arr);
		
	return 0;
}
//////////////////////*/


