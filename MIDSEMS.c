#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

void rand128(char *A, char *B)
{

    int temp;

    for (int i = 0; i < 16; i++)
    {
        temp = rand() % 256;
        printf("%d\t", temp);
        A[i] = temp;

        temp = rand() % 256;
        B[i] = temp;
    }

    // A[0] = 128 ^ (A[0] >> 1);
    //B[0] = 128 ^ (B[0] >> 1);
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
            if (j != 16)
            {
                C[j + 1] = C[j + 1] + a;
            }
            else
            {
                C[j + 1] = a;
            }
        }
    }
}

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
            printf("%d", arr[index]);
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
        printf("\n%d\n", digit);
        digit = 0;
    }
}

int main()
{
    srand(time(NULL));
    char *A = malloc(16);
    char *B = malloc(16);
    rand128(A, B);
    printf("\n");

    int result[5];
    convertC2I(A, result);
    //printf("%s\n", A);
    //printf("%s", B);

    free(A);
    free(B);

    return 0;
}