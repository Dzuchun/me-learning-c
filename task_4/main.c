// This is a code for tasks starting with 4-3 (reversed Polish notation
// calculator)
//
// Hope to implement at least a couple

#include "calc.h" // my very first header! yay!
#include <ctype.h>
#include <math.h>
#include <stdio.h>

double parse_double(int i) {
    int decimals = 0, c = input_get(i);
    double mantice = 0.0;
    while (isdigit(c)) {
        mantice = mantice * 10 + (c - '0');
        c = input_get(++i);
    }
    if (c != '.') {
        // forget input up to this point
        forget_to(i);
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
    forget_to(i);
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
    forget_to(i);

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
    i = input_begin();

    // skip the whitespace

    while (isblank((c = input_get(i))))
        ++i;

    if (c == -1) {
        input_clear();
        goto fetch;
    }

    if (c == '*') {
        // that's a star operator!
        forget_to(i + 1);
        return Star;
    }

    if (c == '/') {
        // that's a slash operator!
        forget_to(i + 1);
        return Slash;
    }

    if (c == '%') {
        // that's a percent operator!
        forget_to(i + 1);
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
            forget_to(i);
            return Plus;
        }
    } else if (c == '-') {
        c = input_get(++i);
        if (!isdigit(c)) {
            // A minus followed by a non-digit => it's a minus operator!
            forget_to(i);
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
    oprd_put(parse_double(i) * sign);
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
            printf("Operand: %f\n", oprd_pop());
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
                fprintf(stderr,
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
                fprintf(stdout, "There's a single operand in the stack: %f\n",
                        nth_oprd(0));
                break;
            default:
                // there are at least 2 elements!
                fprintf(stdout, "Last operands in the stack: [.., %f, %f]\n",
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
                fprintf(
                    stderr,
                    "\tPower of 0 with non-positive eponent is not defined\n");
                return -4;
            } else
                oprd_put(pow(op1, op2));
            break;
        case Clr:
            oprd_clear();
            break;
        case Unknown:
            // print to stderr
            fprintf(stderr, "\tGot unknown input!\n");
            break;
        }
    }
end:
    // in the end, print to operand in the stack
    printf("Answer to everything is %f\n", oprd_pop());
}
