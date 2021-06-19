#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>

#define NUM_THREADS 5

// definintions
int main(int argc, char *argv[]);
void *custom_thread(void *arg);

int main(int argc, char *argv[])
{
    int i, scope;
    pthread_t tid[NUM_THREADS];
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    //int status = pthread_create(&t_id, &thread_attrib, (void *)&threadRun, thread);
    // Creates threads
    for (i = 0; i < NUM_THREADS; i++)
    {
        pthread_create(&tid[i], &attr, custom_thread, NULL);
    }
    // Joins threads
    for (i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(tid[i], NULL);
    }
}

void *custom_thread(void *arg)
{
    printf("yeet\n");
    pthread_exit(0);
    // printf("I am a Custom Thread Function Created by User");
}