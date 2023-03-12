#ifndef SORT_H
#define SORT_H

#include "node.h"

void swapNodes(node **nodeA, node **nodeB);

int divide(node **queue, int first, int last);

void quickSortQueue(node **queue, int first, int last);

#endif