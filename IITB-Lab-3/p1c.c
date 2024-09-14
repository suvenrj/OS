#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

void handle_signal_child(int signal_number){
    if(signal_number == SIGCHLD){
        printf("GrandChild Exited\n");
    }
}

void handle_signal_parent(int signal_number){
    if(signal_number == SIGCHLD){
        printf("Child Exited\n");
    }
}

int main(){
    printf("Parent pid is %d\n", getpid());
    signal(SIGCHLD, handle_signal_parent);
    int pid = fork();
    if (pid==0){
        signal(SIGCHLD, handle_signal_child);
        int pid_1 = fork();
        if (pid_1==0){
            sleep(30);
            exit(0);
        }
        else{
            printf("Grand-Child pid is %d\n", pid_1);
        }
        sleep(20);
        exit(0);
    }
    else{
        printf("Child pid is %d\n", pid);
    }
    sleep(30);
    printf("Parent Exited\n");
    exit(0);
}