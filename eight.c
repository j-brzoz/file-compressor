#include <stdio.h>
#include "convert.h"

int eightAnalyzeInput(int inputBuforLength, char* inputBufor, FILE* in, int* charcounter, int uniquecounter){ 
    // count chars in the file
    // while ( ( c = fgetc(in) ) != EOF) {
    //     counter++;
    //     fprintf(stderr, "check ");
    //     charcounter[(unsigned short)c]++;
    //     if (charcounter[(unsigned short)c] == 1)
    //         uniquecounter++;
    // }
    // fprintf(stderr, "check2 ");
    // printf("unique characters: %d\n", uniquecounter);
    int tempo = 0;
    while (inputBuforLength = fread( inputBufor, 1, 1200, in ))
    {
        for(int i = 0; i < inputBuforLength; i++){
            if((unsigned short)inputBufor[i] >= 0 && (unsigned short)inputBufor[i] <= 255){
                charcounter[(unsigned short)inputBufor[i]]++;
                if (charcounter[(unsigned short)inputBufor[i]] == 1)
                    uniquecounter++;
            }
        }
        tempo += 1200;
    }
    printf("unique characters: %d\n", uniquecounter);
    return uniquecounter;       
}

void eightOutputGenerator(char c, FILE* in, char* bufor, int buforLength, char* character, char* characterBinary, int size, unsigned short** codes, FILE *out){
    while ( ( c = fgetc(in) ) != EOF) {
            
        // find character in codes
        for(int i = 0; i < size; i++) {
            if(c == codes[i][0]) {
                
                // write code to bufor
                for(int j = 0; j < (int)codes[i][1]; j++) {
                    bufor[buforLength+j] = codes[i][j+2];
                }
                buforLength += (int)codes[i][1];
            }
        }
        
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