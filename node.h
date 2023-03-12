#ifndef NODE_H
#define NODE_H

#include <stdio.h>
#include <stdbool.h>

typedef struct node{
    char *value;
    int freq;
    bool used;
    struct node *left;
    struct node *right;
}node;

node *makeNode(char *character, int frequency, bool used, node *left, node *right);

#endif