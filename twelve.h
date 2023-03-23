#ifndef TWELVE_H
#define TWELVE_H

#include <stdio.h>

int twelveAnalyzeInput(int inputBuforLength, char* inputBufor, FILE* in, int* charcounter, int uniquecounter, unsigned short input);

void twelveOutputGenerator(int inputBuforLength, char* inputBufor, FILE* in, unsigned short input, char* bufor, int buforLength, char* character, char* characterBinary, int size, unsigned short** codes, FILE *out);

int twelveDictionary( node *pointer, node *border, node *root, node *last, FILE *out, int buforLength, char *bufor);

#endif