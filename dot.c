#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

int main(){
    int i;
    while(true){
        sleep(1);
        printf("\033[1;32m");
        printf(".");
        printf("\033[0m");
        fflush(stdout);  // Force the output to be printed
    };

    return 0;
}
