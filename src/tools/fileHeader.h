#ifndef FILEHEADER_H
#define FILEHEADER_H

#include <stdio.h>

void header(FILE *out, int inputSize, int isOdd, int remainder, int zeroCounter, char crc);

#endif