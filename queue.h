#ifndef QUEUE_H
#define QUEUE_H

#include "node.h"
#include <stdbool.h>

void printQueue(node **queue, int n);

bool notFull(node **queue, int size);

node *findMinNotUsedNode(node **queue, int queueSize);

void addNewNodeToQueue(node **queue, int queueSize);

void readCodes(node *root, int size, int **codes, int *tmp, int level);

#endif