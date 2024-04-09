#include <stdio.h>
#define BUF_LEN 100

int strrindex(char s[], char t[]) {
    int last_find = -1, i, j;
    // for each possible position in s
    for (i = 0; i < BUF_LEN && s[i] != '\0'; ++i) {
        // try iterating through s and t
        for (j = 0; i + j < BUF_LEN && s[i + j] == t[j]; ++j)
            ;
        // if iteration has stopped at t[j] == '\0', i is a match
        if (t[j] == '\0')
            last_find = i;
    }

    // returns -1, for no index was found
    return last_find;
}

int main() {
    char s[BUF_LEN], t[BUF_LEN];
    printf("Input pattern: ");
    fgets(&t[0], BUF_LEN, stdin);
    while (!feof(stdin)) {
        fgets(&s[0], BUF_LEN, stdin);
        printf("%d\n", strrindex(&s[0], &t[0]));
    }
}
