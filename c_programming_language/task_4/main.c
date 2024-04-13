// This is a code for tasks starting with 4-3 (reversed Polish notation
// calculator)
//
// Hope to implement at least a couple

#include "calc.h" // my very first header! yay!
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

// Matches a double literal, returning pointer past it's end in the line
//
// If double had failed to be matched, reutrns NULL
char *match_double(char *line, double *out) {
    int decimals = 0, sign = 1.0;
    char c = *line;
    // if that's the end -- well, there's not a double here
    if (c == '\0')
        return NULL;
    // first, try parsing leading + or -
    c = *line;
    if (c == '+')
        // it's ok, there's nothing to do, just move the pointer
        c = *++line;
    else if (c == '-') {
        // it's ok, set the sign and move the pointer
        sign = -1.0;
        c = *++line;
    }
    // if c is not a digit now, it's a garbage, return NULL
    if (!isdigit(c))
        return NULL;
    // iterate through the digits and update the out variable
    *out = 0.0;
    while (isdigit(c)) {
        *out = *out * 10 + (c - '0');
        c = *++line;
    }
    // at this point, c is NOT a digit, and line points to it
    if (c != '.') {
        // k, we're done here
        // return pointer to THIS unexpected char
        return line;
    }
    // decimal point was encountered
    // step past it
    c = *++line;
    // iterate through decimal part
    while (isdigit(c)) {
        *out = *out * 10 + (c - '0');
        decimals += 1;
        c = *++line;
    }
    // at this point, c is not a digit and line points to it
    // line will be the result. but first, let's properly communicate fractional
    // part:
    while (decimals--)
        *out /= 10;
    // return the result
    return line;
}

#define MATCH_CH(line, c, r)                                                   \
    if (**line == c) {                                                         \
        ++*line;                                                               \
        return r;                                                              \
    }

#define MATCH_STR(line, pat, r)                                                \
    {                                                                          \
        char *tmp_line = *line, c, t, *tmp_pat = pat;                          \
        while ((c = *tmp_line++) == (t = *tmp_pat++) && t != '\0')             \
            ;                                                                  \
        if (t == '\0') {                                                       \
            *line = tmp_line - 1;                                              \
            return r;                                                          \
        }                                                                      \
    }

/*
 * Reads next input argument or an operation.
 *
 * If it happens to be a number, it's pushed to the operands stack
 */
enum InputType read_next(char **line) {
    char *tmp, c;
    // skip the whitespace

    while (isblank(c = **line) && c != '\0')
        ++*line;

    if (c == '\0' || c == '\n')
        // we've hit the end, ask for more data
        return Incomplete;
    // at this point, c is a fist non-whitespace char

    // try matching double
    double d;
    if ((tmp = match_double(*line, &d)) != NULL) {
        // that's match
        // move pointer past the number
        *line = tmp;
        // insert numper as operand
        oprd_put(d);
        // return that we've parsed an operand
        return Operand;
    }

    // try match single-character commands
    MATCH_CH(line, '*', Star);
    MATCH_CH(line, '/', Slash);
    MATCH_CH(line, '%', Percent);
    MATCH_CH(line, '+', Plus);
    MATCH_CH(line, '-', Minus);

    MATCH_STR(line, "top", Top);
    MATCH_STR(line, "dup", Dup);
    MATCH_STR(line, "swp", Swp);
    MATCH_STR(line, "clr", Clr);
    MATCH_STR(line, "sin", Sin);
    MATCH_STR(line, "exp", Exp);
    MATCH_STR(line, "pow", Pow);

    // If all of the matches had failed - well, that's something weird
    return Unknown;
}

#define MAX_LINE_LEN 100

int main() {
    enum InputType type;
    double op1, op2, sin(double), exp(double), pow(double, double);
    char buf[MAX_LINE_LEN], *bufp;
read:
    while (fgets(buf, MAX_LINE_LEN, stdin) != NULL) {
        bufp = buf;
        for (;;) {
            type = read_next(&bufp);
            switch (type) {
            case Unknown:
                // print to stderr
                fprintf(stderr, "\tGot unknown input: %s\n", bufp);
            case Incomplete:
                goto read;
            case Operand:
                // do nothing;
                // operand was already added to the stack
                break;
            case Plus:
                // perform addition
                op2 = oprd_pop();
                op1 = oprd_pop();
                oprd_put(op1 + op2);
                break;
            case Minus:
                // perform subtraction
                op2 = oprd_pop();
                op1 = oprd_pop();
                oprd_put(op1 - op2);
                break;
            case Star:
                // perform multiplication
                op2 = oprd_pop();
                op1 = oprd_pop();
                oprd_put(op1 * op2);
                break;
            case Slash:
                // perform division
                op2 = oprd_pop();
                if (op2 == 0.0) {
                    // No allowed
                    fprintf(
                        stderr,
                        "\tAttempt to divide by number of my brain cells rn\n");
                    return -1;
                }
                op1 = oprd_pop();
                oprd_put(op1 / op2);
                break;
            case Percent:
                // perform modulo
                op2 = oprd_pop();
                if (op2 == 0.0) {
                    fprintf(stderr, "\tCan't take modulo by 0\n");
                    return -2;
                }
                op1 = oprd_pop();
                oprd_put((double)((int)op1 % (int)op2));
                break;
            case Top:
                // reaction depends on number of elements in the stack
                switch (oprd_len()) {
                case 0:
                    // there are no elements
                    // complete disaster
                    fprintf(stdout, "There are no operands in the stack\n");
                    break;
                case 1:
                    // there's only one element
                    fprintf(stdout,
                            "There's a single operand in the stack: %f\n",
                            nth_oprd(0));
                    break;
                default:
                    // there are at least 2 elements!
                    fprintf(stdout,
                            "Last operands in the stack: [.., %f, %f]\n",
                            nth_oprd(1), nth_oprd(0));
                    break;
                }
                break;
            case Swp:
                if (oprd_len() < 2)
                    fprintf(stdout, "Can't swp top operands: there are 0 or 1 "
                                    "operands in the stack\n");
                else {
                    op2 = oprd_pop();
                    op1 = oprd_pop();
                    oprd_put(op2);
                    oprd_put(op1);
                }
                break;
            case Dup:
                if (oprd_len() < 1)
                    fprintf(stdout, "Can't dup top operand, as there are no "
                                    "operands in the stack");
                else
                    oprd_put(nth_oprd(0));
                break;
            case Sin:
                oprd_put(sin(oprd_pop()));
                break;
            case Exp:
                oprd_put(exp(oprd_pop()));
                break;
            case Pow:
                op2 = oprd_pop();
                op1 = oprd_pop();
                if (op1 < 0.0) {
                    fprintf(stderr, "\tPower with negative base is not "
                                    "well-defined methmtically\n");
                    return -3;
                }
                if (op1 == 0.0 && op2 <= 0.0) {
                    fprintf(stderr, "\tPower of 0 with non-positive eponent is "
                                    "not defined\n");
                    return -4;
                } else
                    oprd_put(pow(op1, op2));
                break;
            case Clr:
                oprd_clear();
                break;
            }
        }
    }
    // in the end, print top operand in the stack
    printf("Answer to everything is %f\n", oprd_pop());
}
