//
//
// Simple Producer - Consumer Model / using condition variables
//
// A condition variable is used to avoid busy waiting.
// It is an explicit queue that threads can put themselves on when some state of
// execution (i.e., some condition) is not as desired (by waiting on the condition)
//
// A call to pthread_cond_wait requires that a locked mutex be passed in along with
// the condition variable. The system releases the mutex on the caller’s behalf when
// the wait for the condition begins.

// the thread that issues the pthread_cond_signal or pthread_cond_broadcast call holds
// the mutex at the time of the call but must release it after the call. Then,
// when the system wakes it up, a waiting thread can regain control of the mutex.
//

#include <stdio.h>
#include <pthread.h>

pthread_mutex_t mutex;
pthread_cond_t condition_c, condition_p;
int buffer = 0; // global shared variable between the two threads

void* producer(void *ptr) {
    for (int i = 1; i <= 10; i++) {
        pthread_mutex_lock(&mutex);
        while (buffer != 0) {
            pthread_cond_wait(&condition_p, &mutex); // make producer wait
        }
        buffer = i; // add a value to buffer
        printf("produced %d\n", buffer);
        pthread_cond_signal(&condition_c); // wake up consumer
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(0);
}

void* consumer(void *ptr) {
    for (int i = 1; i <= 10; i++) {
        pthread_mutex_lock(&mutex);
        while (buffer == 0) {
            pthread_cond_wait(&condition_c, &mutex); // make consumer wait
        }
        printf("consumed %d\n", buffer);
        buffer = 0; // consumer the value
        pthread_cond_signal(&condition_p); // wake up producer
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(0);
}

int main(int argc, char **argv) {
    pthread_t producer_t, consumer_t;

    // init mutex and condition variables
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&condition_c, NULL);
    pthread_cond_init(&condition_p, NULL);

    // create the threads
    pthread_create(&consumer_t, NULL, consumer, NULL);
    pthread_create(&producer_t, NULL, producer, NULL);

    // wait for both consumer and producer to finish
    pthread_join(consumer_t, NULL);
    pthread_join(producer_t, NULL);

    // destroy mutex and condition variables
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&condition_c);
    pthread_cond_destroy(&condition_p);
}
