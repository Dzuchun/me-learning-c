#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <threads.h>

struct timespec SLEEPING_TIME;

struct seller_args {
    int no;
    int _Atomic *tickets;
    int _Atomic *sold;
};
typedef struct seller_args SellerArgs;

void *seller(void *a) {
    SellerArgs *args = a;
    int _Atomic local;
    for (;;) {
        local = atomic_fetch_sub(args->tickets, 1);
        if (!local) {
            // all tickets WERE sold. need to increment value back by 1
            atomic_fetch_add(args->tickets, 1);
            break;
        }
        printf("%d: trying to sell a ticket...\n", args->no);
        thrd_sleep(&SLEEPING_TIME, NULL); // sleeping ain't gonna help you sell
                                          // tickets, you silly thread!
        if (rand() > (RAND_MAX / 2)) {
            // oh wow. you've succeeded anyway. good jod, I guess?
            printf("%d: ticket bought!\n", args->no);
            atomic_fetch_add(args->sold, 1);
        } else {
            printf("%d: customer refused to buy a ticket :(\n", args->no);
            // increment ticket count
            atomic_fetch_add(args->tickets, 1);
        }
    }
    printf("%d: no more to sell, I die now\n", args->no);
    return NULL;
}

#define NUM_OF_AGENTS 100
#define NUM_OF_TICKETS 200000
int main() {
    // set sleeping SLEEPING_TIME
    SLEEPING_TIME.tv_sec = 0;
    SLEEPING_TIME.tv_nsec = 100E4;
    int i;
    int _Atomic tickets = NUM_OF_TICKETS;
    int _Atomic sold = 0; // this counter SPECIFICALLY will only count atomic
                          // increments to make sure that no error happened
    pthread_t agents[NUM_OF_AGENTS];
    SellerArgs args[NUM_OF_AGENTS];
    for (i = 0; i < NUM_OF_AGENTS; ++i) {
        args[i].sold = &sold;
        args[i].tickets = &tickets;
        args[i].no = i;
        pthread_create(agents + i, NULL, seller, args + i);
    }
    for (i = 0; i < NUM_OF_AGENTS; ++i)
        pthread_join(agents[i], NULL);
    printf("Sold: (%d/%d)", sold, NUM_OF_TICKETS);
}
