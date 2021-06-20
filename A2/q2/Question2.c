/*  ************************************************************************************
    Operating Systems 
    Assignment 2 | Question 2
    
    Authors: Robert, Jagveer
    Date: 2021-06-19 (YYYY-MM-DD)

    GENERAL OBJECTIVE
        
    
    ************************************************************************************
*/
// C libraries used within the program
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <sys/wait.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

/*Using #define causes tons of errors
*Global variables declared
*/
int MAXVALUE = 0;
int MINVALUE = 0;
float AVGVAL = 0;
float MEDVAL = 0;
float HALF = 0.5;
float CUT_HALF = 2.0;

//Structure declared to help with calculations
struct array_nums
{
    int len;
    int *nums;
} Array_nums;

//All function declarations
void *avg_fn(struct array_nums *array_nums);
void *max_fn(struct array_nums *array_nums);
void *min_fn(struct array_nums *array_nums);
void *median_fn(struct array_nums *array_nums);
void retrieve_user_parameters(int total_nums, char *the_array[], int target_arr[]);

/**
 * A helper function that quickly checks whether numbers can be sorted
 * Wouldnt work unless its declared before main
 * */
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

/**
 * A helper function that creates individual threads for each calculation
 * Creating a helper function to create threads helped make program cleaner and avoids many C errors
 * Wouldnt work unless its declared before main
 * */
void *create_thread(void *function, struct array_nums *array_nums)
{
    //Process Thread variable declarations
    pthread_t tid;
    pthread_attr_t thread_attr;
    int s;

    //Causes a seg fault when this line isn't present
    s = pthread_attr_init(&thread_attr);

    //Thread attr can't initialize
    if (s != 0)
    {
        printf("**ERROR***\n");
        // return(-1) gave errors
        exit(-1);
    }
    /**
     * Creates a process thread
     * uses the given function parameter to easily create a thread for any given function calculations 
     * */
    s = pthread_create(&tid, &thread_attr, function, array_nums);

    //Thread wasn't created successfully
    if (s != 0)
    {
        printf("**ERROR**\n");
        // return(-1) gave errors
        exit(-1);
    }

    //Allows thread enough time to be created successfully
    sleep(1);

    //Avoids non-attribute warning
    return NULL;
}

//Main recieves user paramters
int main(int argc, char *argv[])
{
    //Grabs # of arguments
    int nums[argc - 1];

    //Properly grabs users arguments and converts into a new array
    retrieve_user_parameters(argc, argv, nums);

    /* Built in sort function within a C library
    *Sorts the new array
    */
    qsort(nums, argc - 1, sizeof(argc - 1), new_sort);

    struct array_nums array_nums;
    array_nums.len = argc - 1;
    array_nums.nums = nums;

    //Creates multiple threads for calcualtions
    create_thread(&avg_fn, &array_nums);
    create_thread(max_fn, &array_nums);
    create_thread(min_fn, &array_nums);
    create_thread(median_fn, &array_nums);

    //Prints formatted output as stated within assignment instructions
    printf("The average value calculated by the first thread is %0.2f\n", AVGVAL);
    printf("The maximum value calculated by the second thread is %d\n", MAXVALUE);
    printf("The minimum value calculated by the third thread is %d\n", MINVALUE);
    printf("The median value calculated by the fourth thread is %0.2f\n", MEDVAL);

    //Successfully finished execution
    return 0;
}

//Function that does calculations to find the max value
void *max_fn(struct array_nums *array_nums)
{
    //Retrieves lenght of array
    int len = array_nums->len;
    int *the_array = array_nums->nums;

    int address = 0;

    //FInds the max val
    for (int i = 0; i < len; i++)
    {
        if (the_array[address] < the_array[i])
            address = i;
    }
    //Returns max val
    MAXVALUE = the_array[address];
    return NULL;
}

//Function that does calculations to find the min value
void *min_fn(struct array_nums *array_nums)
{
    int len = array_nums->len;
    int *the_array = array_nums->nums;

    int address = 0;
    //finds min val
    for (int i = 0; i < len; i++)
    {
        if (the_array[address] > the_array[i])
            address = i;
    }
    //returns min val
    MINVALUE = the_array[address];
    return NULL;
}

//Function that does calculations to find the avg value
void *avg_fn(struct array_nums *array_nums)
{
    //grabs total length of array
    int count = array_nums->len;

    int *the_array = array_nums->nums;

    int total = 0;

    //find the total sum of the array
    for (int i = 0; i < count; i++)
    {
        total = total + the_array[i];
    }
    //returns avg
    AVGVAL = total / count;
    return NULL;
}

//Function that does calculations to find the median value
void *median_fn(struct array_nums *array_nums)
{

    int len = array_nums->len;
    int *the_array = array_nums->nums;
    //calculates median based on formula
    if (len % 2 == 0)
    {
        int temp1 = len * HALF;
        int temp2 = (len - 1) * HALF;
        //returns median
        MEDVAL = (the_array[temp2] + the_array[temp1]) / CUT_HALF;
    }
    else
    {
        int address = len * HALF;
        //returns median
        MEDVAL = (the_array[address]);
    }
    return NULL;
}

//Function that grabs all the users given paramters
void retrieve_user_parameters(int total_nums, char *the_array[], int target_arr[])
{
    //goes through each argument
    for (int i = 0; i < total_nums - 1; i++)
    {
        char *a;
        //converts string to int to do necessary calculations
        int user_param = strtol(the_array[i + 1], &a, 10);

        //adds to array

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
