// This is a code for tasks starting with 4-3 (reversed Polish notation
// calculator)
//
// Hope to implement at least a couple

#include <ctype.h>
#include <stdio.h>

#define INPBUF_SIZE 100
/*
 * This is an input char buffer, used to recognize and parse input.
 */
char inpbuf[INPBUF_SIZE];
int inpe = 0; // points past the end of the buffer (inpbuf[inpe] is illegal)

int input_get(int position) {
    int i, c;

    if (position < 0 || position >= INPBUF_SIZE)
        // that's a bad request
        return -1;

    // buffer is in a normal state
    if (position < inpe)
        // we already have a char, return it right away
        return inpbuf[position];

    // char must be fetched
    // fetch forward until desired char
    for (i = inpe; i <= position && (c = getchar()) != EOF; ++i)
        inpbuf[i] = c;
    inpe = i;
    return c; // this will propagate EOF too!
}

void input_forget(int position) {
    int i;

    if (position <= 0 || position >= INPBUF_SIZE)
        // bad input; nothing to do
        return;

    if (position >= inpe) {
        inpe = 0;
        return;
    } else {
        for (i = 0; i <= (inpe - 1 - position); ++i)
            inpbuf[i] = inpbuf[i + position];
        inpe -= position;
    }
}

/* clears the input buffer */
void input_clear() { inpe = 0; }

#define MAX_OPERANDS 100
/* This is an operand stack */
double oprds[MAX_OPERANDS];
int oprptr = -1;

/* gets an operand from stack, or warns are returns 42.0, if none is available
 */
double pop_oprd() {
    if (oprptr < 0) {
        fprintf(stderr, "\tThere are no operands present, so have a 42.0\n");
        return 42.0;
    } else
        return oprds[oprptr--];
}

/* Puts operand on a stack, for later use */
void put_oprd(double oprd) {
    if (oprptr >= MAX_OPERANDS - 1)
        fprintf(stderr, "\tOperand buffer overflow! Discarded operand: %f\n",
                oprd);
    else
        oprds[++oprptr] = oprd;
}

enum InputType {
    Operand, // a number
    Plus,    // '+'
    Minus,   // '-'
    Star,    // '*'
    Slash,   // '/'
    Eof,     // THE end
    Unknown, // couldn't recognize the input :(
};

double parse_double(int i) {
    int decimals = 0, c = input_get(i);
    double mantice = 0.0;
    while (isdigit(c)) {
        mantice = mantice * 10 + (c - '0');
        c = input_get(++i);
    }
    if (c != '.') {
        // forget input up to this point
        input_forget(i);
        // return the result
        return mantice;
    }
    // decimal pointer was encountered
    c = input_get(++i);
    while (isdigit(c)) {
        mantice = mantice * 10 + (c - '0');
        decimals += 1;
        c = input_get(++i);
    }
    // forget pointer up until this point
    input_forget(i);
    // make decimal places
    while (decimals--)
        mantice /= 10;
    // return the result
    return mantice;
}

/*
 * Reads next input argument or an operation.
 *
 * If it happens to be a number, it's pushed to the operands stack
 */
enum InputType read_next() {
    int i = 0;
    int c = input_get(i);
    double sign = 1.0;

    // skip the whitespace
    while (isblank(c) || c == '\n')
        c = input_get(++i);

    if (c == EOF)
        // well, it's the end
        return Eof;

    if (c == '*') {
        // that's a star operator!
        input_forget(i + 1);
        return Star;
    }

    if (c == '/') {
        // that's a slash operator!
        input_forget(i + 1);
        return Slash;
    }

    if (c == '+') {
        c = input_get(++i);
        if (!isdigit(c)) {
            // A plus followed by a non-digit => it's a plus operator!
            input_forget(i);
            return Plus;
        }
    } else if (c == '-') {
        c = input_get(++i);
        if (!isdigit(c)) {
            // A minus followed by a non-digit => it's a minus operator!
            input_forget(i);
            return Minus;
        }

        // note that minus was encountered
        sign = -1.0;
    } else if (!isdigit(c)) {
        // it's not a number, and not one of the operators
        // meaning, it's unknwon input
        input_clear();
        return Unknown;
    }

    // at this point, we should be parsing a number
    put_oprd(parse_double(i) * sign);
    return Operand;
}

int main() {
    enum InputType type;
    double op1, op2;
    while ((type = read_next()) != Eof) {
        // This was used for debug
        /*
        switch (type) {
        case Operand:
            printf("Operand: %f\n", pop_oprd());
            break;
        case Plus:
            printf("Plus\n");
            break;
        case Minus:
            printf("Minus\n");
            break;
        case Star:
            printf("Star\n");
            break;
        case Slash:
            printf("Slash\n");
            break;
        case Unknown:
            printf("Unknown input received\n");
            break;
        }
        */
        switch (type) {
        case Operand:
            // do nothing. was already added to the stack
            break;
        case Plus:
            // perform addition
            op2 = pop_oprd();
            op1 = pop_oprd();
            put_oprd(op1 + op2);
            break;
        case Minus:
            // perform subtraction
            op2 = pop_oprd();
            op1 = pop_oprd();
            put_oprd(op1 - op2);
            break;
        case Star:
            // perform multiplication
            op2 = pop_oprd();
            op1 = pop_oprd();
            put_oprd(op1 * op2);
            break;
        case Slash:
            // perform division
            op2 = pop_oprd();
            if (op2 == 0.0) {
                // No allowed
                fprintf(stderr,
                        "\tAttempt to divide by number of my brain cells rn\n");
                return -1;
            }
            op1 = pop_oprd();
            put_oprd(op1 / op2);
            break;
        case Unknown:
            // print to stderr
            fprintf(stderr, "\tGot unknown input!\n");
            break;
        }
    }
    // in the end, print to operand in the stack
    printf("Answer to everything is %f\n", pop_oprd());
}
