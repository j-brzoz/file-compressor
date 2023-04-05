#include <stdio.h>
#include <stdlib.h>
#include "convert.h"

void header(FILE *out, int inputSize, int isOdd, int remainder, int zeroCounter, unsigned char crc){
    // charcter that will be put in the output file
    char* character = malloc( sizeof *character );
    // binary representation of charcter   
    char *characterBinary = malloc( 8 * sizeof *characterBinary );

    if( remainder == 1 ) {
        characterBinary[0] = '0';
        characterBinary[1] = '1';
    }
    else if( remainder == 2 ) {
        characterBinary[0] = '1';
        characterBinary[1] = '0';
    }
    else {
        characterBinary[0] = '0';
        characterBinary[1] = '0';
    }

    if( inputSize == 8 ) {
        characterBinary[2] = '0';
        characterBinary[3] = '1';
    }
    else if( inputSize == 12 ) {
        characterBinary[2] = '1';
        characterBinary[3] = '0';
    }
    else if( inputSize == 16 ) {
        characterBinary[2] = '1';
        characterBinary[3] = '1';
    }

    if(isOdd == 0){
        characterBinary[4] = '0';
    }
    else {
        characterBinary[4] = '1';
    }

    switch (zeroCounter) {
        case 0:
            characterBinary[5] = '0';
            characterBinary[6] = '0';
            characterBinary[7] = '0';
            break;
        case 1:
            characterBinary[5] = '0';
            characterBinary[6] = '0';
            characterBinary[7] = '1';
            break;
        case 2:
            characterBinary[5] = '0';
            characterBinary[6] = '1';
            characterBinary[7] = '0';
            break;
        case 3:
            characterBinary[5] = '0';
            characterBinary[6] = '1';
            characterBinary[7] = '1';
            break;
        case 4:
            characterBinary[5] = '1';
            characterBinary[6] = '0';
            characterBinary[7] = '0';
            break;
        case 5:
            characterBinary[5] = '1';
            characterBinary[6] = '0';
            characterBinary[7] = '1';
            break;
        case 6:
            characterBinary[5] = '1';
            characterBinary[6] = '1';
            characterBinary[7] = '0';
            break;
        case 7:
            characterBinary[5] = '1';
            characterBinary[6] = '1';
            characterBinary[7] = '1';
            break;
        default:
            abort ();
    }
    
    // rewind the file
    rewind(out);

    // write info as first byte
    character[0] = binToDec( characterBinary );
    fwrite( character, 1, 1, out );

    // write crc as second byte
    character[0] = crc;
    fwrite( character, 1, 1, out );

    free(character);
    free(characterBinary);
}