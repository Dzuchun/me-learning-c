
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int parse_n(char *s) {
    if ((*s++ != '-') || !isdigit(*s))
        return -1;
    int res = 0;
    char c;
    while ((c = *s++) && isdigit(c))
        res = res * 10 + (c - '0');
    if (c)
        return -2;
    return res;
}

int a_normal_fucking_fgets(char *line, int max_len, FILE *file) {
    max_len--;
    int c = fgetc(file);
    if (c == EOF)
        return -1;
    while (max_len-- && c != EOF && c != '\n') {
        *line++ = c;
        c = fgetc(file);
    }
    *line = '\0';
    return max_len;
}

#define MAX_LINE_LEN 200

/* Unix `tail` copycat */
int main(int argc, char **argv) {
    // freopen("./README.md", "r", stdin);
    int n, wrapped = 0;
    if (argc < 2)
        n = 10;
    else if ((n = parse_n(argv[1])) <= 0) {
        printf("Please enter valid integer number of lines, or ommit the "
               "argument\n");
        return n;
    }

    char line_arr[MAX_LINE_LEN];
    char *line = line_arr;
    // this is supposed to be array of '\0'
    char(*buf)[MAX_LINE_LEN] = calloc(sizeof(char), n * MAX_LINE_LEN);
    int i = 0;
    while (a_normal_fucking_fgets(line, MAX_LINE_LEN, stdin) >= 0) {
        // it seems that fgets has a little feature of NOT WORKING PROPERLY.
        // ALL THE SPECT I COULD FIND, STATE THAT MUST RETURN NULL, IF NO
        // CHARACTERS WERE WRITTEN. BUT IT DOES. NOT. DO. THAT.
        //
        // fun fact: specs also state that it should set EOF (bit or th) for
        // feof well, it won't do that either.
        //
        // basically, as far as I can see, there's always a phantom read of 0
        // bytes that does not change the EOF flag and does not return NULL.
        // f*cking brilliant.
        strcpy(buf[i], line);
        i++;
        if (i == n)
            wrapped = 1;
        i %= n;
    }
    if (!wrapped)
        // i points past the last line
        // to print all lines, we'll iterate 0..i
        // we'll reassign n to the actual number of lines
        // and use i for iteration
        for (n = i, i = 0; i < n; ++i)
            puts(buf[i]);
    else {
        // i points at the first line to be printed
        // now, iterate exactly n times
        for (int exactly_n = n; exactly_n--; i++, i %= n)
            puts(buf[i]);
    }

    free(buf);
}
