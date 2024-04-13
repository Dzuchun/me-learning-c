/* Type of the parsed input */
enum InputType {
    Operand,    // a number, written to the oprd stack already
    Plus,       // '+'
    Minus,      // '-'
    Star,       // '*'
    Slash,      // '/'
    Percent,    // '%'
    Top,        // print 2 top elements
    Dup,        // duplicate top element
    Swp,        // swap 2 top elements
    Clr,        // clears the stack
    Sin,        // sin library function
    Exp,        // exp library function
    Pow,        // pow library function
    Incomplete, // Current line has no more inputs
    Unknown,    // couldn't recognize the input :(
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
