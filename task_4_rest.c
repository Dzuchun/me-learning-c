// This is a code for tasks starting with 4-3 (reversed Polish notation
// calculator)
//
// Hope to implement at least a couple
//
// NOTE: this code intentionally avoids using std library. the point is to get
// accustomed to the language, not fast developing

#include <ctype.h>
#include <math.h>
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
int oprptr = 0;

/* gets an operand from stack, or warns are returns 42.0, if none is available
 */
double pop_oprd() {
    if (oprptr <= 0) {
        fprintf(stderr, "\tThere are no operands present, so have a 42.0\n");
        return 42.0;
    } else
        return oprds[--oprptr];
}

/* Puts operand on a stack, for later use */
void put_oprd(double oprd) {
    if (oprptr >= MAX_OPERANDS)
        fprintf(stderr, "\tOperand buffer overflow! Discarded operand: %f\n",
                oprd);
    else
        oprds[oprptr++] = oprd;
}

enum InputType {
    Operand, // a number
    Plus,    // '+'
    Minus,   // '-'
    Star,    // '*'
    Slash,   // '/'
    Percent, // '%'
    Top,     // print 2 top elements
    Dup,     // duplicate top element
    Swp,     // swap 2 top elements
    Clr,     // clears the stack
    Sin,     // sin library function
    Exp,     // exp library function
    Pow,     // pow library function
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

int match_word(int i, const char *word) {
    int c, t, j;
    for (j = 0; (c = input_get(i)) != EOF && c == (t = word[j]) && t != '\0';
         ++i, ++j)
        ;
    if (t != '\0') {
        // match had failed.
        return 0;
    }
    // forget the input until word's last char
    input_forget(i);
    return 1;
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

    if (c == '%') {
        // that's a percent operator!
        input_forget(i + 1);
        return Percent;
    }

    if (match_word(i, "top")) {
        // that's a top operator!
        // (unput was forgorren already)
        return Top;
    }

    if (match_word(i, "dup")) {
        return Dup;
    }

    if (match_word(i, "swp")) {
        return Swp;
    }

    if (match_word(i, "clr")) {
        return Clr;
    }

    if (match_word(i, "sin")) {
        return Sin;
    }

    if (match_word(i, "exp")) {
        return Exp;
    }

    if (match_word(i, "pow")) {
        return Pow;
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
    double op1, op2, sin(double), exp(double), pow(double, double);
    while (1) {
        type = read_next();
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
        case Eof:
            // break out of the cycle
            goto end;
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
        case Percent:
            // perform modulo
            op2 = pop_oprd();
            if (op2 == 0.0) {
                fprintf(stderr, "\tCan't take modulo by 0\n");
                return -1;
            }
            op1 = pop_oprd();
            put_oprd((double)((int)op1 % (int)op2));
            break;
        case Top:
            // reaction depends on number of elements in the stack
            switch (oprptr) {
            case 0:
                // there are no elements
                // complete disaster
                fprintf(stdout, "There are no operands in the stack\n");
                break;
            case 1:
                // there's only one element
                fprintf(stdout, "There's a single operand in the stack: %f\n",
                        oprds[oprptr - 1]);
                break;
            default:
                // there are at least 2 elements!
                fprintf(stdout, "Last operands in the stack: [.., %f, %f]\n",
                        oprds[oprptr - 2], oprds[oprptr - 1]);
                break;
            }
            break;
        case Swp:
            if (oprptr < 2)
                fprintf(stdout, "Can't swp top operands: there are 0 or 1 "
                                "operands in the stack\n");
            else {
                op1 = oprds[oprptr - 1];
                oprds[oprptr - 1] = oprds[oprptr - 2];
                oprds[oprptr - 2] = op1;
            }
            break;
        case Dup:
            if (oprptr < 1)
                fprintf(stdout, "Can't dup top operand, as there are no "
                                "operands in the stack");
            else
                put_oprd(oprds[oprptr - 1]);
            break;
        case Clr:
            oprptr = 0;
            break;
        case Sin:
            put_oprd(sin(pop_oprd()));
            break;
        case Exp:
            put_oprd(exp(pop_oprd()));
            break;
        case Pow:
            op2 = pop_oprd();
            op1 = pop_oprd();
            // matmatecally, real power function is defined for non-negative
            // base only
            if (op1 < 0.0)
                fprintf(stderr, "\tAttempt to raise negative value to a power. "
                                "That's not supported\n");
            else if (op1 == 0.0 && op2 < 0)
                fprintf(stderr, "\tAttempt to raise zero toa negative power, "
                                "that's not supported.\n");
            else
                put_oprd(pow(op1, op2));
            break;
        case Unknown:
            // print to stderr
            fprintf(stderr, "\tGot unknown input!\n");
            break;
        }
    }
end:
    // in the end, print to operand in the stack
    printf("Answer to everything is %f\n", pop_oprd());
}
