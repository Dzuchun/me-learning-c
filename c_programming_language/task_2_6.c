#include <stdio.h>

int setbits(int x, int p, int n, int y) {
    int tmp;
    // first, we'll set ones, where needed
    // create a mask
    tmp = (1 << n) - 1;
    // extract n rightmost bits of y
    tmp &= y;
    // move them to position p
    tmp <<= p;
    // set ones
    x |= tmp;

    // then, set zeroes
    // create a mask
    tmp = (1 << n) - 1;
    // extract n rightmost zeroes of y
    tmp &= ~y;
    // move them to position p
    tmp <<= p;
    // set zeroes
    x &= ~tmp;

    return x;
}

int main() {
    int x, p, n, y, res;
    while (1) {
        printf("input x: ");
        if (scanf("%x", &x) == EOF)
            break;
        printf("input p: ");
        if (scanf("%x", &p) == EOF)
            break;

        printf("input n: ");
        if (scanf("%x", &n) == EOF)
            break;

        printf("input y: ");
        if (scanf("%x", &y) == EOF)
            break;

        res = setbits(x, p, n, y);
        printf("result: %X\n\n", res);
    }
}
