/*  ************************************************************************************
    Operating Systems 
    Assignment 1 | Question 1
    
    Authors: Robert, Jagveer
    Date: 2021-06-02 (YYYY-MM-DD)

    GENERAL OBJECTIVE
        This kills the zombie process that was created by running the 'kill' system
        command with appropriate flags.
    
    ************************************************************************************
*/

#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>

int main() {

    // Call z_creator to create a zombie process
    system("./z_creator &");

    // View list of processes to make sure zombie process is working
    system("ps -l");

    sleep(3);
    //Given System call to kills the zombie process
    system("kill -9 $(ps -l|grep -w Z|tr -s ' '|cut -d ' ' -f 5)");
    
    sleep(10);

    // Results to show the updated processes 
    printf("\n\nNew Updated Process states:\n\n");
    //Shows the updated processes after killing the zombie process
    system("ps -l");

    return 0;
}
