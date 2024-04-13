#include <stdio.h>

/*
 * Counts number of set bits
 */
unsigned int bitcount(unsigned int x) {
    int res = 0;
    while (x) {
        x &= (x - 1);
        res++;
    }
    return res;
}

int main() {
    unsigned int input;
    while (fscanf(stdin, "%x", &input) != EOF)
        fprintf(stdout, "%d\n", bitcount(input));
}
