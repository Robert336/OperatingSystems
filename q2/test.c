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
    int id = fork();

    if(id>0){
        printf("Parent process: %d", id);
    }else if(id == 0){
        printf("Child process: %d\n", id);
        // Read file
        // Write file data to shared mmy between parent and child
        // Terminate child
        // Fork new child in parent ?
        readfile();
        
    }else{
        printf("Fork Failed to create child process");
        return -1;
    }
}

void  readfile(){
    FILE *fp;
    // The limit for maximum characters in file
    char maximum_char[500];
    // Opens sample file
    char* sample_txt = "sample_in.txt";
    
    fp = fopen(sample_txt, "r");
    if (fp == NULL){
        printf("##ERROR## Unable to open %s",sample_txt);
        exit(0);
    }
    // 
    while (fgets(maximum_char, 500, fp) != NULL){
        // Prints out each line
        // char str[] = strcat(str,maximum_char);
       printf("%s", maximum_char);
        // printf("%s",str);
    }
    fclose(fp);
}