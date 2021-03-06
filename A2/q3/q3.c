#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <time.h>

typedef struct thread //represents a single thread
{
	char tid[4]; // id of the thread as read from file
	int start_time; // the start time at which the thread starts
	int lifetime; // the total span of time the thread will be active for
	int is_running; // tracking the state of the thread
	pthread_t t_id; // proper thread id

} Thread;

void* threadRun(Thread* t);//the thread function, the code executed by each thread
int readFile(char* fileName, Thread *threads);//function to read the file content and build array of threads
void logStart(char* tID);//function to log that a new thread is is_running
void logFinish(char* tID);//function to log that a thread has finished its time
pthread_t create_thread(Thread *thread); // creates new thread
void startClock();//function to start program clock
long getCurrentTime();//function to check current time since clock was is_running
time_t programClock;//the global timer/clock for the program


int main(int argc, char *argv[])
{
	if(argc<2)
	{
		printf("Input file name missing...exiting with error code -1\n");
		return -1;
	}
	
	// takes commmand line argument
	char *filename = argv[1];

	Thread *threads; // array of threads (size is determined in the readFile function)
	int threadCount = readFile( filename, threads); // creates the array of threads and returns the count
	
	startClock();
	// printf("Times a tickin'! (clock is_running)");

	int completed_t = 0;

    programClock = time(NULL);
    while (completed_t < threadCount) {
        for (int i = 0; i < threadCount; i++) {
            Thread thread = threads[i];
            // time_t now = time(NULL);
			
            if (!thread.is_running && getCurrentTime() == thread.start_time) {
				threads[i].is_running = 1; // thread is running!
                completed_t++;
                pthread_t t_id = create_thread(&threads[i]);
                threads[i].t_id = t_id;
            }
        }
    }

    for (int i = 0; i < threadCount; i++){
        pthread_join(threads[i].t_id, NULL);
	}

	return 0;
}

//use this method in a suitable way to read file
int readFile(char* fileName, Thread *threads){
	FILE *in = fopen(fileName, "r");
	if(!in)
	{
		printf("Child A: Error in opening input file...exiting with error code -1\n");
		return -1;
	}

	struct stat st;
	fstat(fileno(in), &st);
	char* fileContent = (char*)malloc(((int)st.st_size+1)* sizeof(char));
	fileContent[0]='\0';	
	while(!feof(in))
	{
		char line[100];
		if(fgets(line,100,in)!=NULL)
		{
			strncat(fileContent,line,strlen(line));
		}
	}
	fclose(in);

	char* command = NULL;
	int threadCount = 0;
	char* fileCopy = (char*)malloc((strlen(fileContent)+1)*sizeof(char));
	strcpy(fileCopy,fileContent);
	command = strtok(fileCopy,"\r\n");
	while(command!=NULL)
	{
		threadCount++;
		command = strtok(NULL,"\r\n");
	}
	Thread *threads1 =  malloc(sizeof(Thread)*threadCount);

	char* lines[threadCount];
	command = NULL;
	int i=0;
	command = strtok(fileContent,"\r\n");
	while(command!=NULL)
	{
		lines[i] = malloc(sizeof(command)*sizeof(char));
		strcpy(lines[i],command);
		i++;
		command = strtok(NULL,"\r\n");
	}

	for(int k=0; k<threadCount; k++)
	{
		char* token = NULL;
		int j = 0;
		token =  strtok(lines[k],";");
		
		Thread *newThread =  malloc(sizeof(Thread));

		while(token!=NULL) //this loop tokenizes each line of input file
		{
			
			//printf("TOKEN == %s", token); // **COMMENT THIS LINE WHEN FINISHED**
			
			// check which value is currently the token
			// Options are: tID, starttime, or lifetime

			if (j == 0){
				strcpy(newThread->tid, token);
			} else if (j == 1){
				newThread->start_time = atoi(token);
			} else {
				newThread->lifetime = atoi(token);
				newThread->is_running = 0; // start false
			

			token = strtok(NULL, ";");
			j++;

		}

		// add the newly created thread to the collection of threads (threads array)
		threads1[k] = *newThread;
	}
	return threadCount;
}
}
void logStart(char* tID)//invoke this method when you start a thread
{
	printf("[%ld] New Thread with ID %s is is_running.\n", getCurrentTime(), tID);
	return 0;
}

void logFinish(char* tID)//invoke this method when a thread is over
{
	printf("[%ld] Thread with ID %s is finished.\n", getCurrentTime(), tID);
	return 0;
}

void *threadRun(Thread *t) // thread starting routine
{
	logStart(t->tid);
	sleep(t->lifetime);
	logFinish(t->tid);
	return 0;
	// exit thread at the end of it's lifetime
}

pthread_t create_thread(Thread* thread) {
    pthread_t t_id;
    pthread_attr_t thread_attrib;
    int status;
    status = pthread_attr_init(&thread_attrib);
    status = pthread_create(&t_id, &thread_attrib, (void*)&threadRun, thread);
    return t_id;
}

void startClock()//invoke this method when you start servicing threads
{
	programClock = time(NULL);
	return 0;
}

long getCurrentTime()//invoke this method whenever you want to check how much time units passed since you invoked startClock()
{
	time_t now;
	now = time(NULL);
	return abs(now-programClock);
}