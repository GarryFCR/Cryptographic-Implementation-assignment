#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

// A and B are 127 bit numbers
#define CHAR_NUM_LEN 16 // 8x16 = 128 bits
#define INT_NUM_LEN 5 // 26x5 = 130 bits

// generates char* little endian
unsigned char *rand127();

// converts char* to int*
int *convertC2I(unsigned char *val, unsigned int charLength);
unsigned char *convertI2C(int *val, unsigned int charLength);

//addition of two 2^8 base number i.e A and B and result is a 2^26 base number stored in X
int *add(int *A, int *B);

int *mult(int *A, int *B);

// methods for printing numbers
void printCharNum(char label[], unsigned char *val, unsigned int len);
void printCharNumBinary(char label[], unsigned char *val, unsigned int len);
void printInts(char label[], int *limbs, unsigned int len);
void printIntsBinary(char label[], int *val, unsigned int len);



// MAIN FUNCTION

int main() {
    unsigned char *charA = rand127();
    unsigned char *charB = rand127();

    int *intA = convertC2I(charA, CHAR_NUM_LEN);
    int *intB = convertC2I(charB, CHAR_NUM_LEN);

    // uncomment to see A and B for debugging

    // printCharNum("A", charA, CHAR_NUM_LEN);
    // printInts("A", intA, INT_NUM_LEN);
    // printCharNum("B", charB, CHAR_NUM_LEN);
    // printInts("B", intB, INT_NUM_LEN);

    // printCharNumBinary("A ", charA, CHAR_NUM_LEN);
    // printIntsBinary("A", intA, INT_NUM_LEN);
    // printCharNumBinary("B ", charB, CHAR_NUM_LEN);
    // printIntsBinary("B", intB, INT_NUM_LEN);

    int *intC = add(intA, intB);
    // printInts("A+B", intC, INT_NUM_LEN * 2);
    int *intD = mult(intA, intB);
    // printInts("AxB", intD, INT_NUM_LEN * 2);

    unsigned char *charC = convertI2C(intC, CHAR_NUM_LEN);
    printCharNum("A+B", charC, CHAR_NUM_LEN);
    unsigned char *charD = convertI2C(intD, CHAR_NUM_LEN * 2);
    printCharNum("AxB", charD, CHAR_NUM_LEN * 2);


    free(charA);
    free(charB);
    free(charC);
    free(charD);
    free(intA);
    free(intB);
    free(intC);
    free(intD);
    return 0;
}

// FUNCTION IMPLEMENTATION STARTS HERE

unsigned char *rand127() {
    unsigned char *val = malloc(CHAR_NUM_LEN * sizeof(unsigned char));
    for (int i = 0; i < 16; i++) {
        if(i < 15) {
            val[i] = rand() % 256;
        } else {
            val[i] = rand() % 128;
        }
        // val[i] = 1; // for debug
    }
    return val;
}

int *convertC2I(unsigned char *val, unsigned int charLength) {
    int *result = malloc(INT_NUM_LEN * 2 * sizeof(int));

    int bitPosition = 0;
    if(charLength == 0) charLength = CHAR_NUM_LEN;
    for (int i = 0; i < charLength; i++) {
        if((bitPosition % 26) <= 18) { // 26 - 8
            // the char is completely inside one 2**26 limb
            result[bitPosition / 26] |= (val[i] << (bitPosition % 26));
        } else {
            // the char is partially inside one 2**26 limb
            int valAnd = (1 << (26 - (bitPosition % 26))) - 1;
            result[bitPosition / 26] |= (val[i] & valAnd) << (bitPosition % 26);
            result[bitPosition / 26 + 1] |= val[i] >> (26 - (bitPosition % 26));   
        }
        bitPosition += 8;
    }

    return result;
}

unsigned char *convertI2C(int *val, unsigned int charLength) {
    unsigned char *result = malloc(CHAR_NUM_LEN * 2 * sizeof(unsigned char));

    int bitPosition = 0;
    if(charLength == 0) charLength = CHAR_NUM_LEN;
    for (int i = 0; i < charLength; i++) {
        if((bitPosition % 26) <= 18) { // 26 - 8
            // the char is completely inside one 2**26 limb
            result[i] = (val[bitPosition / 26] >> (bitPosition % 26)) & 255;
        } else {
            // the char is partially inside one 2**26 limb
            int valAnd = (1 << (26 - (bitPosition % 26))) - 1;
            result[i] = ((val[bitPosition / 26] >> (bitPosition % 26)) & valAnd) | ((val[bitPosition / 26 + 1] << (26 - (bitPosition % 26))) & 255);
        }
        bitPosition += 8;
    }

    return result;
}

