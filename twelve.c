#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "convert.h"
#include "node.h"

int twelveAnalyzeInput(int inputBuforLength, char* inputBufor, FILE* in, int* charcounter, int uniquecounter, unsigned short input) {
    char maska1 = 16 + 32 + 64 + 128;
    char maska2 = 1 + 2 + 4 + 8;
    int remainder;
    while( inputBuforLength = fread( inputBufor, 1, 1200, in ) ) {
        remainder = inputBuforLength % 3;
        if(inputBuforLength == 1200) {
            for(int i = 0; i < 1200; i+=3){
                // 1 + 0,5
                if((unsigned short)inputBufor[i] >= 0 && (unsigned short)inputBufor[i] <= 255 && (unsigned short)inputBufor[i+1] >= 0 && (unsigned short)inputBufor[i+1] <= 255 && (unsigned short)inputBufor[i+2] >= 0 && (unsigned short)inputBufor[i+2] <= 255){
                    input = (unsigned short)pow(2, 4) * (unsigned short)inputBufor[i] + (((unsigned short)(inputBufor[i+1] & maska1)) / (unsigned short)pow(2, 4));
                    charcounter[input]++;
                    if (charcounter[input] == 1)
                        uniquecounter++;
                
                    // 0,5 + 1
                    input = (unsigned short)pow(2, 8) * ((unsigned short)(inputBufor[i+1] & maska2)) + (unsigned short)inputBufor[i+2];
                    charcounter[input]++;
                    if (charcounter[input] == 1)
                        uniquecounter++;
                }
            }
        }
        else {
            for(int i = 0; i < inputBuforLength - remainder; i+=3){
                if((unsigned short)inputBufor[i] >= 0 && (unsigned short)inputBufor[i] <= 255 && (unsigned short)inputBufor[i+1] >= 0 && (unsigned short)inputBufor[i+1] <= 255 && (unsigned short)inputBufor[i+2] >= 0 && (unsigned short)inputBufor[i+2] <= 255){
                    // 1 + 0,5
                    input = (unsigned short)pow(2, 4) * (unsigned short)inputBufor[i] + (((unsigned short)(inputBufor[i+1] & maska1)) / (unsigned short)pow(2, 4));
                    charcounter[input]++;
                    if (charcounter[input] == 1)
                        uniquecounter++;
                    // 0,5 + 1
                    input = (unsigned short)pow(2, 8) * ((unsigned short)(inputBufor[i+1] & maska2)) + (unsigned short)inputBufor[i+2];
                    // printf("input: %d\n", input);
                    // printf("1: %d\n", (unsigned short)pow(2, 8) * ((unsigned short)(inputBufor[i] & maska2)));
                    // printf("2: %d\n", (unsigned short)inputBufor[i+1]);
                    charcounter[input]++;
                    if (charcounter[input] == 1)
                        uniquecounter++;
                }
            }
        }
        if(remainder == 2) {
            if((unsigned short)inputBufor[inputBuforLength-1] >= 0 && (unsigned short)inputBufor[inputBuforLength-1] <= 255 && (unsigned short)inputBufor[inputBuforLength-2] >= 0 && (unsigned short)inputBufor[inputBuforLength-2] <= 255){
                // 1 + 0,5 
                input = (unsigned short)pow(2, 4) * (unsigned short)inputBufor[inputBuforLength-2] + (((unsigned short)(inputBufor[inputBuforLength-1] & maska1)) / (unsigned short)pow(2, 4));
                charcounter[input]++;
                if (charcounter[input] == 1)
                    uniquecounter++;
                // 0,5
                input =  ((unsigned short)(inputBufor[inputBuforLength-1] & maska2));
                charcounter[input]++;
                if (charcounter[input] == 1)
                    uniquecounter++;
            }
        }
        else if(remainder == 1) {
                if((unsigned short)inputBufor[inputBuforLength-1] >= 0 && (unsigned short)inputBufor[inputBuforLength-1] <= 255){
                // 1
                input = (unsigned short)inputBufor[inputBuforLength-1];
                charcounter[input]++;
                if (charcounter[input] == 1)
                    uniquecounter++;
            }
        }
    }
    return uniquecounter;
}

