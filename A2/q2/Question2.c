/*  ************************************************************************************
    Operating Systems 
    Assignment 2 | Question 2
    
    Authors: Robert, Jagveer
    Date: 2021-06-19 (YYYY-MM-DD)

    GENERAL OBJECTIVE
    Using multi-threading to complete differnt task at the same time.
    ************************************************************************************
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>

// definintions
int main(int argc, char *argv[]);
void *thread_avg(struct num_array *nums);
void *thread_max(struct num_array *nums);
void *thread_min(struct num_array *nums);
void *thread_median(struct num_array *nums);
void sort(int a[], int n);
void swap(int *p, int *q);

// stats of the list are stored globally
int max;
int min;
float avg;
float median;

typedef struct num_array
{
    int length;
    int *nums;
};

// From discussions we need to sort first?
int main(int argc, char *argv[])
{
    max = 0;
    min = 0;
    avg = 0;
    median = 0;

    struct num_array arr_nums; // this is the main array with the numbers from the argument line

    int nums[argc - 1];         // unsorted args
    arr_nums.length = argc - 1; // getting the length of the array

    printf("SIZE OF NUMS (int) == %d\n", arr_nums.length);

    // copy the number values from argv to the nums array
    for (int i = 0; i < arr_nums.length; i++)
    {
        char *c;
        int number = strtol(argv[i + 1], &c, 10);
        if (c == argv[i + 1])
        {
            printf("Error -> error converting numbers");
            exit(-1);
        }
        else
        {
            nums[i] = number;
        }
        printf("nums[%d] == %d\n", i, nums[i]);
    }

    // sort the array, min -> max
    sort(nums, argc - 1);

    arr_nums.nums = nums;

    //printf("is dis sorted doe?! %d -> %d\n", nums[0], nums[array_size - 1]);

    pthread_t tid1;
    pthread_t tid2;
    pthread_t tid3;
    pthread_t tid4;
    pthread_attr_t attr1;
    pthread_attr_t attr2;
    pthread_attr_t attr3;
    pthread_attr_t attr4;
    int t1 = pthread_attr_init(&attr1);
    int t2 = pthread_attr_init(&attr2);
    int t3 = pthread_attr_init(&attr3);
    int t4 = pthread_attr_init(&attr4);

    if (t1 != 0 || t2 != 0 || t3 != 0 || t4 != 0)
    {
        printf("error\n");
        exit(-1);
    }

    // starts the thread to calculate the stats
    t1 = pthread_create(&tid1, &attr1, (void *)thread_avg, &arr_nums);
    t2 = pthread_create(&tid2, &attr2, (void *)thread_max, &arr_nums);
    t3 = pthread_create(&tid3, &attr3, (void *)thread_min, &arr_nums);
    t4 = pthread_create(&tid4, &attr4, (void *)thread_median, &arr_nums);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);
    pthread_join(tid4, NULL);
    return 0;
}

// thread to work on the average
void *thread_avg(struct num_array *nums)
{
    int sum = 0;
    int count = 0;
    for (int i = 0; i < nums->length; i++)
    {
        printf("nums[i] = %d\n", nums->nums[i]);
        printf("SUM = %d\n i = %d\n", sum, i);
        sum = sum + nums->nums[i];
        count++;
    }
    avg = (sum / count);
}

// thread to work on the max
void *thread_max(struct num_array *nums)
{
    int *ptr = &nums->nums[1];
    max = ptr - 1;
    printf(" max %d\n", max);
}

// thread to work on the min
void *thread_min(struct num_array *nums)
{
    min = nums->nums[0];
    printf("min %d\n", min);
}

// thread to work on the median
void *thread_median(struct num_array *nums)
{
    int mid;
    if (nums->length % 2 == 1)
    {
        mid = nums->length / 2 - 1;
    }
    else
    {
        int mid1, mid2;
        mid1 = nums->nums[nums->length / 2 - 1];
        mid2 = nums->nums[nums->length / 2 + 1];
        median = (mid1 + mid2) / 2;
    }

    printf("%2f\n", median);
}

void swap(int *p, int *q)
{
    int t;

    t = *p;
    *p = *q;
    *q = t;
}

void sort(int a[], int n)
{
    int i, j, temp;

    for (i = 0; i < n - 1; i++)
    {
        for (j = 0; j < n - i - 1; j++)
        {
            if (a[j] > a[j + 1])
                swap(&a[j], &a[j + 1]);
        }
    }
}
