#include <stdio.h>
#include <math.h>
#include "convert.h"

int sixteenAnalyzeInput(int inputBuforLength, char* inputBufor, FILE* in, int* charcounter, int uniquecounter, unsigned short input){
    int isOdd;
    while( inputBuforLength = fread( inputBufor, 1, 1200, in ) ) {
        isOdd = inputBuforLength % 2;
        if(inputBuforLength == 1200) {
            for(int i = 0; i < 1200; i+=2) {
                if((unsigned short)inputBufor[i] >= 0 && (unsigned short)inputBufor[i] <= 255 && (unsigned short)inputBufor[i+1] >= 0 && (unsigned short)inputBufor[i+1] <= 255){
                    input = pow(2, 8) * (int)inputBufor[i] + (int)inputBufor[i+1];
                    charcounter[input]++;
                    if (charcounter[input] == 1)
                        uniquecounter++;
                }
            }
        }
        else {
            for(int i = 0; i < inputBuforLength - isOdd - 1; i+=2) {
                if((unsigned short)inputBufor[i] >= 0 && (unsigned short)inputBufor[i] <= 255 && (unsigned short)inputBufor[i+1] >= 0 && (unsigned short)inputBufor[i+1] <= 255){
                    input = pow(2, 8) * (int)inputBufor[i] + (int)inputBufor[i+1];
                    charcounter[input]++;
                    if (charcounter[input] == 1)
                        uniquecounter++;
                }
            }
        }
        if(isOdd) {
            // printf("input: %d\n", (int)inputBufor[inputBuforLength-1]);
            if((unsigned short)inputBufor[inputBuforLength-1] >= 0 && (unsigned short)inputBufor[inputBuforLength-1] <= 255){
                input = (unsigned short)inputBufor[inputBuforLength-1];
                charcounter[input]++;
                if (charcounter[input] == 1)
                    uniquecounter++;
            }
        }
    }
    return uniquecounter;
}

void sixteenOutputGenerator(int inputBuforLength, char* inputBufor, FILE* in, unsigned short input, char* bufor, int buforLength, char* character, char* characterBinary, int size, unsigned short** codes, FILE *out){
    int isOdd;
    while( inputBuforLength = fread( inputBufor, 1, 2, in )) {
        isOdd = inputBuforLength % 2;
        if(inputBuforLength == 2) {
            for(int i = 0; i < 2; i+=2) {
                input = pow(2, 8) * (int)inputBufor[i] + (int)inputBufor[i+1];
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
    if(isOdd){
        for(int i = 0; i < 2; i+=2) {
            input = (int)inputBufor[i];
            // printf("input: %c\n", (char)input);
            for(int k = 0; k < size; k++) {
                // printf("codes[%d][0]: %d\n", k, codes[i][0]);
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
        isOdd = 0;
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