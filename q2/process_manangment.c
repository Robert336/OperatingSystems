/*  OS Assignment 1 Q2
    Authors: Robert, Jagveer
    Date: 2021-06-02 (YYYY-MM-DD)
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>

void readFile(void *shm_ptr);
void writeOutput(char* command, char* output);


int main(){
    printf("Start of the parent process\n");

    /* GENERAL OBJECTIVE
        create child process to get linux coommands from file and store in shared memory
        contents of the shared memory will be copied to dynamically allocated array
        create another child process to execute these commands and send the output through a pipe
        parent process with read data from the pipe and write the commands and outputs to a text file
    */
    
    // code below is creating shared memory using POSIX API
    // Size of the shared memory object in bytes
    const int SIZE = 128; // arbitrary size to acomedate Strings being read from the file

    // Name of the shared memory obejct
    const char *name = "READ_LINE";

    // Shared memory file decriptor
    int shm_fd;
    
    // Pointer to the shared memory object
    void *shm_ptr;
    

    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);

    // Opening the shared memory 
    if (shm_fd < 0) { printf("shared memory failed to open");}

    // Configures the size if the shared memory
    ftruncate(shm_fd, SIZE);

    // Memory mapping the shared memory object
    shm_ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
		printf("Map failed\n");
		exit(-1);
	}

    // Child process to read from file
    int child_pid = fork(); // creating child process

    if(child_pid == 0){
        printf("this is a child process");
        readFile(shm_ptr);
        
    } else if (child_pid == -1){
        printf("Child process failed");
    }


}

// Reads the sample_in.txt file
void readFile(void *shm_ptr){
    char max_char[500];

        FILE *fp;
        fp = fopen("sample_in.txt", "r"); // opens file in "read" mode

        // read file to shared memory. line by line.  
        char str[60];
        str = fgets(max_char, 500, fp);
        while(str != NULL){
            // concat the newly read line to the main String in shared memory
            // print to the shared memory (shm_ptr)
            sprintf(shm_ptr, "%s", (char*)max_char);
            shm_ptr += strlen(str);

            // test
            printf("%s", (char*)shm_ptr);
            
            // reads the next line
            str = fgets(max_char, 500, fp);
        }

}


// Output function
void writeOutput(char* command, char* output)
{
    FILE* fp;
    // open file in writing mode
    fp = fopen("output.txt", "w");

    if (fp == NULL){
        printf("ERROR >>> file pointer is NULL");
    }
   
    /* Complete code to save the commands in a output.txt*/
	fprintf(fp, "The output of: %s : is\n", command);
	fprintf(fp, ">>>>>>>>>>>>>>>\n%s<<<<<<<<<<<<<<<\n", output);	

    fclose(fp);

}