int *add(int *A, int *B) {
    int a, b, r, i = 0, j = 0, k = 0;
    int sum = 0;
    int C[17];
    C[16] = 0;

    for (i = 0; i < 16; i++) {
        C[i] = A[i] + B[i];
    }
    for (j = 0; j < 16; j++) {
        if (C[j] > 255) {
            a = C[j] / 256;
            b = C[j] % 256;

            C[j] = b;
            if (j != 15) {
                C[j + 1] = C[j + 1] + a;
            } else {
                C[j + 1] = a;
            }
        }
    }

    unsigned char s[17];
    for (int i = 0; i < 17; i++) {
        s[i] = C[i];
    }

    return convertC2I(s, CHAR_NUM_LEN);
}

// adds value to a int*, carries if overflow
void addVal(int *ptr, int val) {
    if(val >= 1 << 26) {
        printf("addVal: %d is too large\n", val);
        exit(1);
    }

    if((ptr[0] + val) < (1 << 26)) {
        // if not overflow, simply add
        ptr[0] = ptr[0] + val;
    } else {
        // handle overflow
        ptr[0] = (ptr[0] + val) % (1 << 26);
        ptr[1] = ptr[1] + 1; // add a carry
    }
}

// Karatsuba (https://brilliant.org/wiki/karatsuba-algorithm/)
void multiplyKaratsubaStep(int x, int y, int *result) {
    // splitting x and y into two halves
    int xH = (x >> 13) & 0x1fff; 
    int xL = x & 0x1fff;
    int yH = (y >> 13) & 0x1fff;
    int yL = y & 0x1fff;

    // xy = a*(b**n) + e*(b**n/2) + d
    int a = xH * yH;
    int d = xL * yL;
    int e = xH * yL + xL * yH; // (xH + xL) * (yH + yL) - a - d;

    // writing the result to pointer
    addVal(result + 1, a); // result[1] = result[1] + a;
    addVal(result + 1, e >> 13); // result[1] = result[1] + e >> 13;
    addVal(result, (e & 0x1fff) << 13); // result[0] = result[0] + (e & 0x3ffffff) << 13;
    addVal(result, d); // result[0] = result[0] + d;
}

int *mult(int *x, int *y) {
    int *result = malloc(INT_NUM_LEN * 2 * sizeof(int));
    for (int i = 0; i < INT_NUM_LEN; i++) {
        for (int j = 0; j < INT_NUM_LEN; j++) {
            // apply Karatsuba to all the pairs of limbs
            multiplyKaratsubaStep(x[i], y[j], result + (i + j));
        }
    }
    return result;
}

// PRINTING FUNCTIONS BELOW

// print int* as big endian
void printCharNum(char label[], unsigned char *val, unsigned int length) {
    if(length == 0) length = CHAR_NUM_LEN;
    printf("charNum %s ", label);
    // little endian
    for (int i = length - 1; i >= 0; i--) {
        printf("%d ", val[i]);
    }
    printf("\n");
}

// print int* as big endian
void printInts(char label[], int *limbs, unsigned int length) {
    if(length == 0) length = INT_NUM_LEN;
    printf("intNum %s ", label);
    for (int i = length - 1; i >= 0; i--) {
        printf("%d ", limbs[i] % (1 << 26));
    }
    printf("\n");
}

// prints char* as big endian in binary (useful for debugging)
void printCharNumBinary(char label[], unsigned char *val, unsigned int length) {
    if(length == 0) length = CHAR_NUM_LEN;
    printf("charNum %s ", label);
    // little endian
    for (int i = length - 1; i >= 0; i--) {
        printf(
            "%d%d%d%d%d%d%d%d", 
            val[i] >> 7 & 1,
            val[i] >> 6 & 1,
            val[i] >> 5 & 1,
            val[i] >> 4 & 1,
            val[i] >> 3 & 1,
            val[i] >> 2 & 1,
            val[i] >> 1 & 1,
            val[i] & 1
        );
    }
    printf("\n");
}

// prints char* as big endian in binary
void printIntsBinary(char label[], int *val, unsigned int length) {
    if(length == 0) length = INT_NUM_LEN;
    printf("intNum %s ", label);
    // little endian
    for (int i = length - 1; i >= 0; i--) {
        printf(
            "%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d", 
            // val[i] >> 27 & 1,
            // val[i] >> 26 & 1,
            val[i] >> 25 & 1,
            val[i] >> 24 & 1,
            val[i] >> 23 & 1,
            val[i] >> 22 & 1,
            val[i] >> 21 & 1,
            val[i] >> 20 & 1,
            val[i] >> 19 & 1,
            val[i] >> 18 & 1,
            val[i] >> 17 & 1,
            val[i] >> 16 & 1,
            val[i] >> 15 & 1,
            val[i] >> 14 & 1,
            val[i] >> 13 & 1,
            val[i] >> 12 & 1,
            val[i] >> 11 & 1,
            val[i] >> 10 & 1,
            val[i] >> 9 & 1,
            val[i] >> 8 & 1,
            val[i] >> 7 & 1,
            val[i] >> 6 & 1,
            val[i] >> 5 & 1,
            val[i] >> 4 & 1,
            val[i] >> 3 & 1,
            val[i] >> 2 & 1,
            val[i] >> 1 & 1,
            val[i] & 1
        );
    }
    printf("\n");
}
