// This is a code for tasks starting with 4-3 (reversed Polish notation
// calculator)
//
// Hope to implement at least a couple

#include <ctype.h>
#include <math.h>
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

#define MAX_OPERANDS 100
/* This is an operand stack */
double oprds[MAX_OPERANDS];
int oprptr = 0;

/* gets an operand from stack, or warns are returns 42.0, if none is
 * available
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
    Sin,     // stdlib sin
    Exp,     // stdlib exp
    Pow,     // stdlib pow
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
        inpb = i;
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
    inpb = i;
    // make decimal places
    while (decimals--)
        mantice /= 10;
    // return the result
    return mantice;
}

int match_word(int i, const char *word) {
    int c, t = word[0], j;
    for (j = 0; (c = input_get(i)) != -1 && c == t && t != '\0';
         ++i, ++j, t = word[j])
        ;
    if (t != '\0') {
        // match had failed.
        return 0;
    }
    // forget the input until word's last char
    inpb = i;
    return 1;
}

/*
 * Reads next input argument or an operation.
 *
 * If it happens to be a number, it's pushed to the operands stack
 */
enum InputType read_next() {
    int i;
    int c;
    double sign = 1.0;

fetch:
    // first, ask to fetch the input (if necessary)
    input_fetch();
    if (!input_len())
        // input has len 0
        // that actually means that EOF was reached
        return Eof;
    i = inpb;

    // skip the whitespace

    while (isblank((c = input_get(i))))
        ++i;

    if (c == -1) {
        input_clear();
        goto fetch;
    }

    if (c == '*') {
        // that's a star operator!
        inpb = i + 1;
        return Star;
    }

    if (c == '/') {
        // that's a slash operator!
        inpb = i + 1;
        return Slash;
    }

    if (c == '%') {
        // that's a percent operator!
        inpb = i + 1;
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
            inpb = i;
            return Plus;
        }
    } else if (c == '-') {
        c = input_get(++i);
        if (!isdigit(c)) {
            // A minus followed by a non-digit => it's a minus operator!
            inpb = i;
            return Minus;
        }

        // note that minus was encountered
        sign = -1.0;
    } else if (!isdigit(c)) {
        // it's not a number, and not one of the operators
        // meaning, it's unknown input
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

    for (;;) {
        type = read_next();
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
                return -2;
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
        case Sin:
            put_oprd(sin(pop_oprd()));
            break;
        case Exp:
            put_oprd(exp(pop_oprd()));
            break;
        case Pow:
            op2 = pop_oprd();
            op1 = pop_oprd();
            if (op1 < 0.0) {
                fprintf(stderr, "\tPower with negative base is not "
                                "well-defined methmtically\n");
                return -3;
            }
            if (op1 == 0.0 && op2 <= 0.0) {
                fprintf(
                    stderr,
                    "\tPower of 0 with non-positive eponent is not defined\n");
                return -4;
            } else
                put_oprd(pow(op1, op2));
            break;
        case Clr:
            oprptr = 0;
            break;
        case Unknown:
            // notify
            fprintf(stderr, "\tThat was not a correct input\n");
            break;
        }
    }
end:
    // in the end, print to operand in the stack
    printf("Answer to everything is %f\n", pop_oprd());
}
