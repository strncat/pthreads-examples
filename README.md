PThreads Examples
==============
Source: http://www.amazon.com/PThreads-Programming-Standard-Multiprocessing-Nutshell/dp/1565921151

#### Example1: example1-pthreads-creation.c ####
* Simple threads creation using ```pthread_create``` and ```phthread_join```
* Threads share everything but the stack and registers. They share the Heap and Data (global variables)

#### Example2: example2-mutexes.c ####
* Mutexes can be used to protect a shared resource. Here is an example of protecting access to the counter variable:

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

#### Example3 ####
* 
