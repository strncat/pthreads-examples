//
//  main.c
//  pthreads-examples
//
//  Created by Fatima B on 12/10/15.
//  Copyright © 2015 Fatima B. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

int r1 = 0, r2 = 0;

void do_one_thing(int *pnum_times) {
    int x = 0;
    for (int i = 0; i < 4; i++) {
        printf(" doing one thing\n");
        for (int j = 0; j < 10000; j++) {
            x = x + i;
            (* pnum_times)++;
        }
    }
}

void do_another_thing(int *pnum_times) {
    int x = 0;
    for (int i = 0; i < 4; i++) {
        printf(" doing another \n");
        for (int j = 0; j < 10000; j++) {
            x = x + i;
        }
        (* pnum_times)++;
    }
}

void do_wrap_up(int one_times, int another_times) {
    int total = one_times + another_times;
    printf("wrap up: one thing %d, another %d, total %d\n",
           one_times, another_times, total);
}


int main(int argc, const char * argv[]) {
    pthread_t thread1, thread2;

    // the pthread_create() function returns zero
    pthread_create(& thread1,
                   NULL, // a thread attribute object
                   (void *) do_one_thing, // A pointer to the routine at which the new thread will start executing
                   (void *) &r1); // A pointer to a parameter to be passed to the routine at which the new thread starts

    pthread_create(& thread2,
                   NULL,
                   (void *) do_another_thing,
                   (void *) &r2);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    do_wrap_up(r1, r2);

    return 0;
}
