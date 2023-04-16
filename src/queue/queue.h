#ifndef QUEUE_H
#define QUEUE_H

#include "node.h"
#include <stdbool.h>

void printQueue(node **queue, int n);

bool notFull(node **queue, int size);

node *findMinNotUsedNode(node **queue, int queueSize);

int addNewNodeToQueue(node **queue, int queueSize);

void readCodes(node *root, int size, unsigned short **codes, unsigned short *tmp, int level);

#endif