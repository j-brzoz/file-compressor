#include "queue.h"
#include "node.h"
#include <stdlib.h>

// to check values
void printQueue(node **queue, int n){
    for(int i = 0; i < n; i++){
        if(queue[i]->value != NULL){
            printf("val: %d\n", *(queue[i]->value));
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
    long long int minFrequency = 1000000000000; // aprox 1TB
    int minIndex;
    for(int i = 0; i < queueSize; i++){
        if(queue[i] == NULL){
            queue[minIndex]->used = true;
            return queue[minIndex];
        }
        if(queue[i] != NULL && queue[i]->used == false && queue[i]->freq < minFrequency) {
            minFrequency = queue[i]->freq;
            minIndex = i;
        }
    }
    // for(int i = 0; i < queueSize; i++){
    //     if(queue[i]->used == false){
    //         queue[i]->used = true;
    //         return queue[i];
    //     }
    // }
    queue[minIndex]->used = true;
    return queue[minIndex];
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
void readCodes(node *root, int size, unsigned short **codes, unsigned short *tmp, int level){
    if(root->left != NULL){
        tmp[level] = '1';
        readCodes(root->left, size, codes, tmp, level + 1);
    }
    
    if (root->right != NULL){
        tmp[level] = '0';
        readCodes(root->right, size, codes, tmp, level + 1);
    }

    if (root->value != NULL) {
        for(int i = 0; i < size; i++){
            if(codes[i] == NULL){
                codes[i] = malloc((level+1) * sizeof *codes);
                codes[i][0] = (short)(*root->value);
                codes[i][1] = level;
                for(int j = 0; j < level; j++){
                    codes[i][j+2] = tmp[j];
                }
                break;
            }
        }
    }
}