#ifndef SIXTEEN_H
#define SIXTEEN_H

#include <stdio.h>

int sixteenAnalyzeInput( FILE* in, int* charcounter, int uniqueCounter );

void sixteenOutputGenerator(FILE* in, int uniqueCounter, unsigned short** codes, FILE *out);

#endif