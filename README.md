Pthreads Examples
==============
Source: http://www.amazon.com/PThreads-Programming-Standard-Multiprocessing-Nutshell/dp/1565921151

#### Basic Concepts ####
##### Sharing Data ######
* Threads share everything but the stack and registers. They share the Heap and Data (global variables).

Related example: example1-pthreads-creation.c (Threads creation using ```pthread_create``` and ```phthread_join```)


##### Mutexes #####
* Mutexes can be used to protect a shared resource. Here is an example of protecting access to the counter variable: (example2-mutexes.c)

``` 
pthread_mutex_lock(&mutex);
counter++;
printf("job %d started\n", counter);
for (int i = 0; i < 4; i++) {
    // do somework
}
printf("job %d finished\n", counter);
pthread_mutex_unlock(&mutex);
```

Without the mutex lock the output can look like the following:
```
job 1 started
job 2 started
job 2 finished
job 2 finished
```

With the mutex the output will always be: 
```
job 1 started
job 1 finished
job 2 started
job 2 finished
```

##### Thread Models #####
* The boss/worker model: the boss will create threads to handle incoming requests, workers will perform the tasks
* The peer model: all threads work concurrently exchanging data without a manager/boss
* The pipeline model: we have a long stream of input and we have threads to work on each stage of the pipeline, data will be passed from a stage to another


##### Buffering Data Between Threads #####
* Producer Consumer Model:
 * buffer: any data structure accessible to both the producer and the consumer
 * lock: since the buffer is shared, it needs to be synchronized (a mutex can be used for that)
 * suspend/resume mechanism: the consumer may suspend itself when do data is available and same thing for the producer. Condition variables can be used.
 * state information: a state to indicate how much data is in the buffer

related example: example4-producer-consumer.c
Producer model:
```
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
```
Consumer model:
```
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
```
