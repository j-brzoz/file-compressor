#include <stdio.h>
#include <stdlib.h>
#include "./convert.h"

int dictionary(unsigned short **codes, FILE *out, int uniqueCounter, int inputSize, char *bufor, unsigned char *crc, char password){
    unsigned char byte[1];

    // dictionary length in bits
    unsigned int dictLength = 0;
    for(int i = 0; i < uniqueCounter; i++){
        //           charcter  codeLen code
        dictLength += inputSize + 8 + codes[i][1];
    }
    
    byte[0] = (dictLength >> 24) & 0xFF;
    fwrite(byte, 1, 1, out);

    byte[0] = (dictLength >> 16) & 0xFF;
    fwrite(byte, 1, 1, out);

    byte[0] = (dictLength >> 8) & 0xFF;
    fwrite(byte, 1, 1, out);

    byte[0] = dictLength & 0xFF;
    fwrite(byte, 1, 1, out);

    // unique counter
    byte[0] = (uniqueCounter >> 24) & 0xFF;
    fwrite(byte, 1, 1, out);

    byte[0] = (uniqueCounter >> 16) & 0xFF;
    fwrite(byte, 1, 1, out);

    byte[0] = (uniqueCounter >> 8) & 0xFF;
    fwrite(byte, 1, 1, out);

    byte[0] = uniqueCounter & 0xFF;
    fwrite(byte, 1, 1, out);

    int buforLength = 0;
    char *value;
    char *byteBin = malloc(8 * sizeof * byteBin);

    // write dictionary
    for(int i = 0; i < uniqueCounter; i++){
        
        value = DectoBin(codes[i][0], inputSize);
        for(int j = 0; j < inputSize; j++){
            bufor[j+buforLength] = value[j];
        }
        buforLength += inputSize;

        free(value);
        value = DectoBin(codes[i][1], 8);
        
        for(int j = 0; j < 8; j++){
            bufor[j+buforLength] = value[j];
        }
        buforLength += 8;
        
        free(value);
        
        for(int j = 0; j < codes[i][1]; j++){
            bufor[j+buforLength] = codes[i][j+2];
        }
        buforLength += codes[i][1];

        while( buforLength >= 8 ) {
            for(int j = 0; j < 8; j++) {
                byteBin[j] = bufor[j];
            }  
            byte[0] = binToDec( byteBin );
            crc[0] = crc[0] ^ byte[0];
            byte[0] = byte[0] ^ password;
            fwrite( byte, 1, 1, out );
            for( int j = 0; j < buforLength - 8; j++ ) {
                bufor[j] = bufor[j + 8];
            }
            buforLength -= 8;
        }
    }

    free(byteBin);

    return buforLength;
}