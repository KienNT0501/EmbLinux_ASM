#include <signal.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

volatile sig_atomic_t print_flag = false;
int count = 0;
void handle_alarm( int sig ) {
    count++;
    if ( print_flag ) {
    printf( "Timer: <%d> seconds\n", count );
    print_flag = false;
    }
}

int main() {
    signal( SIGALRM, handle_alarm ); // Install handler first,
    print_flag = true;
    alarm( 1 ); // before scheduling it to be called.
    while (count < 10) {
        printf("In while loop\n");
        sleep( 1 );
        print_flag = true;
        alarm( 1 ); // Reschedule.
    }
}