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


##### Condition Variables #####
* A condition variable is used to avoid busy waiting. It is an explicit queue that threads can put themselves on when waiting on a condition. "While a mutex lets threads synchronize by controlling their access to data, a condition variable lets threads synchronize on the value of data."

related example: example4-producer-consumer.c

consumer waits until the buffer isn't empty using a condition variable:
``` 
pthread_mutex_lock(&mutex);
while (buffer == 0) {
    pthread_cond_wait(&condition_c_, &mutex); // make consumer wait
}
// do some work
pthread_cond_signal(&condition_p);
pthread_mutex_unlock(&mutex);
```

producer waits until the buffer is empty to add more data:
```
pthread_mutex_lock(&mutex);
while (buffer != 0) {
    pthread_cond_wait(&condition_p, &mutex); // make producer wait
}
// do some work
pthread_cond_signal(&condition_c); // wake up consumer
pthread_mutex_unlock(&mutex);
}
```

* A call to pthread_cond_wait requires that a locked mutex be passed in along with the condition variable. The system releases the mutex on the caller’s behalf when the wait for the condition begins.

* the thread that issues the pthread_cond_signal or pthread_cond_broadcast call holds the mutex at the time of the call but must release it after the call. Then, when the system wakes it up, a waiting thread can regain control of the mutex.

* pthread_cond_broadcast awakens all threads while pthread_cond_signal awakens one thread. If all threads were awaken in the first case, the system still will select one thread to give back the mutex lock to. The other threads are moved to the queue of threads that are waiting to acquire the mutex. 😱


##### Buffering Data Between Threads #####
* Producer Consumer Model:
 * buffer: any data structure accessible to both the producer and the consumer
 * lock: since the buffer is shared, it needs to be synchronized (a mutex can be used for that)
 * suspend/resume mechanism: the consumer may suspend itself when do data is available and same thing for the producer. Condition variables can be used.
 * state information: a state to indicate how much data is in the buffer

related example: example4-producer-consumer.c
