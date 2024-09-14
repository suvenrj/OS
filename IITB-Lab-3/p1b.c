#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

int n;
int proc_status[500];
int child_pid[500];
int count=0;

void handle_signal(int signal_number){
    if (signal_number==SIGCHLD){
        for (int i=0; i<n; i++){
            int status;
            pid_t result;
            result = waitpid(child_pid[i], &status, WNOHANG);
            if (result>0){
                if (proc_status[i]==0){
                    printf("Child with pid %d exited\n", child_pid[i]);
                    proc_status[i]=1;
                    count+=1;
                }
            }
        }
    }
}
int main(){
    signal(SIGCHLD, handle_signal);
    scanf("%d", &n);
    for (int i=0; i<n; i++){
        child_pid[i] = fork();
        proc_status[i]=0;
        if (child_pid[i]==0){
            srand(i);
            int low = 1;
            int high = 10;
            int random_number = low + rand() % (high - low + 1);
            printf("%d\n", random_number);
            sleep(random_number);
            exit(0);
        }
    }
    while(count!=n){
    }
    printf("All children have exited. Now parent exiting");
    exit(0);
}