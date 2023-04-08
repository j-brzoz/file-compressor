#ifndef TWELVE_H
#define TWELVE_H

#include <stdio.h>

int twelveAnalyzeInput( FILE* in, int* charcounter, int uniqueCounter );

void twelveOutputGenerator( FILE* in, int uniqueCounter, unsigned short** codes, FILE *out, char password, char *remainingChar, int remainingLen, unsigned char *crc );

#endif