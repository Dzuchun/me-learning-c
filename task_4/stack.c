#include <stdio.h>

#define MAX_OPERANDS 100
double oprds[MAX_OPERANDS];
int oprptr = 0;

double oprd_pop() {
    if (oprptr <= 0) {
        fprintf(stderr, "\tThere are no operands present, so have a 42.0\n");
        return 42.0;
    } else
        return oprds[--oprptr];
}

void oprd_put(double oprd) {
    if (oprptr >= MAX_OPERANDS)
        fprintf(stderr, "\tOperand buffer overflow! Discarded operand: %f\n",
                oprd);
    else
        oprds[oprptr++] = oprd;
}

int oprd_len() { return oprptr; }

double nth_oprd(int n) {
    if (n <= oprptr)
        return oprds[oprptr - 1 - n];
    // return garbage
    double d;
    return d;
}

void oprd_clear() { oprptr = 0; }
