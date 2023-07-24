#include "threading.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

// Optional: use these functions to add debug or error prints to your application
// #define DEBUG_LOG(msg,...)
#define DEBUG_LOG(msg,...) printf("threading: " msg "\n" , ##__VA_ARGS__)
#define ERROR_LOG(msg,...) printf("threading ERROR: " msg "\n" , ##__VA_ARGS__)

void* threadfunc(void* thread_param)
{

    // TODO: wait, obtain mutex, wait, release mutex as described by thread_data structure
    // hint: use a cast like the one below to obtain thread arguments from your parameter
    //struct thread_data* thread_func_args = (struct thread_data *) thread_param;
    // Populate thread_data structure
    struct thread_data* thread_data_s = (struct thread_data*)thread_param;

    // Wait before obtaining mutex
    DEBUG_LOG("Sleeping for %d milliseconds", thread_data_s->w_obtain_ms);
    usleep(thread_data_s->w_obtain_ms * 1000);

    // Obtain mutex
    DEBUG_LOG("Obtaining mutex in thread %ld", thread_data_s->thread_id);
    int rc = pthread_mutex_lock(thread_data_s->mutex);
    if (rc != 0) {
        ERROR_LOG("Failed to lock mutex for thread %ld (%d)! Error %d (%s)", thread_data_s->thread_id, rc, errno, strerror(errno));
        thread_data_s->thread_complete_success = false;
        return thread_data_s;
    }

    // Hold mutex before releasing
    DEBUG_LOG("Holding mutex in thread %ld for %d millicseconds", thread_data_s->thread_id, thread_data_s->w_release_ms);
    usleep(thread_data_s->w_release_ms * 1000);

    // Release mutex
    DEBUG_LOG("Releasing mutex in thread %ld", thread_data_s->thread_id);
    rc = pthread_mutex_unlock(thread_data_s->mutex);
    if (rc != 0) {
        ERROR_LOG("Failed to unlock mutex for thread %ld (%d)! Error %d (%s)", thread_data_s->thread_id, rc, errno, strerror(errno));
        thread_data_s->thread_complete_success = false;
        return thread_data_s;
    }

    // Thread completion successful
    thread_data_s->thread_complete_success = true;
    return thread_data_s;
}


bool start_thread_obtaining_mutex(pthread_t *thread, pthread_mutex_t *mutex,int wait_to_obtain_ms, int wait_to_release_ms)
{
    /**
     * TODO: allocate memory for thread_data, setup mutex and wait arguments, pass thread_data to created thread
     * using threadfunc() as entry point.
     *
     * return true if successful.
     *
     * See implementation details in threading.h file comment block
     */
    // Allocate memory for thread_data dynamically
    struct thread_data* thread_data = malloc(sizeof(thread_data));

    // Set up mutex and wait arguments
    thread_data->mutex = mutex;
    thread_data->thread_id = *thread;
    thread_data->w_obtain_ms = wait_to_obtain_ms;
    thread_data->w_release_ms = wait_to_release_ms;
    thread_data->thread_complete_success = false;

    // Create thread with thread implementation and thread_data
    int rc = pthread_create(thread, NULL, threadfunc, thread_data);
    if (rc != 0) {
        ERROR_LOG("pthread_create() failed with error %d (%s)", errno, strerror(errno));
        free(thread_data);
        return false;
    }
    DEBUG_LOG("Created thread with ID %ld", *thread);
    
    return true;
}

