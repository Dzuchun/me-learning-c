#include <stdio.h>
#include <wchar.h>

#define MAX_LEN 200

int read_string(int buf[]) {
    int c, i;
    i = 0;
    while ((c = getchar()) != EOF && c != '\n') {
        buf[i] = c;
        if (++i == MAX_LEN - 1)
            break;
    }
    buf[i] = '\0';
    if (c == EOF)
        return -1;
    return i;
}

void /* that's a comment! */ print_string(int buf[], int start_index,
                                          /* here's one more!
                                           */
                                          int end_index) {
    int i, c;
    i = start_index;
    if (end_index < 0)
        end_index = MAX_LEN - 1;
    while ((c = buf[i]) != '\0' && i < end_index) {
        putchar(c);
        i += 1;
    }
}

#define ESCAPED 1
#define NOT_ESCAPED 0
#define SINGLE_QUOTE 1
#define DOUBLE_QUOTE 2
#define NO_QUOTE 0

int has_unquoted_sequence(int buf[], int start_index, int first_char,
                          int second_char) {
    int i, c, escape_flag, quote_flag;
    escape_flag = NOT_ESCAPED;
    quote_flag = NO_QUOTE;
    for (i = start_index; i < MAX_LEN && (c = buf[i]) != '\0'; ++i) {
        // if there's was a backslash at previous char, it's not a comment
        // start, that's for sure.
        if (escape_flag == ESCAPED) {
            escape_flag = NOT_ESCAPED;
            continue;
        }
        // if we're inside quotes and encounted backslash - trigger escape_flag
        if (quote_flag != NO_QUOTE && c == '\\') {
            escape_flag = ESCAPED;
            continue;
        }
        // escapes are handled.

        if (quote_flag == NO_QUOTE) {
            if (c == '\'') {
                // single quote-thing start
                quote_flag = SINGLE_QUOTE;
                continue;
            } else if (c == '\"') {
                // double quote-thing start
                quote_flag = DOUBLE_QUOTE;
                continue;
            }
        } else {
            // comments can't start within quote
            if (quote_flag == SINGLE_QUOTE && c == '\'') {
                // single quote escape
                quote_flag = NO_QUOTE;
                continue;
            } else if (quote_flag == DOUBLE_QUOTE && c == '\"') {
                // double quote escape
                quote_flag = NO_QUOTE;
                continue;
            }

            continue;
        }
        // quotes are handled.

        if (c == first_char && buf[i + 1] == second_char) {
            // we've found it!
            return i;
        }
    }

    return -1;
}

/* Finds regular comment, defined by two slashes
 *
 * Returns index slashes start at, or -1 if there's no comment
 * */
int has_regular_comment(int buf[], int start_index) {
    return has_unquoted_sequence(buf, start_index, '/', '/');
}

int has_block_comment_start(int buf[], int start_index) {
    return has_unquoted_sequence(buf, start_index, '/', '*');
}

int has_block_comment_end(int buf[], int start_index) {
    int i, c;
    for (i = start_index; i <= MAX_LEN - 1 && (c = buf[i]) != '\0'; ++i) {
        if (buf[i] == '*' && buf[i + 1] == '/')
            return i;
    }
    return -1;
}

#define BLOCK_COMMENT_TRUE 1
#define BLOCK_COMMENT_FALSE 0

int main() {
    int len, active_block_comment, start_index, idx;
    int buf[MAX_LEN];
    active_block_comment = BLOCK_COMMENT_FALSE;
    while ((len = read_string(buf)) >= 0) {
        start_index = 0;
        if (active_block_comment == BLOCK_COMMENT_TRUE) {
            // we're in a block comment now
            idx = has_block_comment_end(buf, start_index);
            if (idx < 0)
                // the block keeps going; skip the line
                continue;
            // set the start to charater after block comment ends
            active_block_comment = BLOCK_COMMENT_FALSE;
            start_index = idx + 2;
        }
        while (active_block_comment == BLOCK_COMMENT_FALSE) {
            // try parse regular comment
            idx = has_regular_comment(buf, start_index);
            if (idx >= 0) {
                // there is a comment in the line!
                // modify it to end right before the comment
                buf[idx] = '\0';
                // break, since all of the line is guaranteed to be a comment
                // after that
                break;
            }
            // try parse block comment start
            idx = has_block_comment_start(buf, start_index);
            if (idx >= 0) {
                // there's a block comment beginning
                // write uncommented part
                print_string(buf, start_index, idx);
                start_index = idx + 2; // start index after comment had started
                active_block_comment = BLOCK_COMMENT_TRUE;
            } else
                break;
            if (active_block_comment == BLOCK_COMMENT_TRUE) {
                // if there's an active block comment, try to end int
                idx = has_block_comment_end(buf, start_index);
                if (idx >= 0) {
                    // block comment had ended!
                    start_index = idx + 2; // start after comment end
                    active_block_comment = BLOCK_COMMENT_FALSE;
                }
            }
        }
        if (active_block_comment == BLOCK_COMMENT_FALSE)
            print_string(buf, start_index, -1);
        putchar('\n');
    }
}
