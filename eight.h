#ifndef EIGHT_H
#define EIGHT_H

#include <stdio.h>

int eightAnalyzeInput( FILE* in, int* charcounter, int uniquecounter );

void eightOutputGenerator( FILE* in, int uniqueCounter, unsigned short** codes, FILE *out );

#endif