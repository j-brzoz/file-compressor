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

char *DectoBin( short decimal) {
    char *binary = malloc( 8 *sizeof(binary));
    while ( decimal > 0)
    for( int i= 7; i >= 0; i--) {
        if (decimal > 0) {
        binary[i] = (decimal%2 + 48);
        decimal /= 2;
        } else {
            binary[i] = 48;
        } 
    }
    return binary;
}