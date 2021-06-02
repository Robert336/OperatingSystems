#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
int main(void) {
    system("./z_creator &");
    system("ps -l");
    sleep(10);
    system("kill -9 $(ps -l|grep -w Z|tr -s ''|cut -d '' -f 5)");
    sleep(10);
    printf("Testing\n");
    system("ps -l");
    return 0;
}
