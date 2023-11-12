/*
 * Binární vyhledávací strom — iterativní varianta
 *
 * S využitím datových typů ze souboru btree.h, zásobníku ze souboru stack.h 
 * a připravených koster funkcí implementujte binární vyhledávací 
 * strom bez použití rekurze.
 */

#include "../btree.h"
#include "stack.h"
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
 * Funkci implementujte iterativně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
    // We loop until the current node is NULL
    while (tree != NULL) {
        // If the key to search is smaller than the current node key, then go left
        if (key < tree->key) {
            tree = tree->left;
        } 
        // If the key to search is bigger than the current node key, then go right
        else if (key > tree->key) {
            tree = tree->right;
        } 
        // If the key matches the current node key, then set the value to the current node value and return true
        else {
            *value = tree->value;
            return true;
        }
    }
    // Key wasn't found, so return false
    return false;
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
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
    // Initialize a pointer to traverse the tree starting from the root
    bst_node_t **current = tree;

    // Loop until an appropriate position is found
    while (*current != NULL) {
        // If the key we want to insert is smaller than current key, we move to the left child
        if (key < (*current)->key) {
            current = &(*current)->left;
        } 
        // If the key we want to insert is greater than current key, we move to the right child
        else if (key > (*current)->key) {
            current = &(*current)->right;
        } 
        // If the key already exists, then we update the value and return
        else {
            (*current)->value = value;
            return;
        }
    }

    // Allocate memory for a new node and assign key and value
    *current = (bst_node_t *)malloc(sizeof(bst_node_t));
    (*current)->key = key;
    (*current)->value = value;
    // Initialize node children to NULL
    (*current)->left = (*current)->right = NULL;
}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 * 
 * Klíč a hodnota uzlu target budou nahrazené klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 * 
 * Tato pomocná funkce bude využita při implementaci funkce bst_delete.
 *
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
    // Initialize pointers to find the rightmost node
    bst_node_t **rightmost = tree;
    bst_node_t *parent = NULL;

    // Traverse to the rightmost node
    while ((*rightmost)->right != NULL) {
        parent = *rightmost;
        rightmost = &(*rightmost)->right;
    }

    // Replace target node key and value with the rightmost node key and value
    target->key = (*rightmost)->key;
    target->value = (*rightmost)->value;

    // Remove the rightmost node and adjust the tree structure
    bst_node_t *temp = *rightmost;
    if (parent) {
        parent->right = (*rightmost)->left;
    } else {
        *tree = (*rightmost)->left;
    }
    // Free the memory of the removed rightmost node
    free(temp);
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
 * Funkci implementujte iterativně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key) {
    // Initialize a pointer to traverse the tree
    bst_node_t **current = tree;
    bst_node_t *nodeToDelete = NULL;

    // Loop to find the node to delete
    while (*current != NULL) {
        // If the key matches, prepare to delete this node
        if (key == (*current)->key) {
            nodeToDelete = *current;
            // If the node has only one child or no child, replace it with its child or NULL
            if (nodeToDelete->left == NULL || nodeToDelete->right == NULL) {
                *current = (nodeToDelete->left != NULL) ? nodeToDelete->left : nodeToDelete->right;
            } 
            // If the node has two children, use our bst_replace_by_rightmost funciton
            else {
                bst_replace_by_rightmost(nodeToDelete, &nodeToDelete->left);
                return;
            }
        } 
        // The key wasn't matching, so move left or right depending on the key
        else {
            current = (key < (*current)->key) ? &(*current)->left : &(*current)->right;
        }
    }

    // Free the memory of the node to be deleted
    if (nodeToDelete != NULL) {
        free(nodeToDelete);
    }
}

