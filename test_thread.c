#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>


void *thread_work(void *vparg)
{
    return (void *) 5;
}

int main(int argc, char const *argv[])
{
    pthread_t thread_id;
    printf("Before thread\n");
    pthread_create(&thread_id, NULL, thread_work, NULL);
    void *value;
    pthread_join(thread_id, &value);
    printf("%ld\n", (__intptr_t) value);
    printf("After thread\n");
    return 0;
}
