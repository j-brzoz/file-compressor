#include <stdio.h>
#include <stdlib.h>
#include "convert.h"

int eightAnalyzeInput( FILE* in, int* charcounter, int uniqueCounter ) {
    
    // bufor for reading from input file
    char *inputBufor = malloc( 1200 * sizeof *inputBufor );
    // size of input bufor
    int inputBuforLength;

    // read 1200 bytes
    while ( inputBuforLength = fread( inputBufor, 1, 1200, in ) ) {
        for( int i = 0; i < inputBuforLength; i++ ) {
            // make sure the input is correct
            if( ( unsigned short )inputBufor[i] >= 0 && ( unsigned short )inputBufor[i] <= 255 ) {
                charcounter[( unsigned short )inputBufor[i]]++;
                if ( charcounter[( unsigned short )inputBufor[i]] == 1 )
                    uniqueCounter++;
            }
        }
    }
    free( inputBufor );
    return uniqueCounter;       
}

void eightOutputGenerator( FILE* in, int uniqueCounter, unsigned short** codes, FILE *out ) {
    
    // char read from the file
    char c;
    // charcter that will be put in the output file
    char* character = malloc( sizeof *character );
    // binary representation of charcter   
    char *characterBinary = malloc( 8 * sizeof *characterBinary );
    // bufor with codes from characters found in the input
    char *bufor = malloc( 16384 * sizeof *bufor );
    // length of the bufor
    int buforLength = 0;

    // get chararcter
    while ( ( c = fgetc( in ) ) != EOF ) {
            
        // find character in codes
        for( int i = 0; i < uniqueCounter; i++ ) {
            if( c == codes[i][0] ) {
                
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
            character[0] = binToDec( characterBinary );
            
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
        }
        
        // convert code to char
        character[0] = binToDec( characterBinary );
        
        // write character
        fwrite( character, 1, 1, out );
    }

    free( bufor );
    free( character );
    free( characterBinary );
}