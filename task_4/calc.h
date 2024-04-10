/*
 * Returns length of the input buffer that is yet to be parsed
 *
 * If this function returns 0 after [input_fetch] call, it means that input file
 * had hit EOF
 */
int input_len();
/*
 * Clears the input buffer, discarding any unmatched chars
 */
void input_clear();
/*
 * Fetches next line from input, if necessary
 */
void input_fetch();
/*
 * Returns input buffer at position
 *
 * If supplied position is invalid, -1 is returned instead
 */
int input_get(int i);
/* Returns first valid begin index */
int input_begin();

/*
 * Forgets input buffer up to specified position
 *
 * If position is less than current begin position, nothing is done
 *
 * If position in greter than the end position, it's set to the end position
 */
void forget_to(int new_bp);

/* Type of the parsed input */
enum InputType {
    Operand, // a number, written to the oprd stack already
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

/*
 * Returns last operand in the operand stack
 */
double oprd_pop();

/*
 * Puts provided operand at the top of the stack
 */
void oprd_put(double oprd);

/*
 * Returns number of operands in stack
 */
int oprd_len();

/*
 * Returns nth operand on stack from top
 *
 * If there are less than n operands present, returns garbage
 *
 * 0th element is considered top of the stack
 */
double nth_oprd(int n);

/*
 * Clears operands stack, discarding all previously-present elements
 */
void oprd_clear();
