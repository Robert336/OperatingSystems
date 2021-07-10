/*  ************************************************************************************
    Operating Systems 
    Assignment 2 | Question 3
    
    Authors: Robert, Jagveer
    Date: 2021-06-19 (YYYY-MM-DD)

    GENERAL OBJECTIVE
    Creating different threads at different times, with different life spans.
    ************************************************************************************
*/

// C libraries included within file
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <time.h>
#include <stdbool.h>

//Structure declaration to help with threading
typedef struct thread //represents a single thread
{
	char tid[4];	// id of the thread as read from file
	int start_time; // the start time at which the thread starts
	int lifetime;	// the total span of time the thread will be active for
	int is_running; // tracking the state of the thread
	pthread_t t_id; // proper thread id

} Thread;

//All function declarations
//Don't forget to declare main again
int main(int argc, char *argv[]);
void *threadRun(Thread *t);						//the thread function, the code executed by each thread
int readFile(char *fileName, Thread **threads); //function to read the file content and build array of threads
void logStart(char *tID);						//function to log that a new thread is is_running
void logFinish(char *tID);						//function to log that a thread has finished its time
pthread_t create_thread(Thread *thread);		// creates new thread
void startClock();								//function to start program clock
long getCurrentTime();							//function to check current time since clock was is_running
time_t programClock;							//the global timer/clock for the program

int main(int argc, char *argv[])
{
	//A parameter file DNE
	if (argc < 2)
	{
		printf("**ERROR**\n");
		return -1;
	}

	// takes commmand line argument
	char *filename = argv[1];

	Thread *threads;								//= malloc(sizeof(*threads)); // array of threads (size is determined in the readFile function)
	int threadCount = readFile(filename, &threads); // creates the array of threads and returns the count

	//Starts the time
	startClock();

	//count for completed threads
	int completed_t = 0;

	programClock = time(NULL);
	//Goes through all available threads
	while (completed_t < threadCount)
	{
		for (int i = 0; i < threadCount; i++)
		{
			Thread thread = threads[i];
			// time_t now = time(NULL);
			if (!thread.is_running && getCurrentTime() == thread.start_time)
			{
				//create
				threads[i].is_running = 1; // thread is running!
				completed_t++;
				pthread_t t_id = create_thread(&threads[i]);
				threads[i].t_id = t_id;
				//printf("This is working3\n");
			}
		}
	}

	for (int i = 0; i < threadCount; i++)
	{
		//joins threads
		pthread_join(threads[i].t_id, NULL);
		//printf("This is working4\n");
	}
	//free the threads...destroy get rid of
	free(threads);
	//printf("This is working5\n");

	return 0;
}

//use this method in a suitable way to read file
int readFile(char *fileName, Thread **threads)
{
	//opens and reads given file
	FILE *in = fopen(fileName, "r");
	//no file given
	if (!in)
	{
		printf("*ERROR**\n");
		return -1;
	}

	struct stat st;
	fstat(fileno(in), &st);
	char *fileContent = (char *)malloc(((int)st.st_size + 1) * sizeof(char));
	fileContent[0] = '\0';
	while (!feof(in))
	{
		char line[100];
		if (fgets(line, 100, in) != NULL)
		{
			strncat(fileContent, line, strlen(line));
		}
	}
	//close file
	fclose(in);

	char *command = NULL;
	int threadCount = 0;
	char *fileCopy = (char *)malloc((strlen(fileContent) + 1) * sizeof(char));
	strcpy(fileCopy, fileContent);
	command = strtok(fileCopy, "\r\n");
	while (command != NULL)
	{
		threadCount++;
		command = strtok(NULL, "\r\n");
	}
	//size of threads
	*threads = malloc(sizeof(Thread) * threadCount);

	char *lines[threadCount];
	command = NULL;
	int i = 0;
	command = strtok(fileContent, "\r\n");
	while (command != NULL)
	{
		lines[i] = malloc(sizeof(command) * sizeof(char));
		strcpy(lines[i], command);
		i++;
		command = strtok(NULL, "\r\n");
	}

	for (int k = 0; k < threadCount; k++)
	{

		char *token = strtok(lines[k], ";");
		char *tid = token;

		token = strtok(NULL, ";");
		char *start_time = token;

		token = strtok(NULL, ";");
		char *lifetime = token;

		Thread thread;
		strcpy(thread.tid, tid);
		//doesnt work without atoi
		thread.start_time = atoi(start_time);
		thread.lifetime = atoi(lifetime);
		thread.is_running = 0;

		(*threads)[k] = thread;
	}

	return threadCount;
}

void logStart(char *tID) //invoke this method when you start a thread
{
	//Changed from is started to is running
	printf("[%ld] New Thread with ID %s is running.\n", getCurrentTime(), tID);
}

void logFinish(char *tID) //invoke this method when a thread is over
{
	printf("[%ld] Thread with ID %s is finished.\n", getCurrentTime(), tID);
}

void *threadRun(Thread *t) // thread starting routine
{
	logStart(t->tid);
	sleep(t->lifetime);
	logFinish(t->tid);
	// exit thread at the end of it's lifetime
	//exit(0) causes errors
	return NULL;
}

//Function that je;[s create the thread
pthread_t create_thread(Thread *thread)
{
	//declares thread var
	pthread_t t_id;
	pthread_attr_t thread_attrib;
	int status;
	//avoid seg fault
	status = pthread_attr_init(&thread_attrib);

	if (status != 0)
	{
		printf("Error");
	}
	//create
	status = pthread_create(&t_id, &thread_attrib, (void *)&threadRun, thread);
	return t_id;
}

void startClock() //invoke this method when you start servicing threads
{
	programClock = time(NULL);
}

long getCurrentTime() //invoke this method whenever you want to check how much time units passed since you invoked startClock()
{
	time_t now;
	now = time(NULL);
	return now - programClock;
}