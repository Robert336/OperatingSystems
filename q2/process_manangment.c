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

int main(int argc, char *argv[]);
void readFile(char *shm_ptr, char* filename, int len);
void writeOutput(char *command, char *output);
void exec_cmds_from_shm(char *memory_pointer);
void write_cmds_output_to_pipe(char *ptr, int pipeID);
void pipe_to_file(int pipeID);


const int SIZE = 4096; // arbitrary size to acomedate Strings being read from the file
const char *SHM_FILE_NAME = "SHM_FILE";     // Name of the shared memory obejct
const char *OUTPUT_FILE_NAME = "output.txt";

int main(int argc, char *args[])
{
    printf("Start of the parent process\n");

    /* GENERAL OBJECTIVE
        create child process to get linux coommands from file and store in shared memory
        contents of the shared memory will be copied to dynamically allocated array
        create another child process to execute these commands and send the output through a pipe
        parent process with read data from the pipe and write the commands and outputs to a text file
    */
    // Shared memory file decriptor
    int shm_fd;

    // Pointer to the shared memory object
    char *shm_ptr;

    shm_fd = shm_open(SHM_FILE_NAME, O_CREAT | O_RDWR, 0666);

    // Opening the shared memory
    if (shm_fd < 0)
    {
        printf("shared memory failed to open");
    }

    // Configures the size if the shared memory
    ftruncate(shm_fd, SIZE);

    // Memory mapping the shared memory object
    shm_ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    
    if (shm_ptr == MAP_FAILED)
    {
        printf("Map failed\n");
        exit(-1);
    }

    // takes the command line argument
    char *filename = args[1];

    // Child process to read from file
    int child_pid = fork(); // creating child process

    if (child_pid == 0)
    {
        printf("this is a child process");
        readFile(shm_ptr, filename, strlen(filename));
        exec_cmds_from_shm(shm_ptr);

        exit(0);
    }
    else if (child_pid == -1)
    {
        printf("Child process failed");
    }
}

// Reads the sample_in.txt file
void readFile(char *shm_ptr, char* fileName, int len)
{
    FILE *fp;
    fp = fopen(fileName, "r"); // opens file in "read" mode
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    // read file to shared memory. line by line.
    
    while ((read = getline(&line, &len, fileName)) != -1)
    {
        shm_ptr += sprintf(shm_ptr, "%s", line);
    }
}


void exec_cmds_from_shm(char *memory_pointer) {
    char *ptr = memory_pointer;

    char *pipe_name = "/tmp/mypipe";
    mkfifo(pipe_name, 0666);
    pid_t pid = fork();

    if (pid == 0) {
        int pipeID = open(pipe_name, O_WRONLY);
        char result[SHARED_MEMORY_SIZE];
        for (int i = 0; i < 64; i += 1) {
            char data = (char)ptr[i];
            strncat(result, &data, 1);
        }

        char *result_pointer = strtok(result, "\r\n");
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
        printf("\nexec_commands_from_memory: Error while forking!\n");
        exit(-1);
    }
}


// Output function
void writeOutput(char *command, char *output)
{
    FILE *fp;
    // open file in writing mode
    fp = fopen("output.txt", "w");

    if (fp == NULL)
    {
        printf("ERROR >>> file ptr is NULL");
    }

    /* Complete code to save the commands in a output.txt*/
    fprintf(fp, "The output of: %s : is\n", command);
    fprintf(fp, ">>>>>>>>>>>>>>>\n%s<<<<<<<<<<<<<<<\n", output);

    fclose(fp);
}


void write_cmds_output_to_pipe(char *ptr, int pipeID) {
    char result[SHARED_MEMORY_SIZE];
    while (ptr) {
        FILE *virtual_file = popen(ptr, "r");
        char line[1035];
        if (virtual_file) {
            char first_line[50];
            sprintf(first_line, "The output of: %s : is\n>>>>>>>>>>>>>>>\n", ptr);
            strcat(result, first_line);

            while (fgets(line, sizeof(line), virtual_file) != NULL) {
                strcat(line, "\n");
                strcat(result, line);
            }

            strcat(result, "<<<<<<<<<<<<<<<");

        } else {
            printf("\nexecute_commands: Error while executing '%s'!\n",
                   ptr);
            exit(-1);
        }

        fclose(virtual_file);
        ptr = strtok(NULL, "\r\n");
    }

    write(pipeID, result, SHARED_MEMORY_SIZE + 1);
}

void pipe_to_file(int pipeID) {
    char result[SHARED_MEMORY_SIZE];
    read(pipeID, result, SHARED_MEMORY_SIZE);

    FILE *output_file = fopen(OUTPUT_FILE_NAME, "w");
    char *ptr = strtok(result, "\r\n");
    while (ptr) {
        fprintf(output_file, "%s\n", ptr);
        ptr = strtok(NULL, "\r\n");
    }

    fclose(output_file);
}
