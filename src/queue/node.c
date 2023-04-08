#include "node.h"
#include <stdbool.h>
#include <stdlib.h>

// makes a node
node *makeNode(unsigned short *character, int frequency, bool used, node *left, node *right){
    node *treeNode = malloc(sizeof *treeNode);

    treeNode->value = character;
    treeNode->freq = frequency;
    treeNode->used = used; 
    treeNode->left = left;
    treeNode->right = right;

    return treeNode; 
}