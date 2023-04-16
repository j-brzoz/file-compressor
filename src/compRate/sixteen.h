#ifndef SIXTEEN_H
#define SIXTEEN_H

#include <stdio.h>

int sixteenAnalyzeInput( FILE* in, int* charcounter, int uniqueCounter );

int sixteenOutputGenerator( FILE* in, int uniqueCounter, unsigned short** codes, FILE *out, char password, char *remainingChar, int remainingLen, unsigned char *crc );

#endif