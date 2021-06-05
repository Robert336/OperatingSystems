/*  ************************************************************************************
    Operating Systems 
    Assignment 1 | Question 1
    
    Authors: Robert, Jagveer
    Date: 2021-06-02 (YYYY-MM-DD)

    GENERAL OBJECTIVE
        This file creates and turns a child process into a zombie process. Z-creator
        is used as a helper file for z_terminator.
    
    ************************************************************************************
*/

#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>

int main() {

  // printf("the pid is: %d ", getpid());
  // pid_t child_process = fork();

  // By forking we create a new child process
  int child_process = fork();

  // Shows the details of both processes
  // system("ps -l");

// The id of parent process is > 0
  if (child_process > 0){ 
    // printf("In the parent process: %d \n",getpid()); 
    printf("\nIn the parent process: %d\n ",child_process); 

    // Check process list to see the zombie process
    // system("ps -l");

    // Variable to decide how long zombie process will remain in the system
    int x = 20;
    sleep(x);
  
  }else if (child_process<0){ //Failed fork processes value is -1
    printf("Failed to fork process\n");
    exit(-1);
  }else{  // The int value of child processes is 0 
    // printf("In the child process: %d \n",getpid());
    printf("In the child process: %d \n\n",child_process);
    exit(0);
  }
  return 0;
}
