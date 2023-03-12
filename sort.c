#include "sort.h"

// swaps two nodes (for sorting)
void swapNodes(node **nodeA, node **nodeB){
    node *tmp = *nodeA;
    *nodeA = *nodeB;
    *nodeB = tmp;
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