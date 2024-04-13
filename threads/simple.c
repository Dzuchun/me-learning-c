
#include <pthread.h>

#include <stdio.h>
void *greet(void *no) {
    fprintf(stdout, "Hello, thread no %d\n", *(int *)no);
    return NULL;
}

#define NUM_OF_THREADS 10
int main() {
    pthread_t threads[NUM_OF_THREADS];
    int i, j;
    for (i = 0; i < NUM_OF_THREADS; ++i) {
        j = i;
        pthread_create(threads + i, NULL, greet, &j);
    }
    for (i = 0; i < NUM_OF_THREADS; ++i)
        pthread_join(threads[i], NULL);
}
