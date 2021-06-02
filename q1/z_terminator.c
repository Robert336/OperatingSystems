#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
int main() {
    system("./z_creator &");
    system("ps -l");
    sleep(3);
    system("kill -9 $(ps -l|grep -w Z|tr -s ' '|cut -d ' ' -f 5)");
    
    sleep(10);
    printf("\n\nNew states\n\n");
    system("ps -l");
    return 0;
}
