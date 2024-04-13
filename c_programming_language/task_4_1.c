#include <stdio.h>

int last_str_index(char *hay, char *pat) {
    register int s = 0;
    char register hc, pc, *shay, *spat;
    // first, find the end
    while (hay[s])
        ++s;
    // now s is a pointer to NULL byte at the end
    // next, iterate back
    while (*(shay = &hay[--s])) {
        // while trying to match a pattern
        spat = pat;
        while ((hc = *shay++) && hc == (pc = *spat++))
            ;
        // if at a break pattern was fully consumed -- that's a match!
        if (!pc)
            return s;
    }
    // returns -1, it no index was found
    return -1;
}

#define BUF_LEN 100
int main(int argc, char **argv) {
    char s[BUF_LEN], *t;
    int ind;
    if (argc < 1)
        ;
    else if (argc < 2)
        printf("Should supply at least one argument to %s", argv[0]);
    else {
        t = argv[1];
        while (!feof(stdin)) {
            fgets(s, BUF_LEN, stdin);
            ind = last_str_index(s, t);
            if (ind < 0)
                printf("No match\n");
            else
                printf("Matched part: %s\n", &s[ind]);
        }
    }
}
