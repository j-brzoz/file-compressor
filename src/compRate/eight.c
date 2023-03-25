#include <stdio.h>
#include <stdlib.h>
#include "../tools/convert.h"
#include "../queue/node.h"
#include "../tools/fileHeader.h"

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

void eightOutputGenerator( FILE* in, int uniqueCounter, unsigned short** codes, FILE *out, char password, char *remainingChar, int remainingLen ) {
    
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
    // crc
    char crc = 'J';
    // count how many zeros we add artficially
    int zeroCounter = 0;

    // add remaining chars from dictionary
    for(int i = 0; i < remainingLen; i++){
        bufor[i] = remainingChar[i];
    }
    buforLength += remainingLen;

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
            character[0] = binToDec( characterBinary ) ^ password;

            crc = crc ^ binToDec( characterBinary );
            
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

        crc = crc ^ binToDec( characterBinary );
        
        // write character
        fwrite( character, 1, 1, out );
    }

    header(out, 8, password, zeroCounter, crc);

    free( bufor );
    free( character );
    free( characterBinary );
}

int eightDictionary( node *pointer, node *border, node *root, node *last, FILE *out, int buforLength, char *bufor) {
        char *character = malloc( sizeof *character );
        char *characterBinary = malloc( 8 * sizeof *characterBinary );
        node *tmp = pointer;
        while( pointer->left != NULL) {
            pointer = pointer->left;
        }
        if( tmp == root) {
            char *chartmp = DectoBin( *(pointer->value), 8 );
            for( int j=0; j < 8; j++) {
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
            char *chartmp = DectoBin( (*pointer->value), 8);
            for( int j=0; j < 8; j++) {
                bufor[buforLength] = chartmp[j];
                buforLength++;
            }
            free(chartmp);
            // if enough bits in bufor
            while( buforLength >= 8 ) {
        
                // get code from bufor
                for(int i = 0; i < 8; i++) {
                    characterBinary[i] = bufor[i];
                    // fprintf( stdout, "%c", bufor[i]);
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
                        char *chartmp = DectoBin( (*pointer->parent->right->value), 8 );
                        for( int j=0; j < 8; j++) {
                            bufor[buforLength] = chartmp[j];
                            buforLength++;
                        }
                        free(chartmp);
                        // if enough bits in bufor
                        while( buforLength >= 8 ) {
                    
                            // get code from bufor
                            for(int i = 0; i < 8; i++) {
                                characterBinary[i] = bufor[i];
                                // fprintf( stdout, "%c", bufor[i]);            
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
                        char *chartmp = DectoBin( *(pointer->parent->right->value), 8 );
                        for( int j=0; j < 8; j++) {
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
                                // fprintf( stdout, "%c", bufor[i]);
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
                            // fprintf( stdout, "%c", bufor[i]);
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
                    buforLength = eightDictionary( pointer->parent->right, pointer->parent, root, last, out, buforLength, bufor);
                }
            }
             
            pointer = pointer->parent;
        }
        free(character);
        free(characterBinary);
        return buforLength;
    }