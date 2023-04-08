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

        unsigned long long int inputFileLength = 0;
        unsigned char *inputFileBufor = malloc(2000 * sizeof * inputFileBufor);
        int length = 0;
        while( length = fread(inputFileBufor, 1, 2000, in)){
            inputFileLength += length;
        }
        free(inputFileBufor);
        rewind(in);

        unsigned long long int byteInInputFile = 0;

        // --------------------------------------------------header info
        unsigned char info[1];
        int infoLen = 0;
        infoLen = fread(info, 1, 1, in);
        byteInInputFile++;
        int inputSize;
        char *header;
        header = DectoBin((unsigned short)info[0], 8);
        int remainder;

        if(header[0] == '1' && header[1] == '1'){
            printf("The file has been damaged!\n");
            fclose(in);
            fclose(out);
            return 1;
        }
        else if(header[0] == '0' && header[1] == '1'){
            remainder = 2; 
        }
        else if(header[0] == '1' && header[1] == '0'){
            remainder = 1; 
        }
        else if(header[0] == '0' && header[1] == '0'){
            remainder = 0; 
        }

        if( header[2] == '0' && header[3] == '1') {
            // printf("compression rate: 8\n");
            inputSize = 8;
        }
        else if( header[2] == '1' && header[3] == '0') {
            // printf("compression rate: 12\n");
            inputSize = 12;
        }
        else if( header[2] == '1' && header[3] == '1') {
            // printf("compression rate: 16\n");
            inputSize = 16;
        }
        else{
            printf("The file has been damaged!\n");
            fclose(in);
            fclose(out);
            return 1;
        }

        int isOdd = 0;
        if(header[4] == '1'){
            isOdd = 1;
        }
        // printf("isOdd: %d\n", isOdd);

        int addedZeros = 0;
        int power = 4;
        for(int i = 0; i < 3; i++){
            addedZeros += (header[i+5]-48) * power;
            power /= 2;
        }
        // printf("added zeros: %d\n", addedZeros);
        
        infoLen = fread(info, 1, 1, in);
        byteInInputFile++;

        unsigned char crc;        
        crc = info[0];

        //----------------------------------------- dictionary
        
        // dictionary length
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
        // printf("dL: %d\n", dictLength);

        // unique counter
        unsigned char uniqueCounterBin[4];
        fread( uniqueCounterBin, 1, 4, in);
        byteInInputFile += 4;

        int uniqueCounter = 0;
        power = 1;
        for(int i = 3; i >= 0; i--){
            uniqueCounter += uniqueCounterBin[i] * power;
            power *= 256;
        }
        // printf("uni: %d\n", uniqueCounter);


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
            crc = crc ^ bufor[0];
            dictLength -= 8;
            

            // convert to 8 bit
            tmpBinary = DectoBin((unsigned short)bufor[0], 8);
            // printf("tmp dec: %d\n", (short)bufor[0]);
            // printf("tmp: ");
            // for(int h = 0; h < 8; h++){
            //     printf("%c", tmpBinary[h]);
            // }
            // printf("\n");
            if(byteInInputFile != inputFileLength){
                for(int j = 0; j < 8; j++){
                    // printf("check1a\n");
                    binaryBufor[binaryBuforLength + j] = tmpBinary[j];
                }
                binaryBuforLength += 8;
            }
            else {
                for(int j = 0; j < 8 - addedZeros; j++){
                    // printf("check1b\n");
                    binaryBufor[binaryBuforLength + j] = tmpBinary[j];
                }
                binaryBuforLength += (8 - addedZeros);
            }
            // printf("bufor[%d] vl: ", binaryBuforLength);
            // for(int h = 0; h < binaryBuforLength; h++){
            //     printf("%c", binaryBufor[h]);
            // }
            // printf("\n");
            free(tmpBinary);

            // get value and code length
            if(binaryBuforLength > (inputSize + 8)){
                // printf("bufLen: %d  val+cLen: %d\n", binaryBuforLength, inputSize+8);
                // value
                int power = 1;
                value = 0;
                for(int j = 0; j < inputSize; j++) {
                    value += (binaryBufor[inputSize - 1 - j]-48) * power; 
                    power *= 2;
                }
                
                // printf("val: %c\n", value);

                // code length
                power = 1;
                codeLength = 0;

                for(int j = 0; j < 8; j++) {
                    codeLength += (binaryBufor[inputSize + 8 - 1 - j]-48) * power;
                    power *= 2;
                }
                
                // printf("codeL: %d\n", codeLength);
                for(int j = 0; j < uniqueCounter; j++){
                    if(codes[j] == NULL){
                        codes[j] = malloc((codeLength+2) * sizeof codes);
                        codes[j][0] = value;
                        codes[j][1] = codeLength;
                        index = j;
                        break;
                    }
                }
                for (int j = 0; j < binaryBuforLength - (inputSize + 8); j++){
                    binaryBufor[j] = binaryBufor[j + inputSize + 8];
                }
                binaryBuforLength -= inputSize;
                binaryBuforLength -= 8;
                // code
                if(binaryBuforLength > codeLength) {
                    // printf("bufor c: ");
                    // for(int h = 0; h < binaryBuforLength; h++){
                    //     printf("%c", binaryBufor[h]);
                    // }
                    // printf("\n");
                    for(int j = 0; j < codeLength; j++){
                        codes[index][j+2] = binaryBufor[j];
                    }
                    
                    for (int j = 0; j < binaryBuforLength - codeLength; j++)
                    {
                        binaryBufor[j] = binaryBufor[j + codeLength];
                    }
                    binaryBuforLength -= codeLength;
                }
                else {
                    while(codeLength > binaryBuforLength) {
                        // read byte
                        fread( bufor, 1, 1, in );
                        byteInInputFile++;
                        bufor[0] = bufor[0] ^ password;
                        crc = crc ^ bufor[0];
                        dictLength-=8;
                        // convert to 8 bit
                        tmpBinary = DectoBin((unsigned short)bufor[0], 8);
                        if(byteInInputFile != inputFileLength){
                            for(int j = 0; j < 8; j++){
                                // printf("check2a\n");
                                binaryBufor[binaryBuforLength + j] = tmpBinary[j];
                            }
                            binaryBuforLength += 8;
                        }
                        else {
                            for(int j = 0; j < 8 - addedZeros; j++){
                                // printf("check2b\n");
                                binaryBufor[binaryBuforLength + j] = tmpBinary[j];
                            }
                            binaryBuforLength += (8 - addedZeros);
                        }
                        free(tmpBinary);
                    }
                    // printf("bufor[%d] cb: ", binaryBuforLength);
                    // for(int h = 0; h < binaryBuforLength; h++){
                    //     printf("%c", binaryBufor[h]);
                    // }
                    // printf("\n");
                    for(int j = 0; j < codeLength; j++){
                        codes[index][2+j] = binaryBufor[j];
                    }
                    for (int j = 0; j < binaryBuforLength - codeLength; j++)
                    {
                        binaryBufor[j] = binaryBufor[j + codeLength];
                    }
                    binaryBuforLength -= codeLength;
                    // printf("binLen after: %d\n", binaryBuforLength);
                }
                // printf("code: ");
                // for(int k = 0; k < codes[index][1]; k++){
                //     printf("%c ", codes[index][2+k]);
                // }
                // printf("\n");
            }
        }
        

        // print codes
        // for(int i = 0; i < uniqueCounter; i++){
        //     printf("c:%d i:%d   ", codes[i][0], i);
        //     for(int j = 0; j < codes[i][1]; j++){
        //         printf("%c", (char)codes[i][j+2]);
        //     }    
        //     printf("\n");
        // }

        //------------------------------------------------ convert 

        // char read from the file
        unsigned char c[1];
        // binary representation of charcter   
        char *characterBinary;
        // count how many zeros we add artficially
        int zeroCounter = 0;
        int readLen;
        unsigned char lastChar;
        int allChars = 0;
        // char *outputBufor = malloc()
        unsigned char outputChar[3];
        int outputCharLength = 0;
        // get chararcter
        
        // for(int i = 0; i < binaryBuforLength; i++){
        //     printf("%c", binaryBufor[i]);
        // }
        // printf("\n");

        int checkOne = 1;


        while ( (readLen = fread( c, 1, 1, in ) && !feof(out) )|| checkOne) {
            checkOne = 0;
            // printf("readlen: %d\n", readLen);
            // printf("c: %d\n", c[0]);
            if(readLen != 0){
                byteInInputFile++;

                c[0] = c[0] ^ password;
                
                lastChar = c[0];

                crc = crc ^ c[0];

                characterBinary = DectoBin( (unsigned short)(c[0]), 8);
                
                // printf("datalen: %d\n", dataLength);
                if(byteInInputFile != inputFileLength) {
                    for(int i = 0 ; i < 8; i++){
                        // printf("check3a\n");
                        binaryBufor[binaryBuforLength + i] = characterBinary[i];
                    }
                    binaryBuforLength += 8;
                }
                else {
                    for(int i = 0 ; i < 8 - addedZeros; i++){
                        // printf("check3b\n");
                        binaryBufor[binaryBuforLength + i] = characterBinary[i];
                    }
                    binaryBuforLength += (8 - addedZeros);
                }
                free(characterBinary);
            }
            // printf("datalen: %d\n", dataLength);
            
            // for(int i = 0 ; i < dataLength; i++){
            //     printf("%c", data[i]);
            // }
            // printf("\n");;

            int index = 1;

            // printf("binarylen: %d\n", binaryBuforLength);
            
            // if enough bits in bufor
            while( index <= binaryBuforLength ) {
                // printf("index: %d\n", index);
                for(int i = 0; i < uniqueCounter; i++){
                    // printf("codeslen: %d\n", codes[i][1]);
                    if(codes[i][1] == index){
                        // printf("codeslen: %d index: %d\n", index, codes[i][1]);
                        int check = 0;
                        for(int j = 0; j < index; j++){
                            if(codes[i][2+j] == binaryBufor[j]){
                                check++;
                            }
                        }
                        // printf("index: %d check: %d\n", index, check);
                        if(check == index){
                            // printf("found: %c\n", (unsigned char)codes[i][0]);
                            if(inputSize == 8){
                                // unsigned char outputChar[1];
                                outputChar[0] = (unsigned char)codes[i][0];
                                fwrite( outputChar, 1, 1, out );
                                allChars++;
                            }
                            else if(inputSize == 12){
                                unsigned short mask1a = 2048 + 1024 + 512 + 256 + 128 + 64 + 32 + 16;
                                unsigned short mask2a = 8 + 4 + 2 + 1;

                                unsigned short mask1b = 2048 + 1024 + 512 + 256;
                                unsigned short mask2b = 128 + 64 + 32 + 16 + 8 + 4 + 2 + 1;
                                
                                if(outputCharLength == 0) {
                                    outputChar[0] = (unsigned char)((codes[i][0] & mask1a) / 16);
                                    outputChar[1] = (unsigned char)((codes[i][0] & mask2a) * 16);
                                    // printf("o1 %d %d\n", outputChar[0], outputChar[1]);
                                    outputCharLength++;
                                }
                                else if(outputCharLength == 1) {
                                    outputChar[1] += (unsigned char)((codes[i][0] & mask1b) / 256);
                                    outputChar[2] = (unsigned char)((codes[i][0] & mask2b));
                                    // printf("o2 %d %d\n", outputChar[1], outputChar[2]);
                                    outputCharLength++;
                                }
                                if(outputCharLength == 2){
                                    fwrite( outputChar, 1, 3, out );
                                    // printf("out: %d %d %d\n", outputChar[0], outputChar[1], outputChar[2]);
                                    allChars += 3;
                                    outputCharLength = 0;
                                }

                            }
                            else{
                                // unsigned char outputChar[1];
                                unsigned short mask1 = 32768 + 16384 + 8192 + 4096 + 2048 + 1024 + 512 + 256;
                                unsigned short mask2 = 128 + 64 + 32 + 16 + 8 + 4 + 2 + 1;
                                outputChar[0] = (unsigned char)((codes[i][0] & mask1) / 256);
                                // printf("1: %d ", outputChar[0]);
                                fwrite( outputChar, 1, 1, out );

                                outputChar[0] = (unsigned char)(codes[i][0] & mask2);
                                // printf("2: %d\n", outputChar[0]);
                                fwrite( outputChar, 1, 1, out );
                                allChars+=2;
                            }
                            
                            // move codes in bufor
                            for (int h = 0; h < binaryBuforLength - index; h++)
                            {
                                binaryBufor[h] = binaryBufor[h + index];
                            }
                            binaryBuforLength -= index;
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

        if(outputCharLength == 1){
            fwrite( outputChar, 1, 2, out );
            // printf("out2: %d %d\n", outputChar[0], outputChar[1]);
            allChars += 2;
            outputCharLength = 0;
        }

        if( crc != 'J' ) {
            printf("The file has been damaged!\n");
            fclose(out);
            fclose(in);
            
            for(int i = 0; i < uniqueCounter; i++){
                free(codes[i]);
            }
            free(codes);
            free(header);
            free(binaryBufor);
            
            return 2;
        }

        rewind(out);
        unsigned char *nullBufor = malloc(2000 * sizeof * nullBufor);
        int nullCounter[2] = {0, 0};
        int len;
        while(len = fread(nullBufor, 1, 2000, out)){
            // for(int i = 0 ; i < len; i++)
            //     printf("%d ", nullBufor[i]);
            // printf("\n");

            if(nullBufor[len - 2] == 0)
                nullCounter[0] = 1;
            else
                nullCounter[0] = 0;

            if(nullBufor[len - 1] == 0)
                nullCounter[1] = 1;
            else
                nullCounter[1] = 0;
        }
        
        fclose(out);
        fclose(in);

        // printf("all chars: %d\n", allChars);

        if(nullCounter[1] == 1 && nullCounter[0] == 1){
            // printf("trun2\n");
            truncate(outputFile, allChars - 2);
            allChars -= 2;
        }
        else if(nullCounter[1] == 1 ){
            // printf("trun1\n");                 
            truncate(outputFile, allChars - 1);
            allChars--;
        }

        // // handling added zeros
        // characterBinary = DectoBin((unsigned short)(lastChar), 8);
        // index = 1;
        // int removeChars = 0;
        // // printf("added zeros: %d\n", addedZeros);
        // int characterBinaryLen = addedZeros;
        // int isOddIteration = 1;
        // // if enough bits in bufor
        // while( index <= characterBinaryLen ) {
        //     // printf("index: %d\n", index);
        //     for(int i = 0; i < uniqueCounter; i++){
        //         // printf("codeslen: %d\n", codes[i][1]);
        //         if(codes[i][1] == index){
        //             // printf("codeslen: %d index: %d\n", index, codes[i][1]);
        //             int check = 0;
        //             for(int j = 0; j < index; j++){
        //                 if(codes[i][2+j] == characterBinary[8 - addedZeros + j]){
        //                     check++;
        //                 }
        //             }
        //             if(check == index){
        //                 // printf("found: %c\n", (unsigned char)codes[i][0]);
        //                 if( inputSize == 8 ) {
        //                     removeChars++;
        //                 }
        //                 else if( inputSize == 12) {
        //                     removeChars += 1 + isOddIteration;
        //                     isOddIteration = isOddIteration == 1 ? 0 : 1;
        //                 }
        //                 else {
        //                     removeChars+=2;
        //                 }
        //                 // move codes in bufor
        //                 for (int h = 0; h < characterBinaryLen - index; h++)
        //                 {
        //                     characterBinary[h] = characterBinary[8 - characterBinaryLen + h];
        //                 }
        //                 characterBinaryLen -= index;
        //                 index = 0;
        //                 // for(int n = 0; n < dataLength; n++){
        //                 //     printf("%c", data[n]);
        //                 // }
        //                 // printf("\n");
        //             }
        //         }
        //     }
        //     index++;
        // }
       
        // // free(bufor);
        // // free(characterRead);
        // for(int i = 0; i < uniqueCounter; i++){
        //     free(codes[i]);
        // }
        // free(codes);
        // free(characterBinary);
        // free(header);
        // free(binaryBufor);

        
        // // printf("all: %d remove: %d\n", allChars, removeChars);
        // int check = truncate(outputFile, allChars - (removeChars));
        // allChars -= removeChars;

        // out = fopen(outputFile, "rb");
        // rewind(out);
        
        // while(len = fread(nullBufor, 1, 2000, out)){
        //     // for(int i = 0 ; i < len; i++)
        //     //     printf("%d ", nullBufor[i]);
        //     // printf("\n");

        //     if(nullBufor[len - 2] == 0)
        //         nullCounter[0] = 1;
        //     else
        //         nullCounter[0] = 0;

        //     // printf("%d\n", nullBufor[len-1]);
        //     if(nullBufor[len - 1] == 0)
        //         nullCounter[1] = 1;
        //     else
        //         nullCounter[1] = 0;
        // }
        // free(nullBufor);
        // fclose(out);
        //  if(nullCounter[1] == 1 && nullCounter[0] == 1){
        //     // printf("trun2\n");
        //     truncate(outputFile, allChars - 2);
        //     allChars -= 2;
        // }
        // else if(nullCounter[1] == 1 ){
        //     // printf("trun1: ");                 
        //     // printf("%d\n", truncate(outputFile, allChars - 1));
        //     allChars--;
        // }
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

        printf("Finished getting input!\n");

        // if there is one char in the file
        if( uniqueCounter == 1 ) {
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
            node *tmpNode = makeNode(arr+i, freq[i], false, NULL, NULL, NULL);
            if(tmpNode == NULL){
                return 1;
            }
            queue[i] = tmpNode;
        }
        
        // sort inputs
        // quickSortQueue(queue, 0, uniqueCounter - 1);

        // printQueue(queue, uniqueCounter);

        // printf("U: %d\n", uniqueCounter);
        
        // make a binary tree
        int temp = 0;
        while(notFull(queue, queueSize)){
            addNewNodeToQueue(queue, queueSize);
            // quickSortQueue(queue, 2*(temp+1), uniqueCounter + temp);
            // printf("%d\n", temp);
            temp++;
            if(temp % 1000 == 0)
                printf("Iteration: %d\n", temp);
        }

        // temporary array for reading codes
        unsigned short tmp[uniqueCounter - 1];

        // read codes from tree
        readCodes(queue[queueSize-1], uniqueCounter, codes, tmp, 0);

        printf("Finished analyzing input!\n");
        // print codes
        for(int i = 0; i < uniqueCounter; i++){
            printf("c:%d i:%d   ", codes[i][0], i);
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

        // int remaininglen = 0;
        // char *remainingchar;
        // node *dicpoint;
        // dicpoint = queue[queueSize - 1];
        // node *tmp2 = dicpoint;
        // while ( tmp2->right != NULL) {
        //     tmp2 = tmp2->right;
        // }
        // char *bufordic = malloc( 16384 * sizeof *bufordic );
        // if ( inputSize == 8) {
        //     remaininglen = eightDictionary( dicpoint, NULL, dicpoint, tmp2, out, 0, bufordic );
        //     if( remaininglen != 0) {
        //         remainingchar = malloc ( remaininglen * sizeof( remainingchar ));
        //         for ( int i= 0; i < remaininglen; i++) {
        //             remainingchar[i] = bufordic[i];
        //             // printf("%c", remainingchar[i]);
        //         }
        //     }
        // }
        // else if ( inputSize == 12) {
        //     remaininglen = twelveDictionary( dicpoint, NULL, dicpoint, tmp2, out, 0, bufordic );
        //     if( remaininglen != 0) {
        //         remainingchar = malloc ( remaininglen * sizeof( remainingchar ));
        //         for ( int i= 0; i < remaininglen; i++) {
        //             remainingchar[i] = bufordic[i];
        //             // printf("%c", remainingchar[i]);
        //         }
        //     }
        // }
        // else if ( inputSize == 16) {
        //     remaininglen = sixteenDictionary( dicpoint, NULL, dicpoint, tmp2, out, 0, bufordic );
        //     if( remaininglen != 0) {
        //         remainingchar = malloc ( remaininglen * sizeof( remainingchar ));
        //         for ( int i= 0; i < remaininglen; i++) {
        //             remainingchar[i] = bufordic[i];
        //             // printf("%c", remainingchar[i]);
        //         }
        //     }
        // }
        // printf("\n");

        char *remainingChar = malloc(2048 * sizeof * remainingChar);
        int remainingLength;

        unsigned char *crc = malloc( 1 * sizeof * crc );
        crc[0] = 'J';
        remainingLength = dictionary(codes, out, uniqueCounter, inputSize, remainingChar, crc, password);

        //rewind the input file
        rewind(in);

        // generate output
        if(inputSize == 8)
            eightOutputGenerator(in, uniqueCounter, codes, out, password, remainingChar, remainingLength, crc);
        else if(inputSize == 16)
            sixteenOutputGenerator(in, uniqueCounter, codes, out, password, remainingChar, remainingLength, crc);
        else if(inputSize == 12)
            twelveOutputGenerator(in, uniqueCounter, codes, out, password, remainingChar, remainingLength, crc);

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
        free(remainingChar);
        free(crc);        
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
