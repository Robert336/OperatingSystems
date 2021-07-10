/*
 -------------------------------------
 File:    QuestionX.c
 Project: A03Q1S21
 file description
 -------------------------------------
 Author:  Sukhjit Singh Sehra
 ID:      11111111
 Email:   ssehra@wlu.ca
 Version  2021-06-19
 -------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
// Global Data variables.
int a = 5, b = 7;
pthread_mutex_t mutex;
// Function that access the global data.
void *inc_dec(void *arg)
{
	pthread_mutex_lock(&mutex); // locked

	printf("Read value of 'a' global variable is: %d\n", a);
	printf("Read value of 'b' global variable is: %d\n", b);
	sleep(1);

	pthread_mutex_unlock(&mutex); // unlocked

	a = a + 1;
	b = b - 1;
	printf("Updated value of 'a' variable is: %d\n", a);
	printf("Updated value of 'b' variable is: %d\n", b);
	return 0;
}
int main()
{
	// Creating the thread instances.
	pthread_t t1, t2, t3;
	pthread_create(&t1, NULL, inc_dec, NULL);
	pthread_create(&t2, NULL, inc_dec, NULL);
	pthread_create(&t3, NULL, inc_dec, NULL);
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	pthread_join(t3, NULL);

	//Destroying the threads.
	pthread_exit(t1);
	pthread_exit(t2);
	pthread_exit(t3);
	return 0;
}
