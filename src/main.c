#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include "./queue/node.h"
#include "./queue/queue.h"
#include "./compRate/eight.h"
#include "./compRate/twelve.h"
#include "./compRate/sixteen.h"
#include "./tools/convert.h"
#include <time.h>
#include <sys/types.h>
#include "./tools/dictionary.h"

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

    // for measuring time
    clock_t t;
    t = clock();
    // amount of bytes that we will be read as one values
    int inputSize = 0;
    // default password for data encrypting (doesn't change anything)
    char password = 0;
    // mandatory options
    int checkCompRate = 0;
    int checkInput = 0;
    int checkOutput = 0;
    // input and output files
    FILE *in;
    FILE *out;
    // string for file names and password
    char *outputFile = NULL;
    char *inputFile = NULL;
    char *inputPassword = NULL;  
    // for decompression
    int isDecomp = 0;
    //for verbose mode
    int isVerbose = 0;
    
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
            isVerbose++;
            break;
        case 'h':       // print help
            printf("%s", helpString);
            return 0;
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
    
    if(strlen(outputFile) == strlen(inputFile)){
        int check = 0;
        for(int i = 0; i < strlen(outputFile); i++){
            if(outputFile[i] == inputFile[i]){
                check++;
            }
        }

        if(check == strlen(outputFile)){
            fclose(in);
            fprintf (stderr, "Input and output files must have different names!\n");
            return EXIT_FAILURE;
        }
    }

    out = fopen( outputFile, "wb+");

    if( isDecomp > 0 ) {

        // file length in bytes
        unsigned long long int inputFileLength = 0;
        // bufor
        unsigned char *inputFileBufor = malloc(2000 * sizeof * inputFileBufor);
        // bufor length
        int length = 0;
        while( length = fread(inputFileBufor, 1, 2000, in)){
            inputFileLength += length;
        }
        
        free(inputFileBufor);
        
        rewind(in);

        // which byte in input file we currently analyze
        unsigned long long int byteInInputFile = 0;

        // get info from header
        unsigned char info[1];
        fread(info, 1, 1, in);
        byteInInputFile++;

        // compression rate of compressed file 
        int inputSize;
        
        // convert info to binary string
        char *header;
        header = DectoBin((unsigned short)info[0], 8);

        // make sure the file has not been damaged
        if(header[0] != '1' || header[1] != '1' || header[4] != '1'){
            printf("The file has been damaged!\n");
            fclose(in);
            fclose(out);
            free(header);
            return 1;
        }

        if( header[2] == '0' && header[3] == '1') {
            inputSize = 8;
        }
        else if( header[2] == '1' && header[3] == '0') {
            inputSize = 12;
        }
        else if( header[2] == '1' && header[3] == '1') {
            inputSize = 16;
        }
        else{
            printf("The file has been damaged!\n");
            fclose(in);
            fclose(out);
            free(header);
            return 1;
        }

        // how many 'empty' zeros were added to the file
        int addedZeros = 0;
        int power = 4;
        for(int i = 0; i < 3; i++){
            addedZeros += (header[i+5]-48) * power;
            power /= 2;
        }
        
        // more info
        fread(info, 1, 1, in);
        byteInInputFile++;

        // get crc from info
        unsigned char crc;        
        crc = info[0];
        
        // dictionary length in bits
        unsigned char dictLengthBin[4];
        unsigned short value;
        unsigned short codeLength = 0;
        fread( dictLengthBin, 1, 4, in);
        byteInInputFile += 4;

        int dictLength = 0;
        power = 1;
        for(int i = 3; i >= 0; i--){
            dictLength += dictLengthBin[i] * power;
            power *= 256;
        }

        // unumber of unique values
        unsigned char uniqueCounterBin[4];
        fread( uniqueCounterBin, 1, 4, in);
        byteInInputFile += 4;
        int uniqueCounter = 0;
        power = 1;
        for(int i = 3; i >= 0; i--){
            uniqueCounter += uniqueCounterBin[i] * power;
            power *= 256;
        }

        unsigned char crcCheck[1];
        while(fread(crcCheck, 1, 1, in)){
            crcCheck[0] = crcCheck[0] ^ password;
            crc = crc ^ crcCheck[0];
        }
        if( crc != 'J' ) {
            printf("The file has been damaged!\n");
            fclose(out);
            fclose(in);
            free(header);
            return 2;
        }

        unsigned char moveBytes[10];

        rewind(in);
        fread(moveBytes, 1, 10, in);

        // codes
        unsigned short **codes;
        codes = malloc(uniqueCounter * sizeof *codes);
        for(int i = 0; i < uniqueCounter; i++)
            codes[i] = NULL;

        unsigned char bufor[1];
        char *binaryBufor = malloc( 2048 * sizeof * binaryBufor);
        int binaryBuforLength = 0;
        char *tmpBinary;
        int index;
        while( dictLength > 0 ) {
            // read byte
            fread( bufor, 1, 1, in );
            byteInInputFile++;
            bufor[0] = bufor[0] ^ password;
            // crc = crc ^ bufor[0];
            dictLength -= 8;
            
            // convert to 8 bit
            tmpBinary = DectoBin((unsigned short)bufor[0], 8);

            // omit added zeros if last byte
            if(byteInInputFile != inputFileLength){
                for(int j = 0; j < 8; j++){
                    binaryBufor[binaryBuforLength + j] = tmpBinary[j];
                }
                binaryBuforLength += 8;
            }
            else {
                for(int j = 0; j < 8 - addedZeros; j++){
                    binaryBufor[binaryBuforLength + j] = tmpBinary[j];
                }
                binaryBuforLength += (8 - addedZeros);
            }
            free(tmpBinary);

            // get value and code length
            if(binaryBuforLength > (inputSize + 8)){
                // value
                int power = 1;
                value = 0;
                for(int j = 0; j < inputSize; j++) {
                    value += (binaryBufor[inputSize - 1 - j]-48) * power; 
                    power *= 2;
                }

                // code length
                power = 1;
                codeLength = 0;
                for(int j = 0; j < 8; j++) {
                    codeLength += (binaryBufor[inputSize + 8 - 1 - j]-48) * power;
                    power *= 2;
                }
                
                // put value and code length in codes
                for(int j = 0; j < uniqueCounter; j++){
                    if(codes[j] == NULL){
                        codes[j] = malloc((codeLength+2) * sizeof codes);
                        codes[j][0] = value;
                        codes[j][1] = codeLength;
                        index = j;
                        break;
                    }
                }

                // move data in bufor
                for (int j = 0; j < binaryBuforLength - (inputSize + 8); j++){
                    binaryBufor[j] = binaryBufor[j + inputSize + 8];
                }
                binaryBuforLength -= inputSize;
                binaryBuforLength -= 8;

                // get value's code if enough bits in bufor
                if(binaryBuforLength > codeLength) {
                    // put in codes
                    for(int j = 0; j < codeLength; j++){
                        codes[index][j+2] = binaryBufor[j];
                    }
                    // move data in bufor
                    for (int j = 0; j < binaryBuforLength - codeLength; j++){
                        binaryBufor[j] = binaryBufor[j + codeLength];
                    }
                    binaryBuforLength -= codeLength;
                }
                else {
                    // if not enough bits in bufor, get more bits 
                    while(codeLength > binaryBuforLength) {
                        fread( bufor, 1, 1, in );
                        byteInInputFile++;
                        bufor[0] = bufor[0] ^ password;
                        // crc = crc ^ bufor[0];
                        dictLength -= 8;
                        tmpBinary = DectoBin((unsigned short)bufor[0], 8);
                         // omit added zeros if last byte
                        if(byteInInputFile != inputFileLength){
                            for(int j = 0; j < 8; j++){
                                binaryBufor[binaryBuforLength + j] = tmpBinary[j];
                            }
                            binaryBuforLength += 8;
                        }
                        else {
                            for(int j = 0; j < 8 - addedZeros; j++){
                                binaryBufor[binaryBuforLength + j] = tmpBinary[j];
                            }
                            binaryBuforLength += (8 - addedZeros);
                        }
                        free(tmpBinary);
                    }
                    // put in codes
                    for(int j = 0; j < codeLength; j++){
                        codes[index][2+j] = binaryBufor[j];
                    }
                    // move data in bufor
                    for (int j = 0; j < binaryBuforLength - codeLength; j++){
                        binaryBufor[j] = binaryBufor[j + codeLength];
                    }
                    binaryBuforLength -= codeLength;
                }
            }
        }
        
        if(isVerbose){
            printf("Number of unique values: %d\n", uniqueCounter);
            for(int i = 0; i < uniqueCounter; i++){
                printf("value: %d   code: ", codes[i][0]);
                for(int j = 0; j < codes[i][1]; j++){
                    printf("%c", (char)codes[i][j+2]);
                }    
                printf("\n");
            }
        }

        // char read from the file
        unsigned char c[1];
        // binary representation of charcter   
        char *characterBinary;
        // length of read data
        int readLen;
        // how many chars in decompressed file
        int allChars = 0;
        // array for chars put in decompressed file
        unsigned char outputChar[3];
        // for 12 bit compression rate
        int outputCharLength = 0;
        // to make sure we enter "while" loop at least once
        int checkOne = 1;

        // if we read something and it's not the end of the file or first iteration
        while ( (readLen = fread( c, 1, 1, in ) && !feof(out) )|| checkOne) {
            checkOne = 0;

            // if we read new data
            if(readLen != 0){
                byteInInputFile++;
                c[0] = c[0] ^ password;
                crc = crc ^ c[0];
                characterBinary = DectoBin( (unsigned short)(c[0]), 8);
            
                // omit added zeros if last byte
                if(byteInInputFile != inputFileLength) {
                    for(int i = 0 ; i < 8; i++){
                        binaryBufor[binaryBuforLength + i] = characterBinary[i];
                    }
                    binaryBuforLength += 8;
                }
                else {
                    for(int i = 0 ; i < 8 - addedZeros; i++){
                        binaryBufor[binaryBuforLength + i] = characterBinary[i];
                    }
                    binaryBuforLength += (8 - addedZeros);
                }
                free(characterBinary);
            }

            // for checking codes; represents length of currently processed binary string
            int index = 1;
            while( index <= binaryBuforLength ) {
               
                // check for every code in codes
                for(int i = 0; i < uniqueCounter; i++){
                    
                    // if lengths are the same
                    if(codes[i][1] == index){
                        
                        // variable for checking if bits in code are right
                        int check = 0;
                        for(int j = 0; j < index; j++){
                            if(codes[i][2+j] == binaryBufor[j]){
                                check++;
                            }
                        }

                        // if all bits are the same in codes
                        if(check == index){

                            // 8 bit compression rate
                            if(inputSize == 8){
                                outputChar[0] = (unsigned char)codes[i][0];
                                fwrite( outputChar, 1, 1, out );
                                allChars++;
                            } // 12 bit compression rate
                            else if(inputSize == 12){
                                // 1 + 0,5 bytes
                                unsigned short mask1a = 2048 + 1024 + 512 + 256 + 128 + 64 + 32 + 16;
                                unsigned short mask2a = 8 + 4 + 2 + 1;

                                // 0,5 + 1 bytes
                                unsigned short mask1b = 2048 + 1024 + 512 + 256;
                                unsigned short mask2b = 128 + 64 + 32 + 16 + 8 + 4 + 2 + 1;
                                
                                // 1 + 0,5 bytes
                                if(outputCharLength == 0) {
                                    outputChar[0] = (unsigned char)((codes[i][0] & mask1a) / 16);
                                    outputChar[1] = (unsigned char)((codes[i][0] & mask2a) * 16);
                                    outputCharLength++;
                                } // 0,5 + 1 bytes
                                else if(outputCharLength == 1) {
                                    outputChar[1] += (unsigned char)((codes[i][0] & mask1b) / 256);
                                    outputChar[2] = (unsigned char)((codes[i][0] & mask2b));
                                    outputCharLength++;
                                } // if we got 3 chars
                                if(outputCharLength == 2){
                                    fwrite( outputChar, 1, 3, out );
                                    allChars += 3;
                                    outputCharLength = 0;
                                }

                            } // 16 bit compression rate
                            else{
                                unsigned short mask1 = 32768 + 16384 + 8192 + 4096 + 2048 + 1024 + 512 + 256;
                                unsigned short mask2 = 128 + 64 + 32 + 16 + 8 + 4 + 2 + 1;
                                outputChar[0] = (unsigned char)((codes[i][0] & mask1) / 256);
                                fwrite( outputChar, 1, 1, out );

                                outputChar[0] = (unsigned char)(codes[i][0] & mask2);
                                fwrite( outputChar, 1, 1, out );
                                allChars+=2;
                            }
                            
                            // move codes in bufor
                            for (int h = 0; h < binaryBuforLength - index; h++) {
                                binaryBufor[h] = binaryBufor[h + index];
                            }
                            binaryBuforLength -= index;
                            index = 0;
                        }
                    }
                }
                // if code not found, increase index length
                index++;
            }
        }

        // to make sure we don't lose any data from 12 bit compression
        if(outputCharLength == 1){
            fwrite( outputChar, 1, 2, out );
            allChars += 2;
            outputCharLength = 0;
        }
    
        // if( crc != 'J' ) {
        //     printf("The file has been damaged!\n");
        //     fclose(out);
        //     fclose(in);
            
        //     for(int i = 0; i < uniqueCounter; i++){
        //         free(codes[i]);
        //     }
        //     free(codes);
        //     free(header);
        //     free(binaryBufor);
            
        //     return 2;
        // }

        rewind(out);
        
        // delete trailing NULLs that are put in decompressed file 
        // if number of chars in initially compressed file was odd for 16 bit compression
        // if number of chars in initially compressed file was not divisible by 3 for 12 bit compression
        unsigned char *nullBufor = malloc(2000 * sizeof * nullBufor);
        int nullCounter[2] = {0, 0};
        int len;
        while(len = fread(nullBufor, 1, 2000, out)){

            // second to last char
            if(nullBufor[len - 2] == 0)
                nullCounter[0] = 1;
            else
                nullCounter[0] = 0;

            // last char
            if(nullBufor[len - 1] == 0)
                nullCounter[1] = 1;
            else
                nullCounter[1] = 0;
        }
        
        fclose(out);
        fclose(in);

        // truncate file to appropriate size
        if(nullCounter[1] == 1 && nullCounter[0] == 1){
            truncate(outputFile, allChars - 2);
            allChars -= 2;
        }
        else if(nullCounter[1] == 1 ){                
            truncate(outputFile, allChars - 1);
            allChars--;
        }

        // free allocated memory
        for(int i = 0; i < uniqueCounter; i++){
            free(codes[i]);
        }
        free(codes);
        free(header);
        free(binaryBufor);
        free(nullBufor);

        // calculated the time
        t = clock() - t;
        double timeTaken = ((double)t)/CLOCKS_PER_SEC;
        printf("Decompression complete in %fs\n", timeTaken);
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

        if(uniqueCounter == 0){
            fprintf(stderr, "You are trying to compress an empty file!\n");
            free(charcounter);
            return 3;
        }
        // if there is one char in the file
        else if( uniqueCounter == 1 ) {
            uniqueCounter++;
        }
        // characters found in the input
        unsigned short *arr = malloc( uniqueCounter * sizeof *arr);
        // frequency of charcters from the input
        int *freq = malloc( uniqueCounter * sizeof *freq);

        int arrayIndex = 0;
        // convert data from charcounter to arr and freq
        for (int i = 0; i < possibleChars; i++) {
            if( charcounter[i] != 0) {
                arr[arrayIndex] = i;
                freq[arrayIndex] = charcounter[i];
                arrayIndex++;
            }
        }

        // if there is one char in the file
        if( arrayIndex == 1 ) {
            if( arr[0] != 74 ) {
                arr[1] = 74;
                freq[1] = 1;
            }
            else {
                arr[1] = 106;
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
            node *tmpNode = makeNode(arr+i, freq[i], false, NULL, NULL);
            if(tmpNode == NULL){
                return 1;
            }
            queue[i] = tmpNode;
        }
        
        // make a binary tree
        int temp = 0;
        while(notFull(queue, queueSize)){
            addNewNodeToQueue(queue, queueSize);
            temp++;
        }

        // temporary array for reading codes
        unsigned short tmp[uniqueCounter - 1];

        // read codes from tree
        readCodes(queue[queueSize-1], uniqueCounter, codes, tmp, 0);

        if(isVerbose){
            printf("Number of unique values: %d\n", uniqueCounter);
            for(int i = 0; i < uniqueCounter; i++){
                printf("value: %d   code: ", codes[i][0]);
                for(int j = 0; j < codes[i][1]; j++){
                    printf("%c", (char)codes[i][j+2]);
                }    
                printf("\n");
            }
        }

        // place for header
        char character[1];
        character[0] = '0';
        fwrite( character, 1, 1, out );
        fwrite( character, 1, 1, out );

        char *remainingChar = malloc(2048 * sizeof * remainingChar);
        int remainingLength;

        unsigned char *crc = malloc( 1 * sizeof * crc );
        crc[0] = 'J';
        // generate dictionary
        remainingLength = dictionary(codes, out, uniqueCounter, inputSize, remainingChar, crc, password);

        rewind(in);

        // generate output
        if(inputSize == 8)
            eightOutputGenerator(in, uniqueCounter, codes, out, password, remainingChar, remainingLength, crc);
        else if(inputSize == 16)
            sixteenOutputGenerator(in, uniqueCounter, codes, out, password, remainingChar, remainingLength, crc);
        else if(inputSize == 12)
            twelveOutputGenerator(in, uniqueCounter, codes, out, password, remainingChar, remainingLength, crc);

        // free allocated memory
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
        free(remainingChar);
        free(crc);        
        fclose(out);
        fclose(in);

        // calculate the time
        t = clock() - t;
        double timeTaken = ((double)t)/CLOCKS_PER_SEC;
        printf("Compression complete in %fs\n", timeTaken);
    
    }

    return 0;
}