void twelveOutputGenerator(int inputBuforLength, char* inputBufor, FILE* in, unsigned short input, char* bufor, int buforLength, char* character, char* characterBinary, int size, unsigned short** codes, FILE *out){
    char maska1 = 16 + 32 + 64 + 128;
    char maska2 = 1 + 2 + 4 + 8;
    int remainder;
    while( inputBuforLength = fread( inputBufor, 1, 3, in )) {
        remainder = inputBuforLength % 3;
        if(inputBuforLength == 3) {
            for(int i = 0; i < 3; i+=3) {
                // 1 + 0,5 
                input = (unsigned short)pow(2, 4) * (unsigned short)inputBufor[i] + (((unsigned short)(inputBufor[i+1] & maska1)) / (unsigned short)pow(2, 4));
                for(int k = 0; k < size; k++) {
                    if(input == codes[k][0]) {
                        // write code to bufor
                        for(int j = 0; j < codes[k][1]; j++) {
                            bufor[buforLength+j] = (char)codes[k][j+2];
                        }
                        buforLength += codes[k][1];
                    }
                }

                // 0,5 + 1
                input = (unsigned short)pow(2, 8) * ((unsigned short)(inputBufor[i+1] & maska2)) + (unsigned short)inputBufor[i+2];
                for(int k = 0; k < size; k++) {
                    if(input == codes[k][0]) {
                        // write code to bufor
                        for(int j = 0; j < codes[k][1]; j++) {
                            bufor[buforLength+j] = (char)codes[k][j+2];
                        }
                        buforLength += codes[k][1];
                    }
                }
            }
        }
        // for(int i = 0; i < buforLength; i++){
        //     printf("%c ", bufor[i]);
        // }
        // printf("\n");
        while(buforLength >= 7) {
        
            // get code from bufor
            for(int i = 1; i < 8; i++) {
                characterBinary[i] = bufor[i-1];
            }
            
            // convert code to char
            character[0] = binToDec(characterBinary);
            
            // write character
            fwrite(character, 1, 1, out);
            
            // move codes in bufor
            for(int i = 0; i < buforLength - 7; i++){
                bufor[i] = bufor[i + 7];
            }
            buforLength -= 7;
        }
        
    }
    // printf("bufor:\n");
    // for(int i = 0; i < buforLength; i++){
    //     printf("%c ", bufor[i]);
    // }
    // printf("\n");
    // printf("inputbufor:\n");
    // for(int i = 0; i < inputBuforLength; i++){
    //     printf("%c ", inputBufor[i]);
    // }
    // printf("\n");
    // printf("odd: %d\n", isOdd);
    if(remainder == 2){
        for(int i = 0; i < 2; i+=2) {
            input = (unsigned short)pow(2, 4) * (unsigned short)inputBufor[i] + (((unsigned short)(inputBufor[i+1] & maska1)) / (unsigned short)pow(2, 4));
            // printf("input: %c\n", (char)input);
            for(int k = 0; k < size; k++) {
                // printf("codes[%d][0]: %d\n", k, codes[k][0]);
                if(input == codes[k][0]) {
                    // write code to bufor
                    for(int j = 0; j < codes[k][1]; j++) {
                        // printf("c:%d %d\n", codes[k][0], (char)codes[k][j+2]);
                        bufor[buforLength+j] = (char)codes[k][j+2];
                    }
                    buforLength += codes[k][1];
                }
            }
            input =  ((unsigned short)(inputBufor[i+1] & maska2));
            // printf("input: %c\n", (char)input);
            for(int k = 0; k < size; k++) {
                // printf("codes[%d][0]: %d\n", k, codes[k][0]);
                if(input == codes[k][0]) {
                    // write code to bufor
                    for(int j = 0; j < codes[k][1]; j++) {
                        // printf("c:%d %d", codes[k][0], (char)codes[k][j+2]);
                        bufor[buforLength+j] = (char)codes[k][j+2];
                    }
                    buforLength += codes[k][1];
                }
            }
        }
    }
    else if(remainder == 1){
        for(int i = 0; i < 1; i+=1) {
            input = (unsigned short)inputBufor[i];
            // printf("input: %c\n", (char)input);
            for(int k = 0; k < size; k++) {
                // printf("codes[%d][0]: %d\n", k, codes[k][0]);
                if(input == codes[i][0]) {
                    // write code to bufor
                    for(int j = 0; j < codes[k][1]; j++) {
                        // printf("c:%d %d", codes[k][0], (char)codes[k][j+2]);
                        bufor[buforLength+j] = (char)codes[k][j+2];
                    }
                    buforLength += codes[k][1];
                }
            }
        }
    }
    // for(int i = 0; i < buforLength; i++){
    //     printf("%c ", bufor[i]);
    // }
    // printf("\n");
    while(buforLength >= 7) {
        
            // get code from bufor
            for(int i = 1; i < 8; i++) {
                characterBinary[i] = bufor[i-1];
            }
            
            // convert code to char
            character[0] = binToDec(characterBinary);
            
            // write character
            fwrite(character, 1, 1, out);
            
            // move codes in bufor
            for(int i = 0; i < buforLength - 7; i++){
                bufor[i] = bufor[i + 7];
            }
            buforLength -= 7;
    }
    if(buforLength != 0){
        
        // get code from bufor
        for(int i = 1; i <= buforLength; i++) {
                characterBinary[i] = bufor[i-1];
        }
        
        // level up to 8 bits
        for(int i = buforLength+1; i < 8; i++) {
                characterBinary[i] = '0';
        }
        
        // convert code to char
        character[0] = binToDec(characterBinary);
        
        // write character
        fwrite(character, 1, 1, out);
    }
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
                    buforLength = twelveDictionary( pointer->parent->right, pointer->parent, root, last, out, buforLength, bufor);
                }
            }
             
            pointer = pointer->parent;
        }
        free(character);
        free(characterBinary);
        return buforLength;
    }