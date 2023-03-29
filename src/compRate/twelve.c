#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdlib.h>
#include "../tools/convert.h"
#include "../queue/node.h"
#include "../tools/fileHeader.h"

int twelveAnalyzeInput( FILE* in, int* charcounter, int uniqueCounter ) {
    // for character conversion from binary to decimal 
    unsigned short input;
    // bufor for reading from input file
    unsigned char *inputBufor = malloc( 1200 * sizeof *inputBufor );
    // size of input bufor
    int inputBuforLength;
    // for getting relevant bites
    unsigned char mask1 = 16 + 32 + 64 + 128;
    // for getting relevant bites
    unsigned char mask2 = 1 + 2 + 4 + 8;
    // to check what is the remainder of division by 3
    int remainder;

    // read 1200 bytes
    while( inputBuforLength = fread( inputBufor, 1, 1200, in ) ) {

        // check what is the remainder of division by 3
        remainder = inputBuforLength % 3;
        
        // if all 1200 bytes were read
        if( inputBuforLength == 1200 ) {
            for( int i = 0; i < 1200; i+=3 ) {
                
                // make sure the input is correct
                if( inputBufor[i] >= 0 && inputBufor[i] <= 255 && 
                   inputBufor[i+1] >= 0 && inputBufor[i+1] <= 255 && 
                   inputBufor[i+2] >= 0 && inputBufor[i+2] <= 255 ) {
                    
                    // convert 1 + 0,5 bytes to decimal
                    input = ( unsigned short )pow( 2, 4 ) * inputBufor[i] + 
                        ( ( inputBufor[i+1] & mask1 ) / ( unsigned short )pow( 2, 4 ) );
                    charcounter[input]++;
                    if( charcounter[input] == 1 )
                        uniqueCounter++;
                
                    // convert 0,5 + 1 bytes to decimal
                    input = ( unsigned short )pow( 2, 8 ) * ( inputBufor[i+1] & mask2 ) + inputBufor[i+2];
                    charcounter[input]++;
                    if ( charcounter[input] == 1 )
                        uniqueCounter++;
                }
            }
        }
        // if not all 1200 bytes were read
        else {
            for(int i = 0; i < inputBuforLength - remainder; i+=3){
                
                // make sure the input is correct
                if( inputBufor[i] >= 0 && inputBufor[i] <= 255 && 
                inputBufor[i+1] >= 0 && inputBufor[i+1] <= 255 && 
                inputBufor[i+2] >= 0 && inputBufor[i+2] <= 255 ) {
                    
                    // convert 1 + 0,5 bytes to decimal
                    input = ( unsigned short )pow( 2, 4 ) * inputBufor[i] + 
                        ( ( inputBufor[i+1] & mask1 ) / ( unsigned short )pow( 2, 4 ));
                    charcounter[input]++;
                    if ( charcounter[input] == 1 )
                        uniqueCounter++;

                    // convert 0,5 + 1 bytes to decimal
                    input = ( unsigned short )pow( 2, 8 ) * ( inputBufor[i+1] & mask2 ) + inputBufor[i+2];
                    charcounter[input]++;
                    if( charcounter[input] == 1)
                        uniqueCounter++;
                }
            }
        }

        // if remainder is not equal to zero
        if( remainder == 2 ) {
            
            // convert 1 + 0,5 bytes to decimal
            if( inputBufor[inputBuforLength-1] >= 0 && inputBufor[inputBuforLength-1] <= 255 && 
                inputBufor[inputBuforLength-2] >= 0 && inputBufor[inputBuforLength-2] <= 255 ) {
                
                // convert 1 + 0,5 bytes to decimal
                input = ( unsigned short )pow( 2, 4 ) * inputBufor[inputBuforLength-2] + 
                    ( ( inputBufor[inputBuforLength-1] & mask1 ) / ( unsigned short )pow( 2, 4 ) );
                charcounter[input]++;
                if ( charcounter[input] == 1 )
                    uniqueCounter++;
                
                // convert 0,5 byte to decimal
                input =  ( inputBufor[inputBuforLength-1] & mask2 );
                charcounter[input]++;
                if ( charcounter[input] == 1 )
                    uniqueCounter++;
            }
        }
        else if(remainder == 1) {

            // make sure the input is correct
            if( inputBufor[inputBuforLength-1] >= 0 && inputBufor[inputBuforLength-1] <= 255 ) {
                
                // convert 1 byte to decimal
                input = inputBufor[inputBuforLength-1];
                charcounter[input]++;
                if ( charcounter[input] == 1 )
                    uniqueCounter++;
            }
        }
    }
    free(inputBufor);
    return uniqueCounter;
}

