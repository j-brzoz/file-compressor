#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct node{
    char *value;
    int freq;
    bool used;
    struct node *left;
    struct node *right;
}node;

// swaps two nodes (for sorting)
void swapNodes(node **nodeA, node **nodeB){
    node *tmp = *nodeA;
    *nodeA = *nodeB;
    *nodeB = tmp;
}

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

// used for quick sort
int divide(node **queue, int first, int last){
    int pivot = queue[last]->freq;
    int i = first - 1;
    for(int j = first; j <= last - 1; j++){
        if(queue[j]->freq < pivot){
            i++;
            swapNodes(&queue[j], &queue[i]);
        }
    }
    swapNodes(&queue[i + 1], &queue[last]);

    return i + 1;
}

// sorts queue elements in ascending order
void quickSortQueue(node **queue, int first, int last){
    if(first < last){
        int pivot = divide(queue, first, last);
        quickSortQueue(queue, first, pivot - 1);
        quickSortQueue(queue, pivot + 1, last);
    }
}

// to check values
void printQueue(node **queue, int n){
    for(int i = 0; i < n; i++){
        if(queue[i]->value != NULL){
            printf("val: %c\n", *(queue[i]->value));
        }
        printf("left: %p, right: %p\n", (queue[i]->left), (queue[i]->right));
        printf("freq: %d\n\n", queue[i]->freq);
    }
    printf("===================================\n");
}

// checks if queue is not full (2 * number of chars - 1)
bool notFull(node **queue, int size){
    int check = 0;
    for(int i = 0; i < size; i++)
        if(queue[i] != NULL)
            check++;
    return (size - check > 0);
}

// finds nodes with minimum frequency, that don't have a parent already
node *findMinNotUsedNode(node **queue, int queueSize){
    for(int i = 0; i < queueSize; i++){
        if(queue[i]->used == false){
            queue[i]->used = true;
            return queue[i];
        }
    }
}

// makes new node, that points to 2 other nodes
void addNewNodeToQueue(node **queue, int queueSize){
    node *firstNode = findMinNotUsedNode(queue, queueSize);
    node *secondNode = findMinNotUsedNode(queue, queueSize);
    for(int i = 0; i < queueSize; i++){
        if(queue[i] == NULL){
            queue[i] = makeNode(NULL, firstNode->freq + secondNode->freq, false, firstNode, secondNode);
            break;
        }
    }
}

// put values in codes array
void readCodes(node *root, int size, int **codes, int *tmp, int level){
    if(root->left != NULL){
        tmp[level] = 1;
        readCodes(root->left, size, codes, tmp, level + 1);
    }
    
    if (root->right != NULL){
        tmp[level] = 0;
        readCodes(root->right, size, codes, tmp, level + 1);
    }

    if (root->value != NULL) {
        for(int i = 0; i < size; i++){
            if(codes[i] == NULL){
                codes[i] = malloc((level+1) * sizeof *codes);
                codes[i][0] = (*root->value);
                codes[i][1] = level;
                for(int j = 0; j < level; j++){
                    codes[i][j+2] = tmp[j];
                }
                break;
            }
        }
    }
}

int main(){

    char arr[] = { 'a', 'b', 'c', 'd', 'e'};
    int freq[] = { 3, 2, 1, 4, 1};
 
    int size = sizeof(arr) / sizeof(arr[0]);
    int queueSize = 2 * size - 1;

    // output codes array 
    // first row char
    // second row code length
    // next code
    int **codes;
    codes = malloc(size * sizeof *codes);
    for(int i = 0; i < size; i++)
        codes[i] = NULL;
        
    int tmp[size - 1];
    
    // make queue
    node **queue = malloc(queueSize * sizeof *queue);
    for(int i = 0; i < queueSize; i++)
        queue[i] = NULL;

    // put inputs in queue
    for(int i = 0; i < size; i++){
        node *tmpNode = makeNode(arr+i, freq[i], false, NULL, NULL);
        if(tmpNode == NULL){
            return 1;
        }
        queue[i] = tmpNode;
    }
    
    // sort inputs
    quickSortQueue(queue, 0, size - 1);
    
    // make a binary tree
    int temp = 0;
    while(notFull(queue, queueSize)){
        addNewNodeToQueue(queue, queueSize);
        quickSortQueue(queue, 2*(temp+1), size + temp);
        temp++;
    }
    
    // read codes from tree
    readCodes(queue[queueSize-1], size, codes, tmp, 0);

    // print codes
    for(int i = 0; i < size; i++){
        printf("c: %c   ", codes[i][0]);
        for(int j = 0; j < codes[i][1]; j++){
            printf("%d", codes[i][j+2]);
        }
        printf("\n");
    }

    // free memory
    for(int i = 0; i < queueSize; i++){
        free(queue[i]);
    }
    free(queue);

    for(int i = 0; i < size; i++){
        free(codes[i]);
    }
    free(codes);

    return 0;
}