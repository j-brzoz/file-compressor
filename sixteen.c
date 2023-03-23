#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "convert.h"
#include "node.h"

int sixteenAnalyzeInput( FILE* in, int* charcounter, int uniqueCounter ) {
    // for character conversion from binary to decimal 
    unsigned short input;
    // bufor for reading from input file
    char *inputBufor = malloc( 1200 * sizeof *inputBufor );
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
                if( ( unsigned short )inputBufor[i] >= 0 && ( unsigned short )inputBufor[i] <= 255 && 
                   ( unsigned short )inputBufor[i+1] >= 0 && ( unsigned short )inputBufor[i+1] <= 255 ) {

                    // convert two bytes to one decimal value
                    input = pow( 2, 8 ) * ( unsigned short )inputBufor[i] + ( unsigned short )inputBufor[i+1];

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
                if( ( unsigned short )inputBufor[i] >= 0 && ( unsigned short )inputBufor[i] <= 255 && 
                    ( unsigned short )inputBufor[i+1] >= 0 && ( unsigned short )inputBufor[i+1] <= 255 ) {
                    
                    // convert two bytes to one decimal value
                    input = pow( 2, 8 ) * ( unsigned short )inputBufor[i] + ( unsigned short )inputBufor[i+1];
                    
                    charcounter[input]++;
                    if ( charcounter[input] == 1 )
                        uniqueCounter++;
                }
            }
        }

        // there was odd number of bytes 
        if( isOdd ) {
            // make sure the input is correct
            if( ( unsigned short )inputBufor[inputBuforLength-1] >= 0 && ( unsigned short )inputBufor[inputBuforLength-1] <= 255 ) {

                // convert the byte to decimal value
                input = ( unsigned short )inputBufor[inputBuforLength-1];

                charcounter[input]++;
                if ( charcounter[input] == 1 )
                    uniqueCounter++;
            }
        }
    }
    free(inputBufor);
    return uniqueCounter;
}

void sixteenOutputGenerator( FILE* in, int uniqueCounter, unsigned short** codes, FILE *out, char password ) {
    // for character conversion from binary to decimal 
    unsigned short input;
    // bufor for reading from input file
    char *inputBufor = malloc( 1200 * sizeof *inputBufor );
    // size of input bufor
    int inputBuforLength;
    // to check if number of bytes is odd 
    int isOdd;
    // charcter that will be put in the output file
    char* character = malloc( sizeof *character );
    // binary representation of charcter   
    char *characterBinary = malloc( 8 * sizeof *characterBinary );
    characterBinary[0] = 0;
    // bufor with codes from characters found in the input
    char *bufor = malloc( 16384 * sizeof *bufor );
    // length of the bufor
    int buforLength = 0;

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
        input = ( unsigned short )inputBufor[0];

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
        }
        
        // convert code to char
        character[0] = binToDec(characterBinary) ^ password;
        
        // write character
        fwrite(character, 1, 1, out);
    }
    free( inputBufor );
    free( character );
    free( characterBinary );
    free( bufor );
}

int sixteenDictionary( node *pointer, node *border, node *root, node *last, FILE *out, int buforLength, char *bufor) {
        char *character = malloc( sizeof *character );
        char *characterBinary = malloc( 8 * sizeof *characterBinary );
        node *tmp = pointer;
        while( pointer->left != NULL) {
            pointer = pointer->left;
        }
        if( tmp == root) {
            char *chartmp = DectoBin( *(pointer->value), 16);
            for( int j=0; j < 16; j++) {
                bufor[buforLength] = chartmp[j];
                buforLength++;
            }
            free(chartmp);
        }
        else {
            //fprintf( stdout, "01");
            bufor[buforLength ] = '0';
            bufor[buforLength + 1] = '1';
            buforLength += 2;
            //fprintf( stdout, "%c", *(pointer->value));
            char *chartmp = DectoBin( (*pointer->value), 16);
            for( int j=0; j < 16; j++) {
                bufor[buforLength] = chartmp[j];
                buforLength++;
            }
            free(chartmp);
            // if enough bits in bufor
            while( buforLength >= 8 ) {
        
                // get code from bufor
                for(int i = 0; i < 8; i++) {
                    characterBinary[i] = bufor[i];
                    fprintf( stdout, "%c", bufor[i]);
                    
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
        
        while (pointer->parent != border) {
            if (  pointer->parent->right != NULL) {
                if ( pointer->parent->right->value != NULL) {
                    if( pointer->parent->right != last) {
                        //fprintf( stdout, "01");
                        bufor[buforLength ] = '0';
                        bufor[buforLength + 1] = '1';
                        buforLength += 2;
                        //fprintf( stdout, "%c", *(pointer->parent->right->value));
                        char *chartmp = DectoBin( (*pointer->parent->right->value), 16 );
                        for( int j=0; j < 16; j++) {
                            bufor[buforLength] = chartmp[j];
                            buforLength++;
                        }
                        free(chartmp);
                        // if enough bits in bufor
                        while( buforLength >= 8 ) {
                    
                            // get code from bufor
                            for(int i = 0; i < 8; i++) {
                                characterBinary[i] = bufor[i];
                                fprintf( stdout, "%c", bufor[i]);
                                
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
                    else {
                         //fprintf( stdout, "11");
                        bufor[buforLength ] = '1';
                        bufor[buforLength + 1] = '1';
                        buforLength += 2;
                        //fprintf( stdout, "%c", *(pointer->parent->right->value));
                        char *chartmp = DectoBin( *(pointer->parent->right->value), 16 );
                        for( int j=0; j < 16; j++) {
                            bufor[buforLength] = chartmp[j];
                            buforLength++;
                            //printf("%c", chartmp[j]);
                        }
                        free(chartmp);
                        // if enough bits in bufor
                        while( buforLength >= 8 ) {
                    
                            // get code from bufor
                            for(int i = 0; i < 8; i++) {
                                characterBinary[i] = bufor[i];
                                fprintf( stdout, "%c", bufor[i]);
                                
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
                }
                else  {
                    //fprintf( stdout, "00");
                    bufor[buforLength ] = '0';
                    bufor[buforLength + 1] = '0';
                    buforLength += 2;
                    // if enough bits in bufor
                    while( buforLength >= 8 ) {
                
                        // get code from bufor
                        for(int i = 0; i < 8; i++) {
                            characterBinary[i] = bufor[i];
                            fprintf( stdout, "%c", bufor[i]);
                            
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
                    buforLength = sixteenDictionary( pointer->parent->right, pointer->parent, root, last, out, buforLength, bufor);
                }
            }
             
            pointer = pointer->parent;
        }
        free(character);
        free(characterBinary);
        return buforLength;
    }
