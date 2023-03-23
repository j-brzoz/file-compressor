#ifndef EIGHT_H
#define EIGHT_H

#include <stdio.h>

int eightAnalyzeInput( FILE* in, int* charcounter, int uniquecounter );

void eightOutputGenerator( FILE* in, int uniqueCounter, unsigned short** codes, FILE *out );

int eightDictionary( node *pointer, node *border, node *root, node *last, FILE *out, int buforLength, char *bufor);

#endif