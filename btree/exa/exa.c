/*
 * Použití binárních vyhledávacích stromů.
 *
 * S využitím Vámi implementovaného binárního vyhledávacího stromu (soubory ../iter/btree.c a ../rec/btree.c)
 * implementujte funkci letter_count. Výstupní strom může být značně degradovaný (až na úroveň lineárního seznamu) 
 * a tedy implementujte i druhou funkci (bst_balance), která strom, na požadavek uživatele, vybalancuje.
 * Funkce jsou na sobě nezávislé a tedy automaticky NEVOLEJTE bst_balance v letter_count.
 * 
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>


/**
 * Vypočítání frekvence výskytů znaků ve vstupním řetězci.
 * 
 * Funkce inicilializuje strom a následně zjistí počet výskytů znaků a-z (case insensitive), znaku 
 * mezery ' ', a ostatních znaků (ve stromu reprezentováno znakem podtržítka '_'). Výstup je v 
 * uložen ve stromu.
 * 
 * Například pro vstupní řetězec: "abBccc_ 123 *" bude strom po běhu funkce obsahovat:
 * 
 * key | value
 * 'a'     1
 * 'b'     2
 * 'c'     3
 * ' '     2
 * '_'     5
 * 
 * Pro implementaci si můžete v tomto souboru nadefinovat vlastní pomocné funkce.
*/
void letter_count(bst_node_t **tree, char *input) {
    // Initialize the BST to an empty tree
    bst_init(tree);

    // Loop through each character in the input string
    for (int i = 0; input[i] != '\0'; i++) {
        char key;

        // Determine the key based on the character

        // Convert uppercase letters to lowercase
        if (input[i] >= 'A' && input[i] <= 'Z') {
            key = input[i] + 'a' - 'A'; // Convert to lowercase
        } 
        // Directly use lowercase letters
        else if (input[i] >= 'a' && input[i] <= 'z') {
            key = input[i];
        } 
        // Assign space character directly
        else if (input[i] == ' ') {
            key = ' ';
        } 
        // Group all other characters under '_'
        else {
            key = '_';
        }

        // Search and update or insert the key

        int value;
        // Search the BST for the character key
        if (bst_search(*tree, key, &value)) {
            // If found, increase the count by 1
            bst_insert(tree, key, value + 1);
        } 
        // If not found, insert the new character with count 1
        else {
            bst_insert(tree, key, 1);
        }
    }
}

// Helper function to collect nodes in inorder traversal
void collect_nodes_inorder(bst_node_t *node, bst_node_t ***nodes, int *index, int *capacity) {
    // Base case for recursion stop
    if (node == NULL) return;

    // Double the array size if capacity is reached
    if (*index >= *capacity) {
        *capacity *= 2;
        *nodes = realloc(*nodes, *capacity * sizeof(bst_node_t*));
    }

    // We use inorder, so LC - Root - RC
    // Recursively collect nodes from the left subtree
    collect_nodes_inorder(node->left, nodes, index, capacity);
    // Add the current node to the array 
    (*nodes)[(*index)++] = node;
    // Recursively collect nodes from the right subtree
    collect_nodes_inorder(node->right, nodes, index, capacity);
}

// Helper function to build a balanced BST from sorted nodes
bst_node_t* build_balanced_tree(bst_node_t **nodes, int start, int end) {
    // Base case to end recursion. return NULL if start index exceeds end index
    if (start > end) return NULL;

    // Find the middle index
    int middle = (start + end) / 2;
    // Make the middle node the root
    bst_node_t *root = nodes[middle];

    // Recursively build the left subtree using left half of nodes
    root->left = build_balanced_tree(nodes, start, middle - 1);
    // Recursively build the right subtree using right half of nodes
    root->right = build_balanced_tree(nodes, middle + 1, end);

    // Return the new root of the balanced subtree
    return root;
}

/**
 * Vyvážení stromu.
 * 
 * Vyvážený binární vyhledávací strom je takový binární strom, kde hloubka podstromů libovolného uzlu se od sebe liší maximálně o jedna.
 * 
 * Předpokládejte, že strom je alespoň inicializován. K získání uzlů stromu využijte vhodnou verzi vámi naimplmentovaného průchodu stromem.
 * Následně můžete například vytvořit nový strom, kde pořadím vkládaných prvků zajistíte vyváženost.
 *  
 * Pro implementaci si můžete v tomto souboru nadefinovat vlastní pomocné funkce. Není nutné, aby funkce fungovala *in situ* (in-place).
*/
void bst_balance(bst_node_t **tree) {
    // Return immediately if the tree is empty
    if (*tree == NULL) return;

    // Initial capacity for the array to store nodes
    int capacity = 10; 
    // Index to track the number of nodes collected
    int index = 0;
    // Allocate memory for storing pointers to the tree nodes
    bst_node_t **nodes = malloc(capacity * sizeof(bst_node_t*));

    // Collect nodes of the tree using inorder traversal
    collect_nodes_inorder(*tree, &nodes, &index, &capacity);

    // Rebuild the tree in a balanced way using the collected nodes
    *tree = build_balanced_tree(nodes, 0, index - 1);

    // Free the memory used for storing node pointers
    free(nodes);
}
