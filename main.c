#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "node.h"
#include "sort.h"
#include "queue.h"
#include "eight.h"
#include "twelve.h"
#include "sixteen.h"

int main( int argc, char **argv) {
    
    // check if file and compression rate have been given
    if( argc < 3 ) {
        fprintf( stderr, "Lack of arguments!");
        return EXIT_FAILURE;
    }

    // check if file exists
    FILE *in = fopen( argv[1], "r");
    if( in == NULL) {
        fprintf( stderr, "This file doesn't exist!");
        return EXIT_FAILURE;
    }

    // check if given compression rate is correct
    int compressionRate = atoi(argv[2]);
    
    // amount of bytes that we will read as one char
    int inputSize;

    // assign size according to compression rate
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


    // for chars reading from the file
    char c;
    // number of unique chars
    int uniqueCounter = 0;
    // number of possible chars
    int possibleChars ;
    // bufor for reading from input file
    char *inputBufor = malloc(1200 * sizeof *inputBufor);
    // character conversion from binary to decimal 
    unsigned short input;
    // size of input bufor
    int inputBuforLength;
    // array where index represents char and 
    // value how many times that char has been found in the file
    int *charcounter;
    
    // allocate charcounter
    if (inputSize == 8){
        possibleChars = pow(2, inputSize);
        charcounter =  malloc(possibleChars * sizeof *charcounter);
    }
    else if (inputSize == 12) {
        possibleChars = pow(2, inputSize) + 2;
        charcounter =  malloc(possibleChars * sizeof *charcounter);
    }
    else if (inputSize == 16) {
        possibleChars = pow(2, inputSize) + 1;
        charcounter =  malloc(possibleChars * sizeof *charcounter);
    }

    // make sure charcounter is filled with 0s
    for(int i = 0; i < possibleChars; i++)
        charcounter[i] = 0;

    // analyze input
    if(inputSize == 8)
        uniqueCounter = eightAnalyzeInput(in, charcounter, uniqueCounter);
    else if(inputSize == 16)
        uniqueCounter = sixteenAnalyzeInput(inputBuforLength, inputBufor, in, charcounter, uniqueCounter, input);
    else if(inputSize == 12)
        uniqueCounter = twelveAnalyzeInput(inputBuforLength, inputBufor, in, charcounter, uniqueCounter, input);

    // characters found in the input
    unsigned short *arr = malloc( uniqueCounter * sizeof *arr);
    // frequency of charcters from the input
    int *freq = malloc( uniqueCounter * sizeof *freq);

    // convert data from charcounter to arr and freq
    for (int i = 0, j = 0; i < possibleChars; i++) {
        if( charcounter[i] != 0) {
            arr[j] = i;
            freq[j] = charcounter[i];
            j++;
        }
    }
   
    // size of the queue
    int queueSize = 2 * uniqueCounter - 1;

    // output codes array 
    // first row character
    // second row code length
    // next code
    unsigned short **codes;
    codes = malloc(uniqueCounter * sizeof *codes);
    for(int i = 0; i < uniqueCounter; i++)
        codes[i] = NULL;
        
    // make a queue
    node **queue = malloc(queueSize * sizeof *queue);
    for(int i = 0; i < queueSize; i++)
        queue[i] = NULL;

    // put inputs in a queue
    for(int i = 0; i < uniqueCounter; i++){
        node *tmpNode = makeNode(arr+i, freq[i], false, NULL, NULL);
        if(tmpNode == NULL){
            return 1;
        }
        queue[i] = tmpNode;
    }
    
    // sort inputs
    quickSortQueue(queue, 0, uniqueCounter - 1);
    
    // make a binary tree
    int temp = 0;
    while(notFull(queue, queueSize)){
        addNewNodeToQueue(queue, queueSize);
        quickSortQueue(queue, 2*(temp+1), uniqueCounter + temp);
        temp++;
    }

    // temporary array for reading codes
    unsigned short tmp[uniqueCounter - 1];

    // read codes from tree
    readCodes(queue[queueSize-1], uniqueCounter, codes, tmp, 0);

    // print codes
    // for(int i = 0; i < uniqueCounter; i++){
    //     printf("c:%d i:%d   ", codes[i][0], i);
    //     for(int j = 0; j < codes[i][1]; j++){
    //         printf("%c", (char)codes[i][j+2]);
    //     }    
    //     printf("\n");
    // }

    // output file
    FILE *out = fopen( "output.huff", "wb+");

    //rewind the input file
    rewind(in);

    // charcter that will be put in the output file
    char* character = malloc(sizeof *character);
    // binary representation of charcter   
    char *characterBinary = malloc(8 * sizeof *characterBinary);
    characterBinary[0] = 0;
    // bufor with codes from characters found in the input
    char *bufor = malloc(16384 * sizeof *bufor);
    // length of the bufor
    int buforLength = 0;

    // generate output
    if(inputSize == 8)
        eightOutputGenerator(in, uniqueCounter, codes, out);
    else if(inputSize == 16)
        sixteenOutputGenerator(inputBuforLength, inputBufor, in, input, bufor, buforLength, character, characterBinary, uniqueCounter, codes, out);
    else if(inputSize == 12)
        twelveOutputGenerator(inputBuforLength, inputBufor, in, input, bufor, buforLength, character, characterBinary, uniqueCounter, codes, out);

    // check message
    printf("everything went well!\n");

    // free memory
    for(int i = 0; i < queueSize; i++)
        free(queue[i]);

    free(queue);

    for(int i = 0; i < uniqueCounter; i++){
        free(codes[i]);
    }
    free(codes);
    free(bufor);
    free(characterBinary);
    free(character);
    free(inputBufor);
    free(charcounter);
    free(freq);
    free(arr);

    fclose(out);
    fclose(in);
    
    return 0;
}
