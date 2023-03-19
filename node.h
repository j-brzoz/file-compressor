#ifndef NODE_H
#define NODE_H

#include <stdio.h>
#include <stdbool.h>

typedef struct node{
    unsigned short *value;
    int freq;
    bool used;
    struct node *left;
    struct node *right;
}node;

node *makeNode(unsigned short *character, int frequency, bool used, node *left, node *right);

#endif