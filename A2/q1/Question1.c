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

// Yes we do notice ambiguity in the order of the printed statments of the output of the program
int main(int argc, char *argv[])
{

    int i;
    pthread_t tid;
    pthread_attr_t attr;
    int status;
    //Causes seg fault without this line
    status = pthread_attr_init(&attr);

    if (status != 0)
    {
        printf("error");
        return -1;
    }
    printf("This program would create threads\n");
    //int status = pthread_create(&t_id, &thread_attrib, (void *)&threadRun, thread);
    /**
     * Creates threads
     * Declaring a variable to create threads  avoids duplication of printf within the if statement
     * */
    status = pthread_create(&tid, &attr, custom_thread, NULL);
    sleep(1);

    //    for (i = 0; i < NUM_THREADS; i++)
    // {
    //     pthread_join(tid[i], NULL);
    // }

    if (status == 0)
    {
        printf("Custom thread created successfully\n");

        for (i = 0; i < NUM_THREADS; i++)
        {
            printf("I am the process created by compiler By default\n");
            sleep(1);
        }
        // Not really needed?
        pthread_join(tid, NULL);
    }
    else
    {
        printf("Error");
    }
    return 0;
    // // Joins threads
    // for (i = 0; i < NUM_THREADS; i++)
    // {
    //     printf("I am the process created by compiler By default\n");
    //     sleep(1);
    // }
}

void *custom_thread(void *args)
{
    for (int i = 0; i < NUM_THREADS; i++)
    {
        // Gets the double default line shown within the expected output
        if (i == 4)
        {
            sleep(1);
        }
        printf("I am a Custom Thread Function Created by user.\n");
        sleep(1);
    }
    exit(0);
}