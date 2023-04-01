#include <stdlib.h>

int binToDec(char *binary) {
    int power = 1;
    int output = 0;
    for(int i = 0; i < 8; i++) {
        output += (binary[7-i]-48) * power;
        power *= 2;
    }
    return output;
}

char *DectoBin( unsigned short decimal, int bitnumber) {
    char *binary = malloc( bitnumber *sizeof(binary));
    if(decimal == 0){
        for(int i = 0; i < bitnumber; i++){
            binary[i] = 48;
        }
    }
    else{
        while ( decimal > 0)
            for( int i= bitnumber - 1; i >= 0; i--) {
                if (decimal > 0) {
                    binary[i] = (decimal%2 + 48);
                    decimal /= 2;
                } 
                else {
                    binary[i] = 48;
                } 
        }
    }
    return binary;
}