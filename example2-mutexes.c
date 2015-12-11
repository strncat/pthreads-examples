//
// Mutexes (Mutual Exclusion)
// Serializes access to some parts of code
//
//

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

int r1 = 0, r2 = 0, counter = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void do_one_thing() {
    // The mutex variable acts like a lock protecting access to the shared resource: r3
    pthread_mutex_lock(&mutex);
    counter++;
    printf("job %d started\n", counter);
    for (int i = 0; i < 4; i++) {
        printf(" doing one thing\n");
        for (int j = 0; j < 4000; j++) { }
    }
    printf("job %d finished\n", counter);
    pthread_mutex_unlock(&mutex);
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
                   (void *) do_one_thing,
                   (void *) &r2);

    // without waiting, one output was: wrap up: one thing 0, another 0,  ...
    pthread_join(thread1, NULL); // the parent will wait until thread 1 is done
    pthread_join(thread2, NULL); // the parent will wait until thread 2 is done
    return 0;
}
