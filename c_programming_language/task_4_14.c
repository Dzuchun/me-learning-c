#include <stdio.h>
#define swap(t, x, y)                                                          \
    {                                                                          \
        t tmp = x;                                                             \
        x = y;                                                                 \
        y = tmp;                                                               \
    }

int main() {
    int x = 56;
    int y = 23;
    printf("x = %d, y = %d\n", x, y);

    swap(int, x, y);
    printf("x = %d, y = %d\n", x, y);
}
