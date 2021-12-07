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

// prints char* as big endian
void printCharNum(char label[], unsigned char *val);

// print int* as big endian
void printInts(int *limbs, unsigned int number);

// prints char* as big endian in binary (useful for debugging)
void printCharNumBinary(char label[], unsigned char *val);

// prints int* as big endian in binary (useful for debugging)
void printIntsBinary(char label[], int *val);

// converts char* to int*
int *convertC2I(unsigned char *val);

//addition of two 2^8 base number i.e A and B and result is a 2^26 base number stored in X
void add(int *X, int *A, int *B);

// MAIN FUNCTION

int main() {
    unsigned char *charA = rand127();
    unsigned char *charB = rand127();

    printCharNumBinary("A", charA);
    // printCharNumBinary("B", charB);

    int *intA = convertC2I(charA);
    printIntsBinary("A", intA);


    //
    srand(time(NULL));
    //(a) generate A and B randomly as a string of characters.
    char *A = malloc(16);
    char *B = malloc(16);
    rand127(A, B);
    printf("Random 127 bits in 8 bit format\n");
    for (int i = 0; i < 16; i++)
    {
        printf("A :%d\tB:%d\n", (unsigned char)A[i], (unsigned char)B[i]);
    }

    //(b) Convert A and B in base 2 26 using the function convertC2I.
    int resultA[5];
    int resultB[5];
    printf("Base 2^26 representation of A and B\n");
    printf("A:");
    convertC2I(A, resultA);
    printf("\n\nB:");
    convertC2I(B, resultB);

    //(c) Call function add to compute the addition.
    int result1[5];
    int x[16], y[16];
    for (int i = 0; i < 16; i++)
    {
        unsigned char temp = (unsigned char)A[i];
        x[i] = (int)temp;
        temp = (unsigned char)B[i];
        y[i] = (int)temp;
    }
    printf("\n\nAddition of A and B and result is in base 2^26\n");
    Add(result1, x, y);

    free(A);
    free(B);

    return 0;
}

// FUNCTION IMPLEMENTATION STARTS HERE

unsigned char *rand127() {
    unsigned char *val = malloc(CHAR_NUM_LEN sizeof(unsigned char));
    for (int i = 0; i < 16; i++) {
        if(i < 15) {
            val[i] = rand() % 256;
        } else {
            val[i] = rand() % 128;
        }
        // val[i] = 255;
    }
    return val;
}

int *convertC2I(unsigned char *val) {
    int *result = malloc(INT_NUM_LEN * sizeof(int));

    int bitPosition = 0;
    for (int i = 0; i < CHAR_NUM_LEN; i++) {
        if((bitPosition % 26) <= 18) { // 26 - 8
            result[bitPosition / 26] |= (val[i] << (bitPosition % 26));
        } else {
            int valAnd = (1 << (27 - (bitPosition % 26))) - 1;
            result[bitPosition / 26] += (val[i] & valAnd) << (bitPosition % 26);
            result[bitPosition / 26 + 1] += val[i] >> (26 - (bitPosition % 26));   
        }
            // printIntsBinary("X", result);
        bitPosition += 8;
    }

    return result;
}

void add(int *X, int *A, int *B) {
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

    char s[17];
    for (int i = 0; i < 17; i++) {
        s[i] = C[i];
    }

    convertC2I(s, X);
}

// PRINTING FUNCTIONS BELOW

void printCharNum(char label[], unsigned char *val) {
    printf("Printing charNum %s: ", label);
    // little endian
    for (int i = CHAR_NUM_LEN - 1; i >= 0; i--) {
        printf("%d ", val[i]);
    }
    printf("\n");
}

void printInts(int *limbs, unsigned int number) {
    for (int i = number - 1; i >= 0; i--) {
        printf("%d ", limbs[i]);
    }
    printf("\n");
}

void printCharNumBinary(char label[], unsigned char *val) {
    printf("Printing charNum %s:  ", label);
    // little endian
    for (int i = CHAR_NUM_LEN - 1; i >= 0; i--) {
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
void printIntsBinary(char label[], int *val) {
    printf("Printing intNum %s: ", label);
    // little endian
    for (int i = INT_NUM_LEN - 1; i >= 0; i--) {
        printf(
            "%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d", 
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
