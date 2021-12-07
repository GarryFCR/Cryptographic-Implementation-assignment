#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

void convertC2I(char *s, int *coef)
{
    int arr[128];
    int digit = 0, count = 0, pos, index = 0;
    int no_of_bits = 0;

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
void rand127(char *A, char *B)
{

    int temp;

    for (int i = 0; i < 16; i++)
    {
        temp = rand() % 256;
        A[i] = temp;

        temp = rand() % 256;
        B[i] = temp;
    }

    A[15] = 128 ^ (A[15] >> 1);
    B[15] = 128 ^ (B[15] >> 1);
}

//addition
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
        printf("A :%d\tB:%d\n", A[i], B[i]);
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