/*
 * Zrušení celého stromu.
 * 
 * Po zrušení se celý strom bude nacházet ve stejném stavu jako po 
 * inicializaci. Funkce korektně uvolní všechny alokované zdroje rušených 
 * uzlů.
 * 
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree) {
    // Initialize a stack to store nodes for disposal
    stack_bst_t stack;
    stack_bst_init(&stack);

    // Start from the root
    bst_node_t *current = *tree;

    // Loop until all nodes are processed
    while (current != NULL || !stack_bst_empty(&stack)) {
        // Traverse to the leftmost node
        if (current != NULL) {
            stack_bst_push(&stack, current);
            current = current->left;
        } 
        // Process the node and move to the right subtree
        else {
            // Move back "up" by popping from stack so we can get right subtree of the parent node
            current = stack_bst_pop(&stack);
            bst_node_t *right = current->right;
            // Free the current node, and replace it with its right child
            free(current);
            current = right;
        }
    }
    // Set the tree root to NULL after disposal
    *tree = NULL;
}

/*
 * Pomocná funkce pro iterativní preorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu.
 * Nad zpracovanými uzly zavolá bst_add_node_to_items a uloží je do zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit, bst_items_t *items) {
    // Traverse to the leftmost node and push nodes to the stack
    while (tree != NULL) {
        // We can add node to items, because we have preorder: Root - LC - RC
        bst_add_node_to_items(tree, items);
        stack_bst_push(to_visit, tree);
        tree = tree->left;
    }
}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_preorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items) {
    // Initialize a stack to manage the traversal order
    stack_bst_t stack;
    stack_bst_init(&stack);

    // Start with using bst_leftmost_preorder function
    bst_leftmost_preorder(tree, &stack, items);

    // Continue traversal for each node that we can pop from the stack
    while (!stack_bst_empty(&stack)) {
        tree = stack_bst_pop(&stack);
        // Traverse to the leftmost node of the right child of the popped node
        bst_leftmost_preorder(tree->right, &stack, items);
    }
}

/*
 * Pomocná funkce pro iterativní inorder.
 * 
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {
    // Traverse to the leftmost node and push nodes to the stack
    while (tree != NULL) {
        stack_bst_push(to_visit, tree);
        tree = tree->left;
    }
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_inorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items) {
    // Initialize a stack to manage the traversal order
    stack_bst_t stack;
    stack_bst_init(&stack);

    // Start with using bst_leftmost_inorder function
    bst_leftmost_inorder(tree, &stack);

    // Continue traversal for each node that we can pop from the stack
    while (!stack_bst_empty(&stack)) {
        tree = stack_bst_pop(&stack);
        // We should add node to items now, because we have inorder: LC - Root - RC
        bst_add_node_to_items(tree, items);
        // Traverse to the leftmost node of the right child of the popped node
        bst_leftmost_inorder(tree->right, &stack);
    }
}

/*
 * Pomocná funkce pro iterativní postorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů. Do zásobníku bool hodnot ukládá informaci, že uzel
 * byl navštíven poprvé.
 *
 * Funkci implementujte iterativně pomocí zásobníku uzlů a bool hodnot a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
                            stack_bool_t *first_visit) {
    // Traverse to the leftmost node and push nodes to the stack
    // Mark each node we visited for the first time
    while (tree != NULL) {
        stack_bst_push(to_visit, tree);
        stack_bool_push(first_visit, true);
        tree = tree->left;
    }
}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_postorder a
 * zásobníku uzlů a bool hodnot a bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items) {
    // Initialize stacks to manage nodes and their visit status
    stack_bst_t stack;
    stack_bst_init(&stack);
    stack_bool_t visited;
    stack_bool_init(&visited);

    // Start with using bst_leftmost_postorder function
    bst_leftmost_postorder(tree, &stack, &visited);

    // Continue traversal for each node that we can pop from the stack
    while (!stack_bst_empty(&stack)) {
        tree = stack_bst_top(&stack);
        bool first_time = stack_bool_pop(&visited);

        // If the node is visited for the first time, process its right subtree
        if (first_time) {
            stack_bool_push(&visited, false);
            bst_leftmost_postorder(tree->right, &stack, &visited);
        } 
        // If the node is visited for the second time, we can add it to the items, because we have postorder: LC - RC - Root
        else {
            stack_bst_pop(&stack);
            bst_add_node_to_items(tree, items);
        }
    }
}
