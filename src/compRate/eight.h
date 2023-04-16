#ifndef EIGHT_H
#define EIGHT_H

#include <stdio.h>
#include "../queue/node.h"

int eightAnalyzeInput( FILE* in, int* charcounter, int uniquecounter );

int eightOutputGenerator( FILE* in, int uniqueCounter, unsigned short** codes, FILE *out, char password, char *remainingChar, int remainingLen, unsigned char *crc );

#endif