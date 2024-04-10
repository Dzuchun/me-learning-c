#include "calc.h"
#include <stdio.h>

#define INPBUF_SIZE 100
/*
 * This is an input char buffer, used to recognize and parse input.
 */
char inpbuf[INPBUF_SIZE];
int inpe = 0;
int inpb = 0; // points to next unparsed char in the input buffer

int input_len() { return (inpe >= inpb) ? inpe - inpb : 0; }

void input_clear() { inpb = inpe = 0; }

void input_fetch() {
    int i;
    char c;
    if (inpb < inpe)
        // there are still characters in the buffer
        return;

    inpb = inpe = 0;

    while (!feof(stdin) && !input_len()) {

        fgets(&inpbuf[0], INPBUF_SIZE, stdin);

        for (i = 0;
             (i < INPBUF_SIZE) && ((c = inpbuf[i]) != '\0') && (c != '\n'); ++i)
            ;
        // we either reached the end (kinda bad!)
        // or encountered a NULL-byte or newline
        // replace newline with NULL
        if (c == '\n')
            inpbuf[i] = '\0';
        inpe = i;
    }
    if (feof(stdin))
        inpb = inpe = 0;
}

int input_get(int position) {
    return (position < inpb || position >= inpe) ? -1 : inpbuf[position];
}

void forget_to(int new_bp) {
    if (new_bp <= inpb)
        // there's nothing to do
        ;
    else if (new_bp >= inpe)
        // set to end position
        inpb = inpe;
    else
        inpb = new_bp;
}

int input_begin() { return inpb; }
