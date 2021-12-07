// Assignment Midsem

#include <stdio.h>
#include <stdlib.h>

// function to allocate memory
int *allocateInts(unsigned int number) {
    int *limbs = malloc(number * sizeof(int));
    return limbs;
}

int *scanInts(unsigned int number) {
    printf("Scanning MultiPrecision int of length %d\n", number);
    int *limbs = allocateInts(number);
    for (int i = number - 1; i >= 0; i--) {
        printf("Enter limb %d: ", number - i);

        scanf("%d", &limbs[i]);

        if(limbs[i] >= 1 << 26) {
            printf("scanInts: %d is too large\n", limbs[i]);
            exit(1);
        }
    }
    return limbs;
}

void printInts(int *limbs, unsigned int number) {
    for (int i = number - 1; i >= 0; i--) {
        printf("%d ", limbs[i]);
    }
    printf("\n");
}

void addVal(int *ptr, int val) {
    if(val >= 1 << 26) {
        printf("addVal: %d is too large\n", val);
        exit(1);
    }

    if((ptr[0] + val) < (1 << 26)) {
        ptr[0] = ptr[0] + val;
    } else {
        ptr[0] = (ptr[0] + val) % (1 << 26);
        ptr[1] += 1; // add a carry
    }
}

// Karatsuba (https://brilliant.org/wiki/karatsuba-algorithm/)
void multiplyKaratsubaStep(int x, int y, int *result) {
    // splitting x and y into two halves
    int xH = x >> 13; 
    int xL = x & 0x1fff;
    int yH = y >> 13;
    int yL = y & 0x1fff;

    // xy = a*(b**n) + e*(b**n/2) + d
    int a = xH * yH;
    int d = xL * yL;
    int e = xH * yL + xL * yH; // (xH + xL) * (yH + yL) - a - d;

    // writing the result to pointer
    // result[1] += a;
    // result[1] += e >> 13;
    // result[0] += (e & 0x3ffffff) << 13;
    // result[0] += d;

    addVal(result + 1, a);

    addVal(result + 1, e >> 13);

    addVal(result, (e & 0x1fff) << 13);

    addVal(result, d);
}

int *multiply(int *x, unsigned int xLen, int *y, unsigned int yLen, unsigned int resultLen) {
    int *result = allocateInts(xLen + yLen);
    for (int i = 0; i < xLen; i++) {
        for (int j = 0; j < yLen; j++) {
            // result[i+j] += x[i] * y[j];
            multiplyKaratsubaStep(x[i], y[j], result + (i + j));
        }
    }
    
    // finally we need to shift the result to the right
    if(resultLen < xLen + yLen) {
        for (int i = resultLen; i < xLen + yLen - 1; i++) {
            if(result[i] != 0) {
                printf("multiply: %d is too small\n", resultLen);
            }
        }
        int *resultSmall = allocateInts(resultLen);
        for (int i = 0; i < resultLen; i++) {
            resultSmall[i] = result[i];
        }
        free(result);
        return resultSmall;
    }

    return result;
}

int main() {
    int *limbs1, *limbs2, *result;

    limbs1 = scanInts(5);
    printInts(limbs1, 5);

    limbs2 = scanInts(5);
    printInts(limbs2, 5);

    result = multiply(limbs1, 5, limbs2, 5, 10);
    printInts(result, 10);

    return 0;
}
