int binToDec(char *binary) {
    int power = 1;
    int output = 0;
    for(int i = 0; i < 8; i++) {
        output += (binary[7-i]-48) * power;
        power *= 2;
    }
    return output;
}