#include <ctype.h>
#include <stdio.h>
#include <string.h>

/*
 * Design choice: I'm afraid of dynamic allocations for now, so I'll use regular
 * arrays for now.
 */

// I'll use shared text storage for all words
#define STR_BUF_SIZE 5000
char STR_BUF[STR_BUF_SIZE];
// Pointer to a next free space at strbuf
char *sbufp = STR_BUF;

// Stores string to a shared text storage, returning pointer to it's start
//
// It there's not enough space, returns  NULL
char *store_str(char *s) {
    char c, *p = sbufp;
    // Remember amount of available bytes
    int free_space = STR_BUF_SIZE - (p - STR_BUF);
    // iterate while
    // - there are free bytes
    // - character is not '\0'
    while (free_space-- && (c = *s++) != '\0')
        *p++ = c;
    // at this point, we are either:
    // - out of space
    // - at the end of word
    if (!free_space)
        // if we are out of space, that's bad and we return NULL
        return NULL;
    // at this point, we've written the entire string except for NULL-byte at
    // the end
    *p++ = '\0'; // white NULL-byte at the end
    // the result should be current value of sbuf
    char *res = sbufp;
    sbufp = p; // update available space
    return res;
}

// Tree node definition
struct tnode {
    char *word;
    int count;
    struct tnode *lchild;
    struct tnode *rchild;
};

// I'll also use shared tree node storage for all nodes!
#define MAX_WORDS 500
struct tnode NODE_BUF[MAX_WORDS];
// pointer to next free space at node buf
struct tnode *nbufp = NODE_BUF;

// Stores word to a node buffer, returning pointer to corresponding node
//
// If there's not enough space to store node or a word, returns NULL
struct tnode *store_word(char *s) {
    struct tnode *res;
    char *word;
    if (nbufp - NODE_BUF >= MAX_WORDS || (word = store_str(s)) == NULL)
        // if there's not enough space, return NULL
        return NULL;
    res = nbufp++;
    // is there a better way to do that?
    (*res).word = word;
    (*res).count = 0;
    (*res).lchild = NULL;
    (*res).rchild = NULL;
    return res;
}

struct word_record {
    char *word;
    int count;
};

// you didn't this I'm gonna force nested recursion, did you?
struct tnode *recursion_buffer[MAX_WORDS];

// Collects the entire tree into a specified buffer as word records
//
// Returns number of words collected
//
// Assumes there's enough space to accomodate all the words
int collect_tree(struct word_record *arr) {
    // we'll always assume that tree root is at NODE_BUF[0] (if there's a root
    // at all)
    if (nbufp == NODE_BUF)
        // well, there isn't a tree here
        // (nothing to do)
        return 0;
    struct word_record *wrp = arr, tmp;
    struct tnode **stackp = recursion_buffer, *current_node, *tmp2;
    // add a tree root to the stack
    *stackp++ = NODE_BUF;
    // while stack is not empty,
    while (stackp != recursion_buffer) {
        // pop last node in the stack
        current_node = *--stackp;
        // record this node
        tmp.word = current_node->word;
        tmp.count = current_node->count;
        *wrp++ = tmp;
        // add children to stack (if present)
        if ((tmp2 = current_node->rchild) != NULL)
            *stackp++ = tmp2;
        if ((tmp2 = current_node->lchild) != NULL)
            *stackp++ = tmp2;
    }
    return wrp - arr;
}

// Finds this word's node in the tree, or insert one in corret place
//
// Returns NULL on any error
struct tnode *get_node(char *word) {
    if (nbufp == NODE_BUF)
        // well, there it's a tree here
        return store_word(word);
    struct tnode *current, *parent = NULL;
    int cmp_res;
    // always assume that tree starts from NODE_BUF[0]
    current = NODE_BUF;
    while (current != NULL) {
        // compare word (order does not matter, really)
        cmp_res = strcmp(word, current->word);
        if (cmp_res == 0)
            // that's a match, we've found the node
            return current;
        // save this node to parent
        parent = current;
        // advance in direction depending on the comparision result
        // (the exact direction does not matter, as long as it's consistent)
        current = (cmp_res > 0) ? parent->lchild : parent->rchild;
    }
    // at this point, current points to NULL, and parent points to it's parent
    // we must add a new node
    current = store_word(word);
    // bind parrent to it
    if (cmp_res > 0)
        parent->lchild = current;
    else
        parent->rchild = current;
    // return newly-created node (or NULL, for that matter)
    return current;
}

// Increments number of occurences of a word by 1
//
// Returns negative value on error
int bump_word(char *word) {
    // printf("Bumping word: %s\n", word);
    struct tnode *node = get_node(word);
    if (node == NULL)
        return -1;
    ++node->count;
    return 0;
}

#define swap(t, a, b)                                                          \
    {                                                                          \
        t tmp;                                                                 \
        tmp = a;                                                               \
        a = b;                                                                 \
        b = tmp;                                                               \
    }

// copied from task_5_7
void sort_recods(struct word_record *buf, int left, int right) {
    int i, last;
    if (left >= right)
        return;
    last = (left + right) / 2;
    swap(struct word_record, buf[left], buf[last]);
    last = left;
    for (i = left + 1; i <= right; ++i)
        if (buf[i].count > buf[left].count) {
            ++last;
            swap(struct word_record, buf[last], buf[i]);
        }

    swap(struct word_record, buf[left], buf[last]);
    sort_recods(buf, left, last - 1);
    sort_recods(buf, last + 1, right);
}

// Reads next word from file
//
// Propagates EOF, if no word was read
//
// A sequence of non-whitespace characters is considered a word
int get_word(FILE *file, char *buf, int limit) {
    if (limit < 1)
        // there's nothing to do (we need to write at least NULL-byte)
        // (but it's not an error)
        return 0;
    int c;
    // first, skip are blank space chars
    while ((isblank(c = fgetc(file)) || c == '\n') && c != EOF)
        ;
    // if EOF was encountered at this point, propagate it
    if (c == EOF)
        return EOF;
    // at this point, there are characters and c contains first one
    *buf++ = c;
    --limit; // so that there would be a place for NULL-byte
    while (--limit && !isblank(c = fgetc(file)) && (c != '\n') && (c != EOF))
        *buf++ = c;
    // at this point, there's either
    // - an EOF hit (but it's ok, as we've read at least one character)
    // - a black character hit (ok too)
    // - we're out of space and must round up
    // none of these scenario need no special treatment, so we can proceed
    *buf = '\0';
    return 0;
}

#define MAX_WORD_LEN 100
int main() {

    char word[MAX_WORD_LEN];
    while (get_word(stdin, word, MAX_WORD_LEN) != EOF)
        bump_word(word);
    struct word_record buf[MAX_WORDS];
    int word_count = collect_tree(buf);
    sort_recods(buf, 0, word_count - 1);
    int max_lengh = 1, len;
    for (int i = 0; i < word_count; ++i)
        if ((len = strlen(buf[i].word)) > max_lengh)
            max_lengh = len;
    for (int i = 0; i < word_count; ++i)
        printf("%*s ... %d\n", max_lengh, buf[i].word, buf[i].count);
}
