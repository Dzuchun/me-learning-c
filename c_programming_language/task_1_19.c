#include <stdio.h>

/* finds end of the passed string */
#define find_end(s)                                                            \
                                                                               \
    char *s##_end = s;                                                         \
    while (*s##_end)                                                           \
        s##_end++;                                                             \
    s##_end--;

/* swaps data under passed pointers of specific type */
#define swap(t, p1, p2)                                                        \
    {                                                                          \
        t tmp = *p1;                                                           \
        *p1 = *p2;                                                             \
        *p2 = tmp;                                                             \
    }
// wow, this formatting really surprised me. that was cool.

/* reverses string in-place */
void reverse_string(char *s) {
    find_end(s);
    while (s < s_end) {
        swap(char, s, s_end);
        s++;
        s_end--;
    }
}

/* reads line from input. retuns EOF, if EOF was hit. newline character at the
 * end is not included */
int read_line(char *buf, int cap) {
    char c;
    while (cap-- && (c = getchar()) != EOF && c != '\n')
        *buf++ = c;
    *buf = '\0';
    return (c != EOF) ? 0 : EOF;
}

/* prints line to the output, followed by a newline */
void print_line(char *buf, int cap) {
    while (cap-- && *buf) {
        putchar(*buf++);
    }
    putchar('\n');
}

#define MAX_LEN 100

int main() {
    char buf[MAX_LEN];
    while (read_line(buf, MAX_LEN) != EOF) {
        reverse_string(buf);
        print_line(buf, MAX_LEN);
    }
}
