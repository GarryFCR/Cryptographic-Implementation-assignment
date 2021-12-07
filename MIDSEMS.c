#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

//(a) convertC2I(char ∗ A): converts a multi-precision integer in base 2^8 to a multi-precision integer in
//base 2^26 .
// s is the reference to the base 2^8 representation coefficients
// coef is the reference to the base 2^26 representation coefficients that will be written by the function
void convertC2I(char *s, int *coef)
{
    int arr[128];
    int digit = 0, count = 0, pos, index = 0;
    int no_of_bits = 0;

    //converting to binary
    for (int i = 0; i < 16; i++)
    {
        unsigned char val = (unsigned char)s[i];
        for (int j = 0; j < 8; j++)
        {
            arr[index] = 1 & val;
            //printf("%d", arr[index]);
            index++;
            val = val >> 1;
        }
    }
    index = 0;
    //clubbing 26 bit at a time for a digit
    for (int k = 0; k < 5; k++)
    {
        int loop = 26;
        if (k == 4)
        {
            loop = 24;
        }
        for (int j = 0; j < loop; j++)
        {

            pos = (int)pow((double)2, (double)j);
            digit = digit + (pos * arr[index]);
            index++;
        }
        coef[k] = digit;
        printf("\n%d", digit);
        digit = 0;
    }
}
//generating random 127 bit number
void rand127(char *A, char *B)
{

    int temp;

    // first generating the random 128 bits
    for (int i = 0; i < 16; i++)
    {
        temp = rand() % 256;
        A[i] = temp;

        temp = rand() % 256;
        B[i] = temp;
    }

    //right shifting the MSB so that it is 7 bits
    //overall becomes 127 bits
    A[15] = 128 ^ (A[15] >> 1);
    B[15] = 128 ^ (B[15] >> 1);
}

//addition of two 2^8 base number i.e A and B and result is a 2^26 base number stored in X
void Add(int *X, int *A, int *B)
{

    int a, b, r, i = 0, j = 0, k = 0;
    int sum = 0;
    int C[17];
    C[16] = 0;

    for (i = 0; i < 16; i++)
    {
        C[i] = A[i] + B[i];
    }
    for (j = 0; j < 16; j++)
    {

        if (C[j] > 255)
        {
            a = C[j] / 256;
            b = C[j] % 256;

            C[j] = b;
            if (j != 15)
            {
                C[j + 1] = C[j + 1] + a;
            }
            else
            {
                C[j + 1] = a;
            }
        }
    }

    char s[17];
    for (int i = 0; i < 17; i++)
    {
        s[i] = C[i];
    }

    convertC2I(s, X);
}
void convertI2C(int *s, int *coef)
{
    int arr[128];
    int digit = 0, count = 0, pos, index = 0;
    int no_of_bits = 0;

    //converting to binary
    for (int i = 0; i < 5; i++)
    {
        int val = s[i];
        for (int j = 0; j < 26; j++)
        {
            arr[index] = 1 & val;
            index++;
            val = val >> 1;
        }
    }
    index = 0;
    //clubbing 8 bit at a time for a digit
    for (int k = 0; k < 16; k++)
    {

        for (int j = 0; j < 8; j++)
        {

            pos = (int)pow((double)2, (double)j);
            digit = digit + (pos * arr[index]);
            index++;
        }
        coef[k] = digit;
        printf("%d\t", digit);
        digit = 0;
    }
}
int main()
{
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

    //(e) Convert the outputs of add and mult into a base 2 8 multi-precision integer using the function
    //convertI2C.
    int resultC[5];
    printf("\n\nConverting addition of A and B to base 2^8 \n");
    convertI2C(result1, resultC);

    free(A);
    free(B);

    return 0;
}