#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
int main() {
    // Call z_creator to create a zombie process
    system("./z_creator &");
    // View to make sure zombie process is working
    system("ps -l");
    sleep(3);
    // Kills the zombie process
    system("kill -9 $(ps -l|grep -w Z|tr -s ' '|cut -d ' ' -f 5)");
    
    sleep(10);
    // Results to show the updated processes 
    printf("\n\nNew Updated Process states:\n\n");
    system("ps -l");
    return 0;
}
