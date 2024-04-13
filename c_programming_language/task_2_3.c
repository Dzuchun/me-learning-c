#include <ctype.h>
#include <stdio.h>
#include <strings.h>

#define BUF_LEN 100

enum HtoiError {
    // There were no error!
    NoError = 0,
    // Indicates that hex number was not found in the buffer
    NotFound = 1,
    // Indicates that hex number was too large
    TooLarge = 2,
    // Panic occured, whatever that means
    Panic,
};

enum HtoiError ERROR = NoError;

/*
 * Searches for plus of minus sign, skipping possible blank space
 *
 * Returns index where the sign was found
 *
 * If no sign was found, returns negative value
 */
int parse_sign(char buf[], int index);

/*
 * Searches for "0x" or "0X" preceeded by optional whitespace
 *
 * Returns index where the combination was found
 *
 * If combination was not found, returns negative value
 */
int parse_hex_start(char buf[], int index);

/*
 * Searches for hexadecimal digits in the buffer, starting from supplied index
 *
 * Returns number of digits present
 */
int parse_hex_digits(char buf[], int index);

enum Sign { None, Plus, Minus };

/*
 * Parses a hexadecimal integer from supplied string. Integer may be prepended
 * by one or more empty spaces.
 *
 * Integer can be prepended with an optional plus or minus sign.
 *
 * Integer should start from either "0x" or "0X". After that, hexadecimal
 * charaters are parsed, that is [0-9a-fA-F].
 *
 * If number was not found in the buffer, error will be triggered
 *
 * This function must return [int], so if supplied number is bigger that
 * [INT_MAX] or less than [INT_MIN], error will be triggered
 *
 * To check if error occured during evaluation, see [ERROR] variable.
 */
int htoi(char buf[]) {
    int idx = 0, tmp, i, digit;
    unsigned int result;
    char c;
    enum Sign sign;

    ERROR = NoError;
    // first, try finding a sign
    tmp = parse_sign(buf, idx);
    if (tmp < 0)
        // if there's no sign -- that's ok
        sign = None;
    else {
        if (buf[tmp] == '+')
            sign = Plus;
        else
            sign = Minus;
        // move buf pointer
        idx = tmp + 1;
    }

    // next, try finding hex number start ("0x" or "0X")
    tmp = parse_hex_start(buf, idx);
    if (tmp < 0) {
        // that's not ok - there is no hex number!
        ERROR = NotFound;
        return result; // RETURN GARBAGE NYEH-HEH-HEH
    } else
        idx = tmp + 2; // start is two charaters long

    // next, check how many digits there are
    tmp = parse_hex_digits(buf, idx);
    if (tmp == 0) {
        // that's not ok - there's seemingly a hex number, but it's missing...
        // digits.
        ERROR = NotFound;
        return result; // GARBAGE AGAIN :strong:
    }
    if (tmp / 2 > sizeof(int)) {
        // that's not ok - this number can potentially be too large (two digits
        // encode a single byte)
        ERROR = TooLarge;
        return result; // GAR... argh
    }

    // lastly, construct the answer
    result = 0;
    tmp = idx + tmp; // denotes index at the end of iteration
    for (i = idx; i < tmp; ++i) {
        c = buf[i];
        if ('0' <= c && c <= '9') {
            digit = c - '0';
        } else if ('a' <= c && c <= 'f') {
            digit = c - 'a' + 10;
        } else if ('A' <= c && c <= 'F') {
            digit = c - 'A' + 10;
        } else {
            ERROR = Panic;
            return result;
        }
        result = result * 16 + digit;
    }

    // in the end, apply the sign (of needed)
    if (sign == Minus)
        result *= -1;

    return result;
}

#define MAGIC_EOF_CODE -BUF_LEN - 1

/*
 * Reads line from `stdin` into buffer, returning number of characters read
 *
 * If EOF is encountered, while some characters already read, read stops and
 * negative value is returned. If EOF is encountered as first character,
 * [MAGIC_EOF_CODE] is returned.
 *
 * Read stops at '\n' (newline). newline itself is now included into the buffer
 */
int read_line(char buf[]) {
    int i = 0, c;
    // one char is reserved for '\0'
    for (; i < BUF_LEN - 1 && (c = getchar()) != EOF && c != '\n'; ++i)
        buf[i] = c;
    buf[i] = '\0';
    if (c == EOF) {
        if (i > 0)
            return -i;
        else
            return MAGIC_EOF_CODE;
    }
    return i;
}

int main() {
    int len, parsed;
    char buf[BUF_LEN];
    do {
        len = read_line(buf);
        if (len == MAGIC_EOF_CODE)
            break;

        parsed = htoi(buf);
        printf("Parsed int: %d; ", parsed);
        if (ERROR == NoError)
            printf("no errors occured\n");
        else
            printf("ERROR: %d\n", ERROR);
    } while (len >= 0);
}

int parse_sign(char buf[], int index) {
    int c;
    for (; index < BUF_LEN && (c = buf[index]) != '\0'; ++index) {
        if (isblank(c))
            // empty spaces are permitted at this point
            continue;

        if (c == '+' || c == '-')
            // we've found a sign!
            return index;

        // at this point, it's not a blank and not a sign
        // meaning, the're no combinations we're looking for :(
        break; // effectively returns -1, indicating failure
    }
    return -1;
}

int parse_hex_start(char buf[], int index) {
    int c, cf;
    for (; index < BUF_LEN - 1 /* Combination is 2 chars long, meaning iteration
                                  should stop one char before */
           && (c = buf[index]) != '\0';
         ++index) {
        if (isblank(c))
            // empty spaces are ok and skipped
            continue;

        cf = buf[index + 1];
        if (c == '0' && (cf == 'x' || cf == 'X')) {
            // we've found the combination!
            return index;
        }

        // at this point, it's not a blank and not a hex start
        // meaning, there's no complying sequence
        break; // effectively return -1
    }
    return -1;
}

int parse_hex_digits(char buf[], int index) {
    int c, len = 0;
    while (index < BUF_LEN && (c = buf[index]) != '\0' && isxdigit(c)) {
        ++index;
        ++len;
    }
    return len;
}
