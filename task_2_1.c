#include <limits.h>
#include <stdio.h>

int main() {
    printf("char limits: [%d; %d]\n", CHAR_MIN, CHAR_MAX);
    printf("short limits: [%d; %d]\n", SHRT_MIN, SHRT_MAX);
    printf("int limits: [%d; %d]\n", INT_MIN, INT_MAX);
    printf("long limits: [%d; %d]\n", LONG_MIN, LONG_MAX);

    printf("signed char limits: [%d; %d]\n", SCHAR_MIN, SCHAR_MAX);
    // there's no such thing?
    // printf("signed short limits: [%d; %d]\n", SHRT_MIN, SHRT_MAX);
    // printf("signed int limits: [%d; %d]\n", INT_MIN, INT_MAX);
    // printf("signed long limits: [%d; %d]\n", LONG_MIN, LONG_MAX);

    printf("unsigned char limits: [0; %d]\n", UCHAR_MAX);
    printf("unsigned short limits: [0; %d]\n", USHRT_MAX);
    printf("unsigned int limits: [0; %d]\n", UINT_MAX);
    printf("unsigned long limits: [0; %d]\n", ULONG_MAX);
}
