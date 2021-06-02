#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
int main() {

  printf("the pid is: %d ", getpid());
  pid_t child_pid = fork();
  // Shows the details of both processes
  // system("ps -l");
  // Making the Parent Process to Sleep for some time.
  if (child_pid > 0){ 
    printf("In the parent process: %d \n",getpid()); 
    // Check process list to see the zombie process
    // system("ps -l");
    sleep(20);
  }else{ 
    printf("In the child process: %d \n",getpid());
    exit(0);
  }
  return 0;
}
