#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
int main(void) {

  pid_t child_pid = fork();
  // Making the Parent Process to Sleep for some time.
  if (child_pid > 0){ 
    printf("In Parent Process.!\n"); 
    sleep(10);
  }else{ 
    printf("In Child process.!\n"); 
    exit(0);
  }
  return 0;
}
