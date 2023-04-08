#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../tools/convert.h"
#include "../queue/node.h"
#include "../tools/fileHeader.h"

int sixteenAnalyzeInput( FILE* in, int* charcounter, int uniqueCounter ) {
    // for character conversion from binary to decimal 
    unsigned short input;
    // bufor for reading from input file
    unsigned char *inputBufor = malloc( 1200 * sizeof *inputBufor );
    // size of input bufor
    int inputBuforLength;
    // to check if number of bytes is odd 
    int isOdd;

    // read 1200 bytes
    while( inputBuforLength = fread( inputBufor, 1, 1200, in ) ) {
        
        // check if number of bytes is odd 
        isOdd = inputBuforLength % 2;

        // if all 1200 bytes were read
        if( inputBuforLength == 1200 ) {
            for( int i = 0; i < 1200; i += 2 ) {
                
                // make sure the input is correct
                if( inputBufor[i] >= 0 && inputBufor[i] <= 255 && 
                   inputBufor[i+1] >= 0 && inputBufor[i+1] <= 255 ) {

                    // convert two bytes to one decimal value
                    input = pow( 2, 8 ) * inputBufor[i] + inputBufor[i+1];

                    charcounter[input]++;
                    if (charcounter[input] == 1)
                        uniqueCounter++;
                }
            }
        }
        // if not all 1200 bytes were read
        else {
            for( int i = 0; i < inputBuforLength - isOdd - 1; i += 2 ) {
                // make sure the input is correct
                if( inputBufor[i] >= 0 && inputBufor[i] <= 255 && 
                    inputBufor[i+1] >= 0 && inputBufor[i+1] <= 255 ) {
                    
                    // convert two bytes to one decimal value
                    input = pow( 2, 8 ) * inputBufor[i] + inputBufor[i+1];
                    
                    charcounter[input]++;
                    if ( charcounter[input] == 1 )
                        uniqueCounter++;
                }
            }
        }

        // there was odd number of bytes 
        if( isOdd ) {
            // make sure the input is correct
            if( inputBufor[inputBuforLength-1] >= 0 && inputBufor[inputBuforLength-1] <= 255 ) {

                // convert the byte to decimal value
                input = inputBufor[inputBuforLength-1] * 256;

                charcounter[input]++;
                if ( charcounter[input] == 1 )
                    uniqueCounter++;
            }
        }
    }
    free(inputBufor);
    return uniqueCounter;
}

void sixteenOutputGenerator( FILE* in, int uniqueCounter, unsigned short** codes, FILE *out, char password, char *remainingChar, int remainingLen, unsigned char *crc ) {
    // for character conversion from binary to decimal 
    unsigned short input;
    // bufor for reading from input file
    unsigned char *inputBufor = malloc( 1200 * sizeof *inputBufor );
    // size of input bufor
    int inputBuforLength;
    // to check if number of bytes is odd 
    int isOdd;
    // charcter that will be put in the output file
    unsigned char* character = malloc( sizeof *character );
    // binary representation of charcter   
    char *characterBinary = malloc( 8 * sizeof *characterBinary );
    characterBinary[0] = 0;
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

    // get two bytes
    while( inputBuforLength = fread( inputBufor, 1, 2, in ) ) {
        
        // check if number of bytes is odd 
        isOdd = inputBuforLength % 2;
        
        // if 2 bytes were read
        if( inputBuforLength == 2 ) {
            
            // convert two bytes to one decimal value
            input = pow( 2, 8 ) * ( unsigned short )inputBufor[0] + ( unsigned short )inputBufor[1];

            // find value in codes
            for( int k = 0; k < uniqueCounter; k++ ) {
                if( input == codes[k][0] ) {
                    
                    // write code to bufor
                    for( int j = 0; j < codes[k][1]; j++ ) {
                        bufor[buforLength+j] = codes[k][j+2];
                    }
                    buforLength += codes[k][1];
                }
            }
        }

        // if enough bits in bufor
        while( buforLength >= 8 ) {
        
            // get code from bufor
            for( int i = 0; i < 8; i++ ) {
                characterBinary[i] = bufor[i];
            }
            
            // convert code to char
            character[0] = binToDec( characterBinary ) ^ password;

            crc[0] = crc[0] ^ binToDec( characterBinary );
            
            // write character
            fwrite( character, 1, 1, out );
            
            // move codes in bufor
            for( int i = 0; i < buforLength - 8; i++ ) {
                bufor[i] = bufor[i + 8];
            }
            buforLength -= 8;
        }
        
    }

    // there was odd number of bytes
    if( isOdd ){
        
        // convert the byte to decimal value
        input = ( unsigned short )inputBufor[0] * 256;

        // find character in codes
        for( int k = 0; k < uniqueCounter; k++ ) {
            if( input == codes[k][0] ) {

                // write code to bufor
                for(int j = 0; j < codes[k][1]; j++) {
                    bufor[buforLength+j] = (char)codes[k][j+2];
                }
                buforLength += codes[k][1];
            }
        }
    }

    // if enough bits in bufor
    while(buforLength >= 8) {
        
            // get code from bufor
            for(int i = 0; i < 8; i++) {
                characterBinary[i] = bufor[i];
            }
            
            // convert code to char
            character[0] = binToDec(characterBinary) ^ password;

            crc[0] = crc[0] ^ binToDec( characterBinary );

            // write character
            fwrite(character, 1, 1, out);
            
            // move codes in bufor
            for(int i = 0; i < buforLength - 8; i++){
                bufor[i] = bufor[i + 8];
            }
            buforLength -= 8;
    }

    // if any bites left in bufor
    if(buforLength != 0){
        
        // get code from bufor
        for(int i = 0; i < buforLength; i++) {
                characterBinary[i] = bufor[i];
        }
        
        // level up to 8 bits
        for(int i = buforLength; i < 8; i++) {
                characterBinary[i] = '0';
                zeroCounter++;
        }
        
        // convert code to char
        character[0] = binToDec(characterBinary) ^ password;
        
        crc[0] = crc[0] ^ binToDec( characterBinary );
    
        // write character
        fwrite(character, 1, 1, out);
    }

    // write header
    header(out, 16, zeroCounter, crc[0]);

    free( inputBufor );
    free( character );
    free( characterBinary );
    free( bufor );
}