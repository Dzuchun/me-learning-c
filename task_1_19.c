#include <stdio.h>

#define MAX_LEN 100

int string_length(int s[]) {
    int len;
    len = 0;
    while (s[len] != '\0')
        len += 1;
    // len is at a NULL-charater position
    return len;
}

void swap(int s[], int ind1, int ind2) {
    int tmp;
    tmp = s[ind1];
    s[ind1] = s[ind2];
    s[ind2] = tmp;
}

void reverse_string(int s[]) {
    int len, i;
    len = string_length(s);
    for (i = 0; i <= len / 2; i += 1)
        swap(s, i, len - i - 1);
}
int read_line(int line[]) {
    int i, c;
    i = 0;
    while ((c = getchar()) != EOF) {
        if (c == '\n')
            break;
        line[i] = c;
        i += 1;
        if (i == (MAX_LEN - 1))
            break;
    }
    line[i] = '\0';
    if (c == EOF)
        return -1;
    return i;
}

void print_line(int line[]) {
    int i;
    for (i = 0; i < MAX_LEN; i += 1) {
        if (line[i] == '\0')
            break;
        putchar(line[i]);
    }
    putchar('\n');
}

int main() {
    int len;
    int buf[MAX_LEN];
    while ((len = read_line(buf)) >= 0) {
        reverse_string(buf);
        print_line(buf);
    }
}
