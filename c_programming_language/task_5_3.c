
#include <stdio.h>
void my_strcat(char *s, char *t) {
    // first, find end of s
    while (*s)
        ++s;
    // then, concat t to end of s
    while (*t)
        *s++ = *t++;
    // finally, add NULL-byte at the end
    *s = '\0';
}

#define MAX_LEN 100

int main() {
    char s[MAX_LEN];
    fprintf(stdout, "s = ");
    fgets(s, MAX_LEN, stdin);

    my_strcat(s, " - Dracula said. You are a meme, lol!");

    printf("%s", s);
}
