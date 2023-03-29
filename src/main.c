#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include "./queue/node.h"
#include "./queue/sort.h"
#include "./queue/queue.h"
#include "./compRate/eight.h"
#include "./compRate/twelve.h"
#include "./compRate/sixteen.h"
#include "./tools/convert.h"
#include <time.h>
#include <sys/types.h>

char *helpString = 
"To compress\n"
"Usage: compressor -x input-file -o output-file -compress-rate [-c password -v -h]\n"
"   -x input-file\n"
"   -o output-file\n"
"   -compress-rate  Available compression rates: -1 -2 -3\n"
"   -c password \n"
"   -v              verbose\n"
"   -h              print usage\n"
"To decompress\n"
"Usage: compressor -z input-file -o output-file -compress-rate [-c [password] -v -h]\n"
"   -z input-file\n"
"   -o output-file\n"
"   -compress-rate  Available compression rates: -1 -2 -3\n"
"   -c password \n"
"   -v              verbose\n"
"   -h              print usage\n";

int main( int argc, char **argv) {

    clock_t t;
    t = clock();

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
    char *inputPassword = NULL;
    
    int isDecomp = 0;
    
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
            inputFile = optarg;
            isDecomp++;
            checkInput++;
            break;
        case 'c':       // encrypt output data
            // input password from the user
            inputPassword = optarg;

            // xor every char
            password = inputPassword[0];
            for(int i = 1; i < strlen(inputPassword); i++){
                password = password ^ inputPassword[i]; 
            }
            break;
        case 'v':       // give additional information
            fprintf( stderr, "Feature has not been implemented yet!\n");
            return EXIT_FAILURE;
        case 'h':       // print help
            printf("%s", helpString);
            break;
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

    in = fopen( inputFile, "rb");
    if( in == NULL) {
        fprintf( stderr, "The input file \"%s\" doesn't exist!\n", inputFile);
        return EXIT_FAILURE;
    }

    if(isDecomp == 0){
        if( checkCompRate == 0 || checkInput == 0 || checkOutput == 0 ) {
            fprintf (stderr, "Options '-x' , '-o', ('-1' or '-2' or '-3'), are required!\n");
            return EXIT_FAILURE;
        }
    }
    else {
        if( checkInput == 0 || checkOutput == 0 ) {
            fprintf (stderr, "Options '-z' and '-o' are required!\n");
            return EXIT_FAILURE;
        }
    }
    
    out = fopen( outputFile, "wb+");

    if( isDecomp > 0 ) {
        unsigned char info[1];
        int infoLen = 0;
        infoLen = fread(info, 1, 1, in);

        char *header;
        header = DectoBin((short)info[0], 8);

        if(header[0] != '1' || header[1] != '1'){
            printf("The file has been damaged!\n");
            fclose(in);
            fclose(out);
            return 1;
        }

        if( header[2] == '0' && header[3] == '1') {
            printf("compression rate: 8\n");
        }
        else if( header[2] == '1' && header[3] == '0') {
            printf("compression rate: 12\n");
            fclose(in);
            fclose(out);
            return 1;
        }
        else if( header[2] == '1' && header[3] == '1') {
            printf("compression rate: 16\n");
            fclose(in);
            fclose(out);
            return 1;
        }
        else{
            printf("The file has been damaged!\n");
            fclose(in);
            fclose(out);
            return 1;
        }

        int addedZeros = 0;
        int power = 1;
        for(int i = 0; i < 3; i++){
            addedZeros += (header[i+5]-48) * power;
            power *= 2;
        }
        printf("%d\n", addedZeros);
        
        infoLen = fread(info, 1, 1, in);

        unsigned char crc;        
        crc = info[0];


        //==============================================================

        char *bufor = malloc( 16384 * sizeof *bufor);
        unsigned short *characterRead = malloc(257 * sizeof *characterRead);
        int uniqueCounter[1];
        uniqueCounter[0] = 0;

        node *root = readDicEight(in, bufor, 0, NULL, characterRead, 0, uniqueCounter);
        printf("uni1: %d\n", uniqueCounter[0]);

        uniqueCounter[0] = ( uniqueCounter[0] / 2 ) + 1;
        printf("uni2: %d\n", uniqueCounter[0]);

        // temporary array for reading codes
        unsigned short tmp[uniqueCounter[0] - 1];


        // output codes array 
        // first row character
        // second row code length
        // next code
        unsigned short **codes;
        codes = malloc(uniqueCounter[0] * sizeof *codes);
        for(int i = 0; i < uniqueCounter[0]; i++)
            codes[i] = NULL;

        // read codes from treeh
        readCodes(root, uniqueCounter[0], codes, tmp, 0);
        

        // print codes
        for(int i = 0; i < uniqueCounter[0]; i++){
            printf("c:%c i:%d   ", codes[i][0], i);
            for(int j = 0; j < codes[i][1]; j++){
                printf("%c", (char)codes[i][j+2]);
            }    
            printf("\n");
        }

        //==============================================================

        // char read from the file
        unsigned char *c = malloc(sizeof *c);
        // binary representation of charcter   
        unsigned char *characterBinary;
        // bufor with codes from characters found in the input
        unsigned char *data = malloc( 16384 * sizeof *data );
        // length of the bufor
        int dataLength = 0;
        // count how many zeros we add artficially
        int zeroCounter = 0;
        int readLen;
        unsigned char lastChar;
        int allChars = 0;

        // get chararcter
        while ( readLen = fread( c, 1, 1, in ) ) {
            c[0] = c[0] ^ password;
            
            lastChar = c[0];

            crc = crc ^ c[0];

            characterBinary = DectoBin( (unsigned short)(c[0]), 8);
            
            // printf("datalen: %d\n", dataLength);

            for(int i = 0 ; i < 8; i++){
                data[dataLength + i] = characterBinary[i];
            }
            dataLength += 8;
            // printf("datalen: %d\n", dataLength);
            
            // for(int i = 0 ; i < dataLength; i++){
            //     printf("%c", data[i]);
            // }
            // printf("\n");;

            int index = 1; 
            
            // if enough bits in bufor
            while( index <= dataLength ) {
                // printf("index: %d\n", index);
                for(int i = 0; i < uniqueCounter[0]; i++){
                    // printf("codeslen: %d\n", codes[i][1]);
                    if(codes[i][1] == index){
                        // printf("codeslen: %d index: %d\n", index, codes[i][1]);
                        int check = 0;
                        for(int j = 0; j < index; j++){
                            if(codes[i][2+j] == data[j]){
                                check++;
                            }
                        }
                        if(check == index){
                            // printf("found: %c\n", (unsigned char)codes[i][0]);
                            unsigned char outputChar[1];
                            outputChar[0] = (unsigned char)codes[i][0];
                            fwrite( outputChar, 1, 1, out );
                            allChars++;
                            
                            // move codes in bufor
                            for (int h = 0; h < dataLength - index; h++)
                            {
                                data[h] = data[h + index];
                            }
                            dataLength -= index;
                            index = 0;
                            // for(int n = 0; n < dataLength; n++){
                            //     printf("%c", data[n]);
                            // }
                            // printf("\n");
                        }
                    }
                }
                index++;
            }
            // printf("===================================================\n");
        }

        if( crc != 'J' ){
            printf("The file has been damaged!\n");
            fclose(in);
            fclose(out);
            return 1;
        }

        characterBinary = DectoBin( (unsigned short)(lastChar), 8);
        int index = 1;
        int removeChars = 0;
        int characterBinaryLen = addedZeros;
        // if enough bits in bufor
        while( index <= characterBinaryLen ) {
            // printf("index: %d\n", index);
            for(int i = 0; i < uniqueCounter[0]; i++){
                // printf("codeslen: %d\n", codes[i][1]);
                if(codes[i][1] == index){
                    // printf("codeslen: %d index: %d\n", index, codes[i][1]);
                    int check = 0;
                    for(int j = 0; j < index; j++){
                        if(codes[i][2+j] == characterBinary[8 - addedZeros + j]){
                            check++;
                        }
                    }
                    if(check == index){
                        // printf("found: %c\n", (unsigned char)codes[i][0]);
                        removeChars++;
                        // move codes in bufor
                        for (int h = 0; h < characterBinaryLen - index; h++)
                        {
                            characterBinary[h] = characterBinary[8 - characterBinaryLen + h];
                        }
                        characterBinaryLen -= index;
                        index = 0;
                        // for(int n = 0; n < dataLength; n++){
                        //     printf("%c", data[n]);
                        // }
                        // printf("\n");
                    }
                }
            }
            index++;
        }
       
        free(bufor);
        free(characterRead);
        for(int i = 0; i < uniqueCounter[0]; i++){
            free(codes[i]);
        }
        free(codes);
        free(c);
        free(characterBinary);
        free(data);
        fclose(in);
        fclose(out);
        int check = truncate(outputFile, allChars - removeChars);
    }
    else {    
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

        // if there is one char in the file
        if( uniqueCounter == 1 ) {
            uniqueCounter++;
        }
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

        // if there is one char in the file
        if( uniqueCounter == 1 ) {
            if( arr[0] != 'J' ) {
                arr[1] = 'J';
                freq[1] = 1;
            }
            else {
                arr[1] = 'j';
                freq[1] = 1;
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

        // printQueue(queue, uniqueCounter);

        printf("U: %d\n", uniqueCounter);
        
        // make a binary tree
        int temp = 0;
        while(notFull(queue, queueSize)){
            addNewNodeToQueue(queue, queueSize);
            quickSortQueue(queue, 2*(temp+1), uniqueCounter + temp);
            // printf("%d\n", temp);
            temp++;
        }

        // temporary array for reading codes
        unsigned short tmp[uniqueCounter - 1];

        // read codes from treeh
        readCodes(queue[queueSize-1], uniqueCounter, codes, tmp, 0);

        // print codes
        for(int i = 0; i < uniqueCounter; i++){
            printf("c:%c i:%d   ", codes[i][0], i);
            for(int j = 0; j < codes[i][1]; j++){
                printf("%c", (char)codes[i][j+2]);
            }    
            printf("\n");
        }

        // place for header
        char character[1];
        character[0] = '0';
        fwrite( character, 1, 1, out );
        fwrite( character, 1, 1, out );

        int remaininglen = 0;
        char *remainingchar;
        node *dicpoint;
        dicpoint = queue[queueSize - 1];
        node *tmp2 = dicpoint;
        while ( tmp2->right != NULL) {
            tmp2 = tmp2->right;
        }
        char *bufordic = malloc( 16384 * sizeof *bufordic );
        if ( inputSize == 8) {
            remaininglen = eightDictionary( dicpoint, NULL, dicpoint, tmp2, out, 0, bufordic );
            if( remaininglen != 0) {
                remainingchar = malloc ( remaininglen * sizeof( remainingchar ));
                for ( int i= 0; i < remaininglen; i++) {
                    remainingchar[i] = bufordic[i];
                    // printf("%c", remainingchar[i]);
                }
            }
        }
        else if ( inputSize == 12) {
            remaininglen = twelveDictionary( dicpoint, NULL, dicpoint, tmp2, out, 0, bufordic );
            if( remaininglen != 0) {
                remainingchar = malloc ( remaininglen * sizeof( remainingchar ));
                for ( int i= 0; i < remaininglen; i++) {
                    remainingchar[i] = bufordic[i];
                    // printf("%c", remainingchar[i]);
                }
            }
        }
        else if ( inputSize == 16) {
            remaininglen = sixteenDictionary( dicpoint, NULL, dicpoint, tmp2, out, 0, bufordic );
            if( remaininglen != 0) {
                remainingchar = malloc ( remaininglen * sizeof( remainingchar ));
                for ( int i= 0; i < remaininglen; i++) {
                    remainingchar[i] = bufordic[i];
                    // printf("%c", remainingchar[i]);
                }
            }
        }
        // printf("\n");

        //rewind the input file
        rewind(in);

        // generate output
        if(inputSize == 8)
            eightOutputGenerator(in, uniqueCounter, codes, out, password, remainingchar, remaininglen);
        else if(inputSize == 16)
            sixteenOutputGenerator(in, uniqueCounter, codes, out, password, remainingchar, remaininglen);
        else if(inputSize == 12)
            twelveOutputGenerator(in, uniqueCounter, codes, out, password, remainingchar, remaininglen);

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
        free(bufordic);

        if(remaininglen != 0)
            free(remainingchar);

        fclose(out);
        fclose(in);

        // check message
        printf("everything went well!\n");

        t = clock() - t;

        double timeTaken = ((double)t)/CLOCKS_PER_SEC;

        printf("time: %f\n", timeTaken);
    
    }

    return 0;
}
