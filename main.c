#include <stdio.h>
#include <stdlib.h>
#include "node.h"
#include "sort.h"
#include "queue.h"

int binToDec(char *binary){
    int power = 1;
    int output = 0;
    for(int i = 0; i < 8; i++) {
        output += (binary[7-i]-48) * power;
        power *= 2;
    }
    return output;
}

int main( int argc, char **argv) {

    if( argc < 2 ) {
        fprintf( stderr, "Lack of arguments!");
        return EXIT_FAILURE;
    }

    FILE *in = fopen( argv[1], "r");
    if( in == NULL) {
        fprintf( stderr, "This file doesn't exist!");
        return EXIT_FAILURE;
    }

    char c;
    int counter, uniquecounter = 0;
    int charcounter[128] =  { 0 };
    // counting characters in file
    while ( ( c = fgetc(in) ) != EOF) {
        counter ++;
        if( c >= 0 &&  c <= 127)
            charcounter[c]++;
            if (charcounter[c] == 1)
                uniquecounter++;
    }
    printf("unique characters: %d\n", uniquecounter);

    char *arr = malloc( uniquecounter * sizeof(char));
    int *freq = malloc( uniquecounter * sizeof(int));

    for (int i = 0, j = 0; i < 128; i++) {
        if( charcounter[i] != 0) {
            arr[j] = i;
            freq[j] = charcounter[i];
            j++;
        }
    }

    //char arr[] = { 'a', 'b', 'c', 'd', 'e'};
    //int freq[] = { 3, 2, 1, 4, 1};
    
 
    //int size = sizeof(arr) / sizeof(arr[0]);
    int size = uniquecounter;
    int queueSize = 2 * size - 1;

    // output codes array 
    // first row char
    // second row code length
    // next code
    char **codes;
    codes = malloc(size * sizeof *codes);
    for(int i = 0; i < size; i++)
        codes[i] = NULL;
        
    char tmp[size - 1];
    
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
        printf("c: %c   ", codes[i][0]);
        for(int j = 0; j < (int)codes[i][1]; j++){
            printf("%c", codes[i][j+2]);
        }
        
        printf("\n");
    }


    FILE *out = fopen( "output.huff", "wb+");

    rewind(in);

    char* character = malloc(sizeof *character);
    char *characterBinary = malloc(8 * sizeof *characterBinary);
    characterBinary[0] = 0;
    char *bufor = malloc(256 * sizeof *bufor);
    int buforLength = 0;

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
