#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>

// definintions
int main(int argc, char *argv[]);

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

    status = pthread_create(&tid, &attr, thread_calculations, NULL);

    return 0;
}

void *thread_calculations(void *args)
{
}
