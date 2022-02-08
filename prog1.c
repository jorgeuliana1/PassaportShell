#include <stdbool.h>
#include <stdio.h>
#include <time.h>

void delay(int seconds){
    // Converting time into milli_seconds
    int milliSeconds = 1000 * seconds;
    
    clock_t startTime = clock();
  
    // looping till required time is not achieved
    while (clock() < startTime + milliSeconds) ;
}



int main(int argc, char const *argv[]){
    int i=0;
    while(i < 15000){
        printf("\033[1;32m");
        printf(".\n");
        printf("\033[0m");
        delay(100); 
        i++;
    };

    return 0;
}

