#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

static unsigned long counter;
static bool worker_pause;
pthread_mutex_t worker_pause_mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t has_worker_paused = PTHREAD_COND_INITIALIZER;
pthread_cond_t has_main_thread_finished = PTHREAD_COND_INITIALIZER;

void *thread_work(void *vparg)
{
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    while (true)
    {
        pthread_mutex_lock(&worker_pause_mutex);
        while (worker_pause)
        {
            pthread_cond_signal(&has_worker_paused);
            pthread_cond_wait(&has_main_thread_finished, &worker_pause_mutex);
        }
        pthread_mutex_unlock(&worker_pause_mutex);
        for (int i = 0; i < 1000000000; i++)
        {
            counter++;
        }
    }
}

int main(int argc, char const *argv[])
{
    pthread_t thread_id;
    printf("Before thread\n");
    pthread_create(&thread_id, NULL, thread_work, NULL);
    for (int i = 0; i <= 3; i++)
    {
        pthread_mutex_lock(&worker_pause_mutex);
        worker_pause = true;
        pthread_cond_wait(&has_worker_paused, &worker_pause_mutex);
        printf("Counter: %lu\n", counter);
        worker_pause = false;
        pthread_cond_signal(&has_main_thread_finished);
        pthread_mutex_unlock(&worker_pause_mutex);
        sleep(1);
    }

    pthread_cancel(thread_id);
    pthread_join(thread_id, NULL);
    printf("After thread\n");
    return 0;
}