void twelveOutputGenerator( FILE* in, int uniqueCounter, unsigned short** codes, FILE *out, char password, char *remainingChar, int remainingLen ) {
    // for character conversion from binary to decimal 
    unsigned short input;
    // bufor for reading from input file
    unsigned char *inputBufor = malloc( 1200 * sizeof *inputBufor );
    // size of input bufor
    int inputBuforLength;
    // for getting relevant bites
    unsigned char mask1 = 16 + 32 + 64 + 128;
    // for getting relevant bites
    unsigned char mask2 = 1 + 2 + 4 + 8;
    // to check what is the remainder of division by 3
    int remainder;
    // charcter that will be put in the output file
    unsigned char* character = malloc( sizeof *character );
    // binary representation of charcter   
    char *characterBinary = malloc( 8 * sizeof *characterBinary );
    // bufor with codes from characters found in the input
    unsigned char *bufor = malloc( 16384 * sizeof *bufor );
    // length of the bufor
    int buforLength = 0;
    // crc
    unsigned char crc = 'J';
    // count how many zeros we add artficially
    int zeroCounter = 0;

    // add remaining chars from dictionary
    for(int i = 0; i < remainingLen; i++){
        bufor[i] = remainingChar[i];
    }
    buforLength += remainingLen;

    // get three bytes
    while( inputBuforLength = fread( inputBufor, 1, 3, in ) ) {

        // check what is the remainder of division by 3
        remainder = inputBuforLength % 3;

        // if 3 bytes were read
        if( inputBuforLength == 3 ) {
            
            // convert 1 + 0,5 bytes to decimal
            input = ( unsigned short )pow( 2, 4 ) * ( unsigned short )inputBufor[0] + 
                ( ( ( unsigned short )( inputBufor[1] & mask1 ) ) / ( unsigned short )pow( 2, 4 ) );

            // find value in codes
            for(int k = 0; k < uniqueCounter; k++) {
                if(input == codes[k][0]) {
                    
                    // write code to bufor
                    for(int j = 0; j < codes[k][1]; j++) {
                        bufor[buforLength+j] = (char)codes[k][j+2];
                    }
                    buforLength += codes[k][1];
                }
            }

            // convert 0,5 + 1 bytes to decimal
            input = ( unsigned short )pow( 2, 8 ) * ( ( unsigned short )( inputBufor[1] & mask2 ) ) + 
                ( unsigned short )inputBufor[2];

            // find value in codes
            for( int k = 0; k < uniqueCounter; k++ ) {
                if( input == codes[k][0] ) {

                    // write code to bufor
                    for( int j = 0; j < codes[k][1]; j++ ) {
                        bufor[buforLength+j] = ( char )codes[k][j+2];
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
            
            crc = crc ^ binToDec( characterBinary );
            
            // write character
            fwrite( character, 1, 1, out );
            
            // move codes in bufor
            for( int i = 0; i < buforLength - 8; i++ ){
                bufor[i] = bufor[i + 8];
            }
            buforLength -= 8;
        }  
    }

    // if remainder is not equal to zero
    if( remainder == 2 ) {

        // convert 1 + 0,5 bytes to decimal
        input = ( unsigned short )pow( 2, 4 ) * ( unsigned short )inputBufor[0] + 
            ( ( ( unsigned short )( inputBufor[1] & mask1 ) ) / ( unsigned short )pow( 2, 4 ) );

        // find value in codes
        for( int k = 0; k < uniqueCounter; k++)  {
            if( input == codes[k][0] ) {

                // write code to bufor
                for( int j = 0; j < codes[k][1]; j++ ) {
                    bufor[buforLength+j] = ( char )codes[k][j+2];
                }
                buforLength += codes[k][1];
            }
        }

        // convert 0,5 byte to decimal
        input =  ( ( unsigned short )( inputBufor[1] & mask2 ) );

        // find value in codes
        for( int k = 0; k < uniqueCounter; k++ ) {
            if( input == codes[k][0] ) {

                // write code to bufor
                for( int j = 0; j < codes[k][1]; j++ ) {
                    bufor[buforLength+j] = ( char )codes[k][j+2];
                }
                buforLength += codes[k][1];
            }
        }
    }
    else if( remainder == 1 ){
        
        // convert 1 byte to decimal
        input = ( unsigned short )inputBufor[0];
        
        // find value in codes
        for( int k = 0; k < uniqueCounter; k++ ) {
            if( input == codes[k][0] ) {

                // write code to bufor
                for( int j = 0; j < codes[k][1]; j++ ) {;
                    bufor[buforLength+j] = ( char )codes[k][j+2];
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
            
            crc = crc ^ binToDec( characterBinary );
            
            // write character
            fwrite( character, 1, 1, out );
            
            // move codes in bufor
            for( int i = 0; i < buforLength - 8; i++ ) {
                bufor[i] = bufor[i + 8];
            }
            buforLength -= 8;
    }
    if( buforLength != 0 ){
        
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

    header(out, 12, password, zeroCounter, crc);

    free( inputBufor );
    free( character );
    free( characterBinary );
    free( bufor );
}

int twelveDictionary( node *pointer, node *border, node *root, node *last, FILE *out, int buforLength, char *bufor) {
        char *character = malloc( sizeof *character );
        char *characterBinary = malloc( 8 * sizeof *characterBinary );
        node *tmp = pointer;
        while( pointer->left != NULL) {
            pointer = pointer->left;
        }
        if( tmp == root) {
            char *chartmp = DectoBin( *(pointer->value), 12);
            for( int j=0; j < 12; j++) {
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
            char *chartmp = DectoBin( (*pointer->value), 12);
            for( int j=0; j < 12; j++) {
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
                        char *chartmp = DectoBin( (*pointer->parent->right->value), 12 );
                        for( int j=0; j < 12; j++) {
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
                        char *chartmp = DectoBin( *(pointer->parent->right->value), 12 );
                        for( int j=0; j < 12; j++) {
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
                    buforLength = twelveDictionary( pointer->parent->right, pointer->parent, root, last, out, buforLength, bufor);
                }
            }
             
            pointer = pointer->parent;
        }
        free(character);
        free(characterBinary);
        return buforLength;
    }
