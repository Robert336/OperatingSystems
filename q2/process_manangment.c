/*  OS Assignment 1 Q2
    Authors: Robert, Jagveer
    Date: 2021-06-02 (YYYY-MM-DD)
*/

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
//#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<sys/shm.h>

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
    int shmFileDesc;
    
    // Pointer to the shared memory object
    void *ptr;

    // Opening the shared memory 
    if (shm_open(name, O_RDONLY, 0666) < 0) { printf("shared memory failed to open");}

    pid_t child_pid = fork(); // creating child process

    if(child_pid == 0){
        printf("this is a child process");
        
        FILE *fp;
        fp = fopen("sample_in.txt", "r"); // opens file in "read" mode



    }


}



// Read file function
void readFile(){
    FILE *fp;
    fp = fopen("sample_in.txt", "r");


}



// Output function
void writeOuput(char* command, char* output)
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

	/* Your code here*/
    fclose(fp);

}