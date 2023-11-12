/*
 * Tabulka s rozptýlenými položkami
 *
 * S využitím datových typů ze souboru hashtable.h a připravených koster
 * funkcí implementujte tabulku s rozptýlenými položkami s explicitně
 * zretězenými synonymy.
 *
 * Při implementaci uvažujte velikost tabulky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptylovací funkce která přidělí zadanému klíči index z intervalu
 * <0,HT_SIZE-1>. Ideální rozptylovací funkce by měla rozprostírat klíče
 * rovnoměrně po všech indexech. Zamyslete sa nad kvalitou zvolené funkce.
 */
int get_hash(char *key) {
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++) {
    result += key[i];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializace tabulky — zavolá sa před prvním použitím tabulky.
 */
void ht_init(ht_table_t *table) {
    // Initialize each slot in the hashtable to NULL
    for (int i = 0; i < HT_SIZE; i++) {
        (*table)[i] = NULL;
    }
}

/*
 * Vyhledání prvku v tabulce.
 *
 * V případě úspěchu vrací ukazatel na nalezený prvek; v opačném případě vrací
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {
    // Calculate the hash index for the key
    int index = get_hash(key);
    // Traverse the LL at the calculated index
    ht_item_t *item = (*table)[index];
    while (item != NULL) {
         // If the current item's key matches the search key, then we found it
        if (strcmp(item->key, key) == 0) {
            return item;  
        }
        item = item->next;
    }
   // Return NULL, because we haven't found it
    return NULL;
}

/*
 * Vložení nového prvku do tabulky.
 *
 * Pokud prvek s daným klíčem už v tabulce existuje, nahraďte jeho hodnotu.
 *
 * Při implementaci využijte funkci ht_search. Pri vkládání prvku do seznamu
 * synonym zvolte nejefektivnější možnost a vložte prvek na začátek seznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value) {
    int index = get_hash(key);

    // Check if key already exists
    ht_item_t *existingItem = ht_search(table, key);
    if (existingItem != NULL) {
        // If it does, then update the value of the existing item and return
        existingItem->value = value;
        return;
    }

    // Create a new item, if the existing key wasn't found in the previous step
    ht_item_t *newItem = malloc(sizeof(ht_item_t));
    if (newItem == NULL) {
        // Handle memory allocation error
        return;
    }

    // Allocate memory for the key
    newItem->key = malloc(strlen(key) + 1); // +1 for the null terminator
    if (newItem->key == NULL) {
        // Handle memory allocation error, free item we created in previous step
        free(newItem);
        return;
    }
    // Copy the key into new item
    strcpy(newItem->key, key);

    // Set the value for the new item
    newItem->value = value;

    // Insert the item at the beginning of the LL
    newItem->next = (*table)[index];
    (*table)[index] = newItem;
}

/*
 * Získání hodnoty z tabulky.
 *
 * V případě úspěchu vrací funkce ukazatel na hodnotu prvku, v opačném
 * případě hodnotu NULL.
 *
 * Při implementaci využijte funkci ht_search.
 */
float *ht_get(ht_table_t *table, char *key) {
    // Search for the item with the specified key
    ht_item_t *item = ht_search(table, key);
    if (item == NULL) {
         // If item wasn't found -> return NULL
        return NULL;
    }
    // Return a pointer to the item value
    return &item->value;
}

/*
 * Smazání prvku z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje přiřazené k danému prvku.
 * Pokud prvek neexistuje, funkce nedělá nic.
 *
 * Při implementaci NEPOUŽÍVEJTE funkci ht_search.
 */
void ht_delete(ht_table_t *table, char *key) {
    // Calculate the hash index for the key
    int index = get_hash(key);
    // Traverse the LL to find and remove the item
    ht_item_t *current = (*table)[index];
    ht_item_t *previous = NULL;
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            // If the item is found, remove it from the LL and free its memory
            if (previous == NULL) {
                (*table)[index] = current->next;
            } else {
                // Link previous next to the current next, so we take out the item out of the LL
                previous->next = current->next;
            }
            free(current->key);
            free(current);
            return;
        }
        previous = current;
        current = current->next;
    }
}

/*
 * Smazání všech prvků z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje a uvede tabulku do stavu po 
 * inicializaci.
 */
void ht_delete_all(ht_table_t *table) {
    // Traverse the entire table to remove all items
    for (int i = 0; i < HT_SIZE; i++) {
        ht_item_t *item = (*table)[i];
        while (item != NULL) {
            // Free each item and its key
            ht_item_t *nextItem = item->next;
            free(item->key);
            free(item);
            item = nextItem;
        }
        // Set the table slot to NULL after clearing it
        (*table)[i] = NULL;
    }
}
