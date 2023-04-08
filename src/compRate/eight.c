#include <stdio.h>
#include <stdlib.h>
#include "../tools/convert.h"
#include "../queue/node.h"
#include "../tools/fileHeader.h"

int eightAnalyzeInput( FILE* in, int* charcounter, int uniqueCounter ) {
    
    // bufor for reading from input file
    unsigned char *inputBufor = malloc( 1200 * sizeof *inputBufor );
    // size of input bufor
    int inputBuforLength;

    // read 1200 bytes
    while ( inputBuforLength = fread( inputBufor, sizeof*inputBufor, 1200, in ) ) {
        for( int i = 0; i < inputBuforLength; i++ ) {
            // make sure the input is correct
            if( inputBufor[i] >= 0 && inputBufor[i] <= 255 ) {
                charcounter[inputBufor[i]]++;
                if ( charcounter[inputBufor[i]] == 1 )
                    uniqueCounter++;
            }
        }
    }
    free( inputBufor );
    return uniqueCounter;       
}

void eightOutputGenerator( FILE* in, int uniqueCounter, unsigned short** codes, FILE *out, char password, char *remainingChar, int remainingLen, unsigned char *crc ) {
    
    // char read from the file
    unsigned char *c = malloc(sizeof *c);
    // charcter that will be put in the output file
    unsigned char* character = malloc( sizeof *character );
    // binary representation of charcter   
    char *characterBinary = malloc( 8 * sizeof *characterBinary );
    // bufor with codes from characters found in the input
    unsigned char *bufor = malloc( 16384 * sizeof *bufor );
    // length of the bufor
    int buforLength = 0;
    // count how many zeros we add artficially
    int zeroCounter = 0;

    // add remaining chars from dictionary
    for(int i = 0; i < remainingLen; i++){
        bufor[i] = remainingChar[i];
    }
    buforLength += remainingLen;

    // get chararcter
    while ( fread( c, 1, 1, in ) ) {
            
        // find character in codes
        for( int i = 0; i < uniqueCounter; i++ ) {
            if( c[0] == codes[i][0] ) {
                
                // write code to bufor
                for( int j = 0; j < codes[i][1]; j++ ) {
                    bufor[buforLength+j] = codes[i][j+2];
                }
                buforLength += codes[i][1];
            }
        }
        
        // if enough bits in bufor
        while( buforLength >= 8 ) {
        
            // get code from bufor
            for(int i = 0; i < 8; i++) {
                characterBinary[i] = bufor[i];
            }
            
            // convert code to char
            character[0] = binToDec( characterBinary ) ^ password;

            crc[0] = crc[0] ^ binToDec( characterBinary );
            
            // write character
            fwrite( character, sizeof(char), 1, out );
            
            // move codes in bufor
            for( int i = 0; i < buforLength - 8; i++ ) {
                bufor[i] = bufor[i + 8];
            }
            buforLength -= 8;
        }
    }   

    // if any bites left in bufor
    if( buforLength != 0 ) {
        
        // get code from bufor
        for( int i = 0; i < buforLength; i++ ) {
                characterBinary[i] = bufor[i];
        }
        
        // level up to 8 bits
        for( int i = buforLength; i < 8; i++ ) {
                characterBinary[i] = '0';
                zeroCounter++;
        }
        
        // convert code to char
        character[0] = binToDec( characterBinary ) ^ password;

        crc[0] = crc[0] ^ binToDec( characterBinary );
        
        // write character
        fwrite( character, 1, 1, out );
    }

    // write header
    header(out, 8, zeroCounter, crc[0]);

    free(c);
    free( bufor );
    free( character );
    free( characterBinary );
}