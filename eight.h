#ifndef EIGHT_H
#define EIGHT_H

#include <stdio.h>

int eightAnalyzeInput(int inputBuforLength, char* inputBufor, FILE* in, int* charcounter, int uniquecounter);

void eightOutputGenerator(char c, FILE* in, char* bufor, int buforLength, char* character, char* characterBinary, int size, unsigned short** codes, FILE *out);

#endif