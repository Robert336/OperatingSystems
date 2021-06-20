#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <sys/wait.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

int MAXVALUE;
int MINVALUE;
float AVGVAL;
float MEDVAL;
float HALF = 0.5;
float CUT_HALF = 2.0;

struct array_nums
{
    int len;
    int *nums;
} Array_nums;

void *avg_fn(struct array_nums *array_nums);
void *max_fn(struct array_nums *array_nums);
void *min_fn(struct array_nums *array_nums);
void *median_fn(struct array_nums *array_nums);
void retrieve_user_parameters(int total_nums, char *the_array[], int target_arr[]);

//Wouldnt work unless its declared before main
int new_sort(const void *temp1, const void *temp2)
{
    int a = *((int *)temp1);
    int b = *((int *)temp2);
    if (a > b)
        return 1;
    if (b > a)
        return -1;
    return 0;
}

//Wouldnt work unless its declared before main
void *create_thread(void *function, struct array_nums *array_nums)
{
    pthread_t tid;
    pthread_attr_t thread_attr;
    int s;

    s = pthread_attr_init(&thread_attr);

    if (s != 0)
    {
        printf("**ERROR***\n");
        // return(-1) gave errors
        exit(-1);
    }
    s = pthread_create(&tid, &thread_attr, function, array_nums);

    if (s != 0)
    {
        printf("**ERROR**\n");
        // return(-1) gave errors
        exit(-1);
    }
    //Allows thread enough time to perform
    sleep(1);

    return NULL;
}

int main(int argc, char *argv[])
{
    int nums[argc - 1];

    retrieve_user_parameters(argc, argv, nums);

    // Built in sort function within a C library
    qsort(nums, argc - 1, sizeof(argc - 1), new_sort);

    struct array_nums array_nums;
    array_nums.len = argc - 1;
    array_nums.nums = nums;

    MAXVALUE = 0;
    MINVALUE = 0;
    AVGVAL = 0;
    MEDVAL = 0;

    create_thread(&avg_fn, &array_nums);
    create_thread(max_fn, &array_nums);
    create_thread(min_fn, &array_nums);
    create_thread(median_fn, &array_nums);

    printf("The average value calculated by the first thread is %0.2f\n", AVGVAL);
    printf("The maximum value calculated by the second thread is %d\n", MAXVALUE);
    printf("The minimum value calculated by the third thread is %d\n", MINVALUE);
    printf("The median value calculated by the fourth thread is %0.2f\n", MEDVAL);

    return 0;
}

void *max_fn(struct array_nums *array_nums)
{
    int len = array_nums->len;
    int *the_array = array_nums->nums;

    int address = 0;

    for (int i = 0; i < len; i++)
    {
        if (the_array[address] < the_array[i])
            address = i;
    }
    MAXVALUE = the_array[address];
    return NULL;
}

void *min_fn(struct array_nums *array_nums)
{
    int len = array_nums->len;
    int *the_array = array_nums->nums;

    int address = 0;

    for (int i = 0; i < len; i++)
    {
        if (the_array[address] > the_array[i])
            address = i;
    }
    MINVALUE = the_array[address];
    return NULL;
}

void *avg_fn(struct array_nums *array_nums)
{
    int count = array_nums->len;
    int *the_array = array_nums->nums;

    int total = 0;

    for (int i = 0; i < count; i++)
    {
        total = total + the_array[i];
    }
    AVGVAL = total / count;
    return NULL;
}

void *median_fn(struct array_nums *array_nums)
{
    int len = array_nums->len;
    int *the_array = array_nums->nums;

    if (len % 2 == 0)
    {
        int temp1 = len * HALF;
        int temp2 = (len - 1) * HALF;
        MEDVAL = (the_array[temp2] + the_array[temp1]) / CUT_HALF;
    }
    else
    {
        int address = len * HALF;
        MEDVAL = (the_array[address]);
    }
    return NULL;
}

void retrieve_user_parameters(int total_nums, char *the_array[], int target_arr[])
{
    for (int i = 0; i < total_nums - 1; i++)
    {
        char *a;
        int user_param = strtol(the_array[i + 1], &a, 10);

        if (a == the_array[i + 1])
        {
            printf("**ERROR**\n");
            // return(-1) gave errors
            exit(-1);
        }
        else
        {
            target_arr[i] = user_param;
        }
    }
}
