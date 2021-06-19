#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>

// definintions
int main(int argc, char *argv[]);
void *custom_thread(void *arg);

int main(int argc, char *argv[])
{

    //int status = pthread_create(&t_id, &thread_attrib, (void *)&threadRun, thread);
}

void *custom_thread(void *arg)
{

    printf("I am a Custom Thread Function Created by User");
}