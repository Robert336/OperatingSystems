/*  OS Assignment 1 Q2
    Authors: Robert, Jagveer
    Date: 2021-06-02 (YYYY-MM-DD)
*/

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/shm.h>

void readFile(void *shm_ptr);
void writeOutput(char* command, char* output);


int main(){
    printf("Start of the parent process");

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

    // Memory mapping the shared memory object
    shm_ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);


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
        while(fgets(max_char, 500, fp) != NULL){
            // concat the newly read line to the main String in shared memory
            char str[20];
            sprintf(str, "%s", max_char);

            printf("%s", (char*)shm_ptr);
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