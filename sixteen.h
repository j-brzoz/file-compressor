#ifndef SIXTEEN_H
#define SIXTEEN_H

#include <stdio.h>

int sixteenAnalyzeInput(int inputBuforLength, char* inputBufor, FILE* in, int* charcounter, int uniquecounter, unsigned short input);

void sixteenOutputGenerator(int inputBuforLength, char* inputBufor, FILE* in, unsigned short input, char* bufor, int buforLength, char* character, char* characterBinary, int size, unsigned short** codes, FILE *out);

#endif