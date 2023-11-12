/*
 * Binární vyhledávací strom — rekurzivní varianta
 *
 * S využitím datových typů ze souboru btree.h a připravených koster funkcí
 * implementujte binární vyhledávací strom pomocí rekurze.
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializace stromu.
 *
 * Uživatel musí zajistit, že inicializace se nebude opakovaně volat nad
 * inicializovaným stromem. V opačném případě může dojít k úniku paměti (memory
 * leak). Protože neinicializovaný ukazatel má nedefinovanou hodnotu, není
 * možné toto detekovat ve funkci. 
 */
void bst_init(bst_node_t **tree) {
    // Set the initial pointer to NULL -> empty tree
    *tree = NULL;
}

/*
 * Vyhledání uzlu v stromu.
 *
 * V případě úspěchu vrátí funkce hodnotu true a do proměnné value zapíše
 * hodnotu daného uzlu. V opačném případě funkce vrátí hodnotu false a proměnná
 * value zůstává nezměněná.
 * 
 * Funkci implementujte rekurzivně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
    if (tree == NULL) {
        // If the current tree node is NULL, hten we can't find the key. Return false
        return false;
    } else if (key == tree->key) {
        // If the current tree node key matches the search key, then store the value and return true
        *value = tree->value;
        return true;
    } else if (key < tree->key) {
        // If the search key is smaller, go search the left subtree
        return bst_search(tree->left, key, value);
    } else {
        // Else, the search key is larger, and we go search the right subtree
        return bst_search(tree->right, key, value);
    }
}

/*
 * Vložení uzlu do stromu.
 *
 * Pokud uzel se zadaným klíče už ve stromu existuje, nahraďte jeho hodnotu.
 * Jinak vložte nový listový uzel.
 *
 * Výsledný strom musí splňovat podmínku vyhledávacího stromu — levý podstrom
 * uzlu obsahuje jenom menší klíče, pravý větší. 
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
    if (*tree == NULL) {
        // If the current tree node is NULL, then we create a new node and insert it here
        *tree = malloc(sizeof(bst_node_t));
        (*tree)->key = key;
        (*tree)->value = value;
        (*tree)->left = NULL;
        (*tree)->right = NULL;
    } else if (key == (*tree)->key) {
        // If the node with the key exists, update its value
        (*tree)->value = value;
    } else if (key < (*tree)->key) {
        // If key we want to insert is smaller then current key, insert into the left subtree
        bst_insert(&((*tree)->left), key, value);
    } else {
        // Else, the key we want to insert is larger, so we insert it into the right subtree
        bst_insert(&((*tree)->right), key, value);
    }
}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 * 
 * Klíč a hodnota uzlu target budou nahrazeny klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 * 
 * Tato pomocná funkce bude využitá při implementaci funkce bst_delete.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
    if ((*tree)->right != NULL) {
        // Recursively find the rightmost node, recursion will stop when subtree on the right is NULL
        bst_replace_by_rightmost(target, &((*tree)->right));
    } else {
        // Replace the target node key and value with kay and value of the rightmost node
        target->key = (*tree)->key;
        target->value = (*tree)->value;
        // Remove the rightmost node and free its memory
        bst_node_t *temp = *tree;
        // Assign Left child of the rightmost node as new rightmost
        *tree = (*tree)->left;
        free(temp);
    }
}

/*
 * Odstranění uzlu ze stromu.
 *
 * Pokud uzel se zadaným klíčem neexistuje, funkce nic nedělá.
 * Pokud má odstraněný uzel jeden podstrom, zdědí ho rodič odstraněného uzlu.
 * Pokud má odstraněný uzel oba podstromy, je nahrazený nejpravějším uzlem
 * levého podstromu. Nejpravější uzel nemusí být listem.
 * 
 * Funkce korektně uvolní všechny alokované zdroje odstraněného uzlu.
 * 
 * Funkci implementujte rekurzivně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key) {
    if (*tree == NULL) {
        // If the current node is NULL, the key is not found, so we do nothing
        return;
    }
    if (key < (*tree)->key) {
        // If the key is smaller, call recursion on left subtree
        bst_delete(&((*tree)->left), key);
    } else if (key > (*tree)->key) {
        // If the key is larger, call recursion on right subtree
        bst_delete(&((*tree)->right), key);
    } else {
        // Node with the key is found
        if ((*tree)->left == NULL || (*tree)->right == NULL) {
            // If the node has only one child, replace it with its child or NULL
            bst_node_t *temp = ((*tree)->left != NULL) ? (*tree)->left : (*tree)->right;
            free(*tree);
            *tree = temp;
        } else {
            // If the node has two children, replace it with the rightmost node in the left subtree (as requested in comments)
            bst_replace_by_rightmost(*tree, &((*tree)->left));
        }
    }
}

/*
 * Zrušení celého stromu.
 * 
 * Po zrušení se celý strom bude nacházet ve stejném stavu jako po 
 * inicializaci. Funkce korektně uvolní všechny alokované zdroje rušených 
 * uzlů.
 * 
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree) {
    if (*tree != NULL) {
        // Recursively call dispose of the left and right subtrees
        bst_dispose(&((*tree)->left));
        bst_dispose(&((*tree)->right));
        // Free the current node and set its pointer to NULL
        free(*tree);
        *tree = NULL;
    }
}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items) {
    if (tree != NULL) {
        // We have preorder, so Root -> LC -> RC
        // Add the current node to items
        bst_add_node_to_items(tree, items);
        // Recursively perform preorder traversal on the left and right subtrees
        bst_preorder(tree->left, items);
        bst_preorder(tree->right, items);
    }
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items) {
    if (tree != NULL) {
        // We have inorder, so LC -> Root -> RC
        // Recursively perform inorder traversal on the left subtree
        bst_inorder(tree->left, items);
        // Add the current node to items
        bst_add_node_to_items(tree, items);
        // Recursively perform inorder traversal on the right subtree
        bst_inorder(tree->right, items);
    }
}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items) {
    if (tree != NULL) {
        // We have postorder, so LC -> RC -> Root
        // Recursively perform postorder traversal on the left and right subtrees
        bst_postorder(tree->left, items);
        bst_postorder(tree->right, items);
        // Add the current node to items
        bst_add_node_to_items(tree, items);
    }
}
