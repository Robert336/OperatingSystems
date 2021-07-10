/*  ************************************************************************************
    Operating Systems 
    Assignment 2 | Question 1
    
    Authors: Robert, Jagveer
    Date: 2021-06-19 (YYYY-MM-DD)

    GENERAL OBJECTIVE
    Demonstrating the ambiguity of the order of exectution of print statements from threads
    ************************************************************************************
*/

// C libraries included within file
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>

// Constant defined for the total number of iterations required per thread
#define NUM_THREADS 5

// definintions
int main(int argc, char *argv[]);
void *custom_thread(void *arg);

/* Answer to question asked on the assignment instruction sheet:
* 
*Yes we do notice ambiguity in the order of the printed statments of the output of the program
*/
int main(int argc, char *argv[])
{

    int i;
    // Defined thread variables
    pthread_t tid;
    pthread_attr_t attr;
    int status;
    //Causes seg fault without this line
    //Retrieves process thread attribute
    status = pthread_attr_init(&attr);

    // Error testing:
    if (status != 0)
    {
        printf("**Error**");
        return -1;
    }

    printf("This program would create threads\n");

    /**
     * Creates threads
     * Declaring a variable to create the threads avoids duplication of printf within the if statement
     * */
    status = pthread_create(&tid, &attr, custom_thread, NULL);
    // Puts the thread to sleep for 1 second
    sleep(1);

    // Thread is created successfully
    if (status == 0)
    {
        printf("Custom thread created successfully\n");

        for (i = 0; i < NUM_THREADS; i++)
        {
            printf("I am the process created by compiler By default\n");
            sleep(1);
        }
        // Not really needed? Good coding practice to include
        pthread_join(tid, NULL);
    }
    else
    {
        printf("Error");
    }
    return 0;
}

// Function that creates a custom thread as required
void *custom_thread(void *args)
{
    for (int i = 0; i < NUM_THREADS; i++)
    {
        /*
        *Gets the double default line shown within the expected output
        *Not needed otherwise
        */
        if (i == 4)
        {
            sleep(1);
        }
        printf("I am a Custom Thread Function Created by user.\n");
        sleep(1);
    }
    //Successfully completed function
    exit(0);
}