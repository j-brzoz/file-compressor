#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "node.h"
#include "sort.h"
#include "queue.h"

int binToDec(char *binary) {
    int power = 1;
    int output = 0;
    for(int i = 0; i < 8; i++) {
        output += (binary[7-i]-48) * power;
        power *= 2;
    }
    return output;
}  

int main( int argc, char **argv) {
    
    // check if file has been given
    if( argc < 2 ) {
        fprintf( stderr, "Lack of arguments!");
        return EXIT_FAILURE;
    }

    // check if file exists
    FILE *in = fopen( argv[1], "r");
    if( in == NULL) {
        fprintf( stderr, "This file doesn't exist!");
        return EXIT_FAILURE;
    }

    // check if compression rate has been given
    if( argc < 3 ) {
        fprintf( stderr, "Lack of arguments!");
        return EXIT_FAILURE;
    }

    // check if given compression rate is correct
    int compressionRate = atoi(argv[2]);
    
    // amount of bytes that we will read as one char
    int inputSize;

    if(compressionRate == 1)
        inputSize = 8;
    else if(compressionRate == 2)
        inputSize = 12;
    else if(compressionRate == 3)
        inputSize = 16;
    else {
        fprintf( stderr, "Wrong compression rate! Available rates: 1 or 2 or 3");
        return EXIT_FAILURE;
    }


    // for reading from the file
    char c;
    
    // number if chars in the file
    int counter = 0;
    
    // number of unique chars
    int uniquecounter = 0;
    
    // number of possible chars
    int possibleChars = pow(2, inputSize);
    unsigned short input;
    char *inputBufor = malloc(1200 * sizeof *inputBufor);
    int inputBuforLength;
    int isOdd;
    char maska1 = 16 + 32 + 64 + 128;
    char maska2 = 1 + 2 + 4 + 8;
    // array where index represents char and 
    // value how many times that char has been found in the file
    int *charcounter;
    int remainder;
    if (inputSize == 8)
        charcounter =  malloc(possibleChars * sizeof *charcounter);
    else if (inputSize == 12) {
        possibleChars += 2;
        charcounter =  malloc(possibleChars * sizeof *charcounter);
    }
    else if (inputSize == 16) {
        possibleChars++;
        charcounter =  malloc(possibleChars * sizeof *charcounter);
    }
    for(int i = 0; i < possibleChars; i++)
        charcounter[i] = 0;

    if(inputSize == 8){ 
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
        
    }
    else if(inputSize == 16){
        // 16
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
    }
    else if(inputSize == 12){
        // 12
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
    }


    unsigned short *arr = malloc( uniquecounter * sizeof *arr);
    int *freq = malloc( uniquecounter * sizeof *freq);

    for (int i = 0, j = 0; i < possibleChars; i++) {
        if( charcounter[i] != 0) {
            arr[j] = i;
            freq[j] = charcounter[i];
            j++;
        }
    }

    // for(int i = 0; i < 4; i++){
    //     printf("%d %d\n", arr[i], freq[i]);
    // }
    // printf("\n");

    //char arr[] = { 'a', 'b', 'c', 'd', 'e'};
    //int freq[] = { 3, 2, 1, 4, 1};
    
    //========================================================================================================================
    //int size = sizeof(arr) / sizeof(arr[0]);
    int size = uniquecounter;
    int queueSize = 2 * size - 1;

    // output codes array 
    // first row char
    // second row code length
    // next code
    unsigned short **codes;
    codes = malloc(size * sizeof *codes);
    for(int i = 0; i < size; i++)
        codes[i] = NULL;
        
    unsigned short tmp[size - 1];
    
    // make queue
    node **queue = malloc(queueSize * sizeof *queue);
    for(int i = 0; i < queueSize; i++)
        queue[i] = NULL;

    // put inputs in queue
    for(int i = 0; i < size; i++){
        node *tmpNode = makeNode(arr+i, freq[i], false, NULL, NULL);
        if(tmpNode == NULL){
            return 1;
        }
        queue[i] = tmpNode;
    }
    
    // sort inputs
    quickSortQueue(queue, 0, size - 1);
    
    // make a binary tree
    int temp = 0;
    while(notFull(queue, queueSize)){
        addNewNodeToQueue(queue, queueSize);
        quickSortQueue(queue, 2*(temp+1), size + temp);
        temp++;
    }
    
    // read codes from tree
    readCodes(queue[queueSize-1], size, codes, tmp, 0);

    // print codes
    for(int i = 0; i < size; i++){
        printf("c:%d i:%d   ", codes[i][0], i);
        for(int j = 0; j < codes[i][1]; j++){
            printf("%c", (char)codes[i][j+2]);
        }
        
        printf("\n");
    }

    //===============================================================================================================

    FILE *out = fopen( "output.huff", "wb+");

    rewind(in);

    char* character = malloc(sizeof *character);
    char *characterBinary = malloc(8 * sizeof *characterBinary);
    characterBinary[0] = 0;
    char *bufor = malloc(16384 * sizeof *bufor);
    int buforLength = 0;
    int isLast = 0;
    if(inputSize == 8){
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
    else if(inputSize == 16){
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
    else if(inputSize == 12){
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

    printf("everything went well!\n");
    // free memory
    for(int i = 0; i < queueSize; i++){
        free(queue[i]);
    }
    free(queue);

    for(int i = 0; i < size; i++){
        free(codes[i]);
    }
    free(codes);
    free(bufor);
    free(characterBinary);
    free(character);

    fclose(out);
    fclose(in);
    
    return 0;
}
