#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include "node.h"
#include "sort.h"
#include "queue.h"
#include "eight.h"
#include "twelve.h"
#include "sixteen.h"
#include "convert.h"



int main( int argc, char **argv) {

    // amount of bytes that we will read as one char
    int inputSize = 0;

    // default password for data encrypting (doesn't change anything)
    char password = 0;

    // mandatory options
    int checkCompRate = 0;
    int checkInput = 0;
    int checkOutput = 0;

    // input and outour files
    FILE *in;
    FILE *out;

    char *outputFile = NULL;
    char *inputFile = NULL;
    
    int c;
    while ((c = getopt (argc, argv, "123x:z:c:vho:")) != -1)
    switch (c) {
        case '1':       // compress 8 bit characters 
            inputSize = 8;
            checkCompRate++;
            break;
        case '2':       // compress 12 bit characters
            inputSize = 12;
            checkCompRate++;
            break;
        case '3':       // compress 16 bit characters
            inputSize = 16;
            checkCompRate++;
            break;
        case 'x':       // compress file
            inputFile = optarg;
            checkInput++;
            break;
        case 'z':       // decompress file
            fprintf( stderr, "Feature has not been implemented yet!");
            return EXIT_FAILURE;
        case 'c':       // encrypt output data
            // input password from the user
            char *inputPassword = optarg;

            // xor every char
            password = inputPassword[0];
            for(int i = 1; i < strlen(inputPassword); i++){
                password = password ^ inputPassword[i]; 
            }
            break;
        case 'v':       // give additional information
            fprintf( stderr, "Feature has not been implemented yet!");
            return EXIT_FAILURE;
        case 'h':       // print help
            fprintf( stderr, "Feature has not been implemented yet!");
            return EXIT_FAILURE;
        case 'o':       // output file
            outputFile = optarg;
            checkOutput++;
            break;
        case '?':
            if (optopt == 'x' || optopt == 'z' || optopt == 'c' || optopt == 'o')
                fprintf (stderr, "Options '-x', '-z', '-c', '-o', require arguments.\n");
            else 
                fprintf (stderr, "Unknown option `-%c'.\n", optopt);
            return EXIT_FAILURE;
        default:
            abort ();
    }
    int nonOption = 0;
    for (int i = optind; i < argc; i++){
       printf ("Non-option argument %s\n", argv[i]);
        nonOption++;
    }
    if(nonOption > 0){
        return 1;
    }    

    if( checkCompRate > 1 ) {
        fprintf (stderr, "You can chose only one compression rate! ('-1' or '-2' or '-3')!\n");
        return EXIT_FAILURE;
    }

    if( checkInput > 1 ) {
        fprintf (stderr, "You can chose only one input file!\n");
        return EXIT_FAILURE;
    }

    if( checkOutput > 1 ) {
        fprintf (stderr, "You can chose only one output file!\n");
        return EXIT_FAILURE;
    }

    in = fopen( inputFile, "r");
    if( in == NULL) {
        fprintf( stderr, "The input file \"%s\" doesn't exist!", inputFile);
        return EXIT_FAILURE;
    }

    if( checkCompRate == 0 || checkInput == 0 || checkOutput == 0 ) {
        fprintf (stderr, "Options ('-x' or '-z') , '-o', ('-1' or '-2' or '-3'), are required!\n");
        return EXIT_FAILURE;
    }
    
    out = fopen( outputFile, "wb+");     

    // number of unique chars
    int uniqueCounter = 0;
    // number of possible chars
    int possibleChars ;
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
        uniqueCounter = sixteenAnalyzeInput(in, charcounter, uniqueCounter);
    else if(inputSize == 12)
        uniqueCounter = twelveAnalyzeInput(in, charcounter, uniqueCounter);

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
        node *tmpNode = makeNode(arr+i, freq[i], false, NULL, NULL, NULL);
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

    // read codes from treeh
    readCodes(queue[queueSize-1], uniqueCounter, codes, tmp, 0);

    // print codes
    // for(int i = 0; i < uniqueCounter; i++){
    //     printf("c:%d i:%d   ", codes[i][0], i);
    //     for(int j = 0; j < codes[i][1]; j++){
    //         printf("%c", (char)codes[i][j+2]);
    //     }    
    //     printf("\n");
    // }

    

    int remaininglen = 0;
    char *remainingchar;
    node *dicpoint;
    dicpoint = queue[queueSize - 1];
    node *tmp2 = dicpoint;
    while ( tmp2->right != NULL) {
        tmp2 = tmp2->right;
    }
    char *bufordic = malloc( 16384 * sizeof *bufordic );
    if ( atof(argv[2]) == 1) {
        remaininglen = eightDictionary( dicpoint, NULL, queue[queueSize - 1], tmp2, out, 0, bufordic );
        if( remaininglen != 0) {
            remainingchar = malloc ( remaininglen * sizeof( remainingchar ));
            for ( int i= 0; i < remaininglen; i++) {
                remainingchar[i] = bufordic[i];
                printf("%c", remainingchar[i]);
            }
        }
    }
    else if ( atof(argv[2]) == 2) {
        remaininglen = twelveDictionary( dicpoint, NULL, queue[queueSize - 1], tmp2, out, 0, bufordic );
        if( remaininglen != 0) {
            remainingchar = malloc ( remaininglen * sizeof( remainingchar ));
            for ( int i= 0; i < remaininglen; i++) {
                remainingchar[i] = bufordic[i];
                printf("%c", remainingchar[i]);
            }
        }
    }
    else if ( atof(argv[2]) == 3) {
        remaininglen = sixteenDictionary( dicpoint, NULL, queue[queueSize - 1], tmp2, out, 0, bufordic );
        if( remaininglen != 0) {
            remainingchar = malloc ( remaininglen * sizeof( remainingchar ));
            for ( int i= 0; i < remaininglen; i++) {
                remainingchar[i] = bufordic[i];
                printf("%c", remainingchar[i]);
            }
        }
    }
    printf("\n");

    //rewind the input file
    rewind(in);

    // generate output
    if(inputSize == 8)
        eightOutputGenerator(in, uniqueCounter, codes, out, password);
    else if(inputSize == 16)
        sixteenOutputGenerator(in, uniqueCounter, codes, out, password);
    else if(inputSize == 12)
        twelveOutputGenerator(in, uniqueCounter, codes, out, password);

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
    free(charcounter);
    free(freq);
    free(arr);
    fclose(out);
    fclose(in);
    
    return 0;
}
