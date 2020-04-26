//
// Created by Lukas Gust on 4/4/2018.
//
#include "threading.h"

threading::threading() {}

/**
 * Spawn a new thread and perform the provided call back.
 * @param call_back action to do on the new thread.
 * @param args arguments for call back.
 * @return error code
 */
int threading::create(void *call_back, void *args) {
    int thrderr = 0;

    thrderr = pthread_create(&this->thrdid, NULL, (void *(*)(void *))call_back, args);
    if(thrderr){
        std::cerr << "Error creating threads..";
        return thrderr;
    } else{
        return 0;
    }
}

/**
 * Join the thread.
 * @return
 */
int threading::join() {
    pthread_join(this->thrdid, NULL);
    return 0;
}

/**
 * Interupt the thread.
 * @return
 */
int threading::cancel() {
    pthread_cancel(this->thrdid);
}

/**
 * Initialize a mutex.
 * @param mtx mutex to initialize.
 * @return error code.
 */
int threading::initialize_mutex(pthread_mutex_t * mtx) {
    if(pthread_mutex_init(mtx, NULL) < 0){
        std::cerr << "Error while initializing server_mutex..";
        return -1;
    }
    else{
        return 0;
    }
}

/**
 * Lock a mutex.
 * @param mtx mutex to lock.
 * @return error code.
 */
int threading::lock_mutex(pthread_mutex_t * mtx) {
    if(pthread_mutex_lock(mtx) == 0){
      //std::cout  << "acquiring lock" << std::endl;
        return 0;
    }
    else
        return -1;
}

/**
 * Unlock a mutex.
 * @param mtx mutex to unlock.
 * @return error code.
 */
int threading::unlock_mutex(pthread_mutex_t * mtx) {
    if(pthread_mutex_unlock(mtx) == 0) {
      //std::cout << "releasing lock" << std::endl;
        return 0;
    }
    else
        return -1;
}
