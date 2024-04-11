#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

// copied from task-1-19
/* reads line from input. retuns EOF, if EOF was hit. newline character at the
 * end is not included */
int read_line(char *buf, int cap) {
    char c;
    while (cap-- && (c = getchar()) != EOF && c != '\n')
        *buf++ = c;
    *buf = '\0';
    return (c != EOF) ? 0 : EOF;
}

// copied from task-1-19
/* prints line to the output, followed by a newline */
void print_line(char *buf, int cap) {
    while (cap-- && *buf) {
        putchar(*buf++);
    }
    putchar('\n');
}

#define MAX_LINE_LENGTH 100
#define MAX_LINES 500

/* reads lines from input, placing NULL pointer at the end */
int read_lines(char **buf, int max_lines, int max_len) {
    char **start = buf;
    while (max_lines-- && read_line(*buf++, max_len) != EOF)
        ;
    *buf = NULL; // indicate end of the lines array with a NULL pointer
    return buf - start;
}

/* prints to output lines stored in the buffer */
void print_lines(char **buf, int max_lines, int max_len) {
    char *line;
    while (max_lines-- && (line = *buf++) != NULL)
        print_line(line, max_len);
}

#define swap(t, a, b)                                                          \
    {                                                                          \
        t tmp = b;                                                             \
        b = a;                                                                 \
        a = tmp;                                                               \
    }

int lex_cmp(char *s1, char *s2) {
    char c1, c2;
    while ((c1 = *s1) && (c2 = *s2) && c1 == c2)
        ++s1, ++s2;
    return (!c1 && !c2) ? 0 : c1 - c2;
}

int num_cmp(char *s1, char *s2) {
    double v1 = atof(s1), v2 = atof(s2);
    return (v1 > v2) ? 1 : (v1 == v2) ? 0 : -1;
}

int dir_cmp(char *s1, char *s2) {
    char c1 = *s1++, c2 = *s2++;
    for (;;) {
        while (c1 != '\0' && !isalnum(c1) && !isblank(c1))
            c1 = *s1++;
        while (c2 != '\0' && !isalnum(c2) && !isblank(c2))
            c2 = *s2++;
        if (c1 != c2)
            return c1 - c2;
        if (c1 == '\0')
            return 0;
        c1 = *s1++;
        c2 = *s2++;
    }
}

void sort_lines(char **buf, int left, int right, int (*comp)(void *, void *)) {
    int i, last;
    if (left >= right)
        return;
    last = (left + right) / 2;
    swap(char *, buf[left], buf[last]);
    last = left;
    for (i = left + 1; i <= right; ++i)
        if ((*comp)(buf[i], buf[left]) < 0) {
            ++last;
            swap(char *, buf[last], buf[i]);
        }

    swap(char *, buf[left], buf[last]);
    sort_lines(buf, left, last - 1, comp);
    sort_lines(buf, last + 1, right, comp);
}

int main() {
    char raw_buf[(MAX_LINES + 1) * (MAX_LINE_LENGTH + 1)];
    char *buf[MAX_LINES + 1];
    int lines;
    for (lines = 0; lines < MAX_LINES; ++lines)
        buf[lines] = &raw_buf[lines * (MAX_LINE_LENGTH + 1)];
    buf[MAX_LINES] = NULL;

    // freopen("./task_5_7.c", "r", stdin);
    lines = read_lines((char **)buf, MAX_LINES, MAX_LINE_LENGTH);
    sort_lines((char **)buf, 0, lines - 1, (int (*)(void *, void *))dir_cmp);
    print_lines((char **)buf, lines - 1, MAX_LINE_LENGTH);
}
