/*  OS Assignment 1 Q2
    Authors: Robert, Jagveer
    Date: 2021-06-02 (YYYY-MM-DD)
*/

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>


printf("starting parent process");



// Read file function




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