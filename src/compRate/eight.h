#ifndef EIGHT_H
#define EIGHT_H

#include <stdio.h>
#include "../queue/node.h"

int eightAnalyzeInput( FILE* in, int* charcounter, int uniquecounter );

void eightOutputGenerator( FILE* in, int uniqueCounter, unsigned short** codes, FILE *out, char password, char *remainingChar, int remainingLen );

int eightDictionary( node *pointer, node *border, node *root, node *last, FILE *out, int buforLength, char *bufor);

node *readDicEight(FILE *in, char *bufor, int buforLength, unsigned short *charzero, unsigned short *character, int charlen, int *uniqueCounter);

#endif