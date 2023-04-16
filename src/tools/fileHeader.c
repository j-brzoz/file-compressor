#include <stdio.h>
#include <stdlib.h>
#include "convert.h"

int header(FILE *out, int inputSize, int zeroCounter, unsigned char crc){
    // charcter that will be put in the output file
    char character[1];
    // binary representation of charcter   
    char *characterBinary = malloc( 8 * sizeof *characterBinary );
    if(characterBinary == NULL){
        fprintf(stderr, "There was a problem with allocating memory. Sorry!");
        return -1;
    }

    // to check if file has not been damaged
    characterBinary[0] = '1';
    characterBinary[1] = '1';
    characterBinary[4] = '1';

    // which compression rate was used
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

    // how many zeros we added
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

    free(characterBinary);
    return 0;
}