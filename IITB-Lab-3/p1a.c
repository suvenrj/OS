#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

void handle_signal(int signal_number){
    if (signal_number==SIGINT){
        printf("Do you want to quit? (y/n)");
        char answer[5];
        scanf("%s", answer);
        if (answer[0]=='y'){
            exit(0);
        }
        else if (answer[0]=='n'){
            return;
        }
    }
    else if (signal_number==SIGTERM){
        printf("Do you want to quit? (y/n)");
        char answer[5];
        scanf("%s", answer);
        if (answer[0]=='y'){
            exit(0);
        }
        else if (answer[0]=='n'){
            return;
        }
    }
}

int main(){
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);
    while (1){
    }
}