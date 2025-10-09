#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
int count = 0;
void signal_handler(int sig){
    count++;
    printf("\nSIGINT received ! Signal count: %d\n", count);
}
int main(){
    signal(SIGINT, signal_handler);
    printf("Waiting for SIGINT - Ctrl + C ... \n");
    while( count < 3){}
    printf("Signal count = %d - Program terminated!\n", count);
    return 0;
}