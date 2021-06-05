/*  ************************************************************************************
    Operating Systems 
    Assignment 1 | Question 2
    
    Authors: Robert, Jagveer
    Date: 2021-06-02 (YYYY-MM-DD)

    GENERAL OBJECTIVE
        create child process to get linux coommands from file and store in shared memory
        contents of the shared memory. create another child process to execute these 
        commands and send the output through a pipe parent process with read data 
        from the pipe and write the commands and outputs to a text file.
    
    ************************************************************************************
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

int main(int argc, char *argv[]);
void readFile(char *shm_ptr, char *filename, int length);
void writeOutput(char *command, char *output);
void exec_cmds_from_shm(char *memory_pointer);
void write_cmds_output_to_pipe(char *ptr, int pipeID);
void pipe_to_file(int pipeID);

/* List of various constants used within the program */
const int SIZE = 4096; // arbitrary size to acomedate Strings being read from the file
const char *SHM_FILE_NAME = "SHM_FILE";     // Name of the shared memory obejct
const char *OUTPUT_FILE_NAME = "output.txt"; // name of the output file

int main(int argc, char *args[])
{
    // Shared memory file decriptor
    int shm_fd;

    // Pointer to the shared memory object
    char *shm_ptr;

    shm_fd = shm_open(SHM_FILE_NAME, O_CREAT | O_RDWR, 0666);

    // Opening the shared memory
    if (shm_fd < 0){
        printf("**ERROR** Shared Memory failed to open");
    }

    // Configures the size if the shared memory
    ftruncate(shm_fd, SIZE);

    // Memory mapping the shared memory object
    shm_ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    
    if (shm_ptr == MAP_FAILED){
        printf("Map failed\n");
        exit(-1);
    }

    // takes the command line argument (AS MENTIONED IN LECTURE args[0] cannot be read)
    char *filename = args[1];

    // Child process to read from file
    int child_process_id = fork(); // creating child process

    if (child_process_id == 0){
        // printf("this is a child process");
        readFile(shm_ptr, filename, strlen(filename));
        exec_cmds_from_shm(shm_ptr);

        //unmap shared memory
        munmap(shm_ptr, SIZE);
        //unlink shared memory
        shm_unlink(SHM_FILE_NAME);

        exit(0);
    }
    else if (child_process_id == -1){  
        printf("**ERROR** Creation of the child process has failed");
    }else{
         // printf("Start of the parent process\n");
    }
}

// Reads the sample_in.txt file
void readFile(char *shm_ptr, char* fileName, int file_length)
{
    FILE *fp = fopen(fileName, "r"); // opens file in "read" mode
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    // read file to shared memory. line by line.
    
    while ((read = getline(&line, &len, fp)) != -1)
    {
        shm_ptr += sprintf(shm_ptr, "%s", line);
    }
}


void exec_cmds_from_shm(char *memory_pointer) {
    char *ptr = memory_pointer;

    char *pipe_name = "/tmp/mypipe"; // naming the pipe file
    mkfifo(pipe_name, 0666); // creates named pipe that stays active until pipe is closed
    pid_t pid = fork(); // create child process

    if (pid == 0) { // child process
        int pipeID = open(pipe_name, O_WRONLY); // opens pipe in write only mode
        char result[SIZE];
        for (int i = 0; i < 64; i += 1) {
            char data = (char)ptr[i];
            strncat(result, &data, 1);
        }

        char *result_pointer = strtok(result, "\r\n"); // 
        write_cmds_output_to_pipe(result_pointer, pipeID);
        close(pipeID);
    }

    else if (pid > 0) {
        int pipeID = open(pipe_name, O_RDONLY);
        int status;
        wait(&status);
        if (WEXITSTATUS(status) == -1) {
            perror("\nexec_commands_from_memory: Error while waiting\n");
            exit(-1);
        }

        pipe_to_file(pipeID);
        close(pipeID);
    }

    else {
        printf("\nexec_commands_from_memory: Error while forking\n");
        exit(-1);
    }
}


// Output function
void writeOutput(char *command, char *output)
{
    FILE *fp;
    // open file in writing mode
    fp = fopen("output.txt", "w");

    if (fp == NULL){
        printf("***ERROR*** File Pointer is NULL");
    }

    /* Complete code to save the commands in a output.txt*/
    fprintf(fp, "The output of: %s : is\n", command);
    fprintf(fp, ">>>>>>>>>>>>>>>\n%s<<<<<<<<<<<<<<<\n", output);

    //Once file is written to it closes
    fclose(fp);
}


void write_cmds_output_to_pipe(char *ptr, int pipeID) {
    char result[SIZE];
    while (ptr) {
        FILE *virtual_file = popen(ptr, "r"); // opening pipe file in read
        char line[1035];
        if (virtual_file) {
            char first_line[50];
            sprintf(first_line, "The output of: %s : is\n>>>>>>>>>>>>>>>\n", ptr);
            strcat(result, first_line);

            // get the contents of each line of the pipe (virtual file)
            while (fgets(line, sizeof(line), virtual_file) != NULL) {
                strcat(line, "\n");
                strcat(result, line); // append to the overall output string
            }

            strcat(result, "<<<<<<<<<<<<<<<"); // marks end of output

        } else {
            printf("\nexecute_commands: Error while executing '%s'!\n", ptr);
            exit(-1);
        }

        fclose(virtual_file);
        ptr = strtok(NULL, "\r\n"); // split the str into tokens with '\n' deliminiter
    }

    write(pipeID, result, SIZE + 1); // writes output string to the pipe
}

void pipe_to_file(int pipeID) {
    char result[SIZE];
    read(pipeID, result, SIZE);

    FILE *output_file = fopen(OUTPUT_FILE_NAME, "w");
    char *ptr = strtok(result, "\r\n");
    while (ptr) {
        fprintf(output_file, "%s\n", ptr);
        ptr = strtok(NULL, "\r\n");
    }

    fclose(output_file);
}