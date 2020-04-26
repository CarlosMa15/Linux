//
// Created by Lukas Gust on 4/4/2018.
//

#ifndef COLLABSPREADSHEET_MY_THREAD_H
#define COLLABSPREADSHEET_MY_THREAD_H

#include <pthread.h>
#include <unistd.h>
#include <iostream>

class threading{
public:
    pthread_t thrdid;

private:
    static pthread_mutex_t mutex;

public:
    threading();
    int create(void* call_back, void* args);
    int join();
    int cancel();

    static int initialize_mutex(pthread_mutex_t * mtx);
    static int lock_mutex(pthread_mutex_t * mtx);
    static int unlock_mutex(pthread_mutex_t * mtx);
};
#endif //COLLABSPREADSHEET_MY_THREAD_H
