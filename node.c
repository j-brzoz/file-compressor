#include "node.h"
#include <stdbool.h>
#include <stdlib.h>

// makes a node
node *makeNode(char *character, int frequency, bool used, node *left, node *right){
    node *treeNode = malloc(sizeof *treeNode);
    treeNode->value = treeNode->value;
    treeNode->left = treeNode->left;
    treeNode->right = treeNode->right;

    treeNode->value = character;
    treeNode->freq = frequency;
    treeNode->used = used; 
    treeNode->left = left;
    treeNode->right = right;
    
    return treeNode; 
}