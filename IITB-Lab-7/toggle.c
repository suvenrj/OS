#include "types.h"
#include "stat.h"
#include "user.h"

int main(){
    init_counters();
    acquire_lock(1);
    int pid = fork();
    for (int i=0; i<30; i++){
        if (pid==0){
            acquire_lock(2);
            acquire_lock(3);
            set_var(3, get_var(3)+1);
            printf(1, "child value %d\n", get_var(3));
            release_lock(3);
            release_lock(1);
        }
        else{
            acquire_lock(1);
            acquire_lock(3);
            set_var(3, get_var(3)-1);
            printf(1, "parent value %d\n", get_var(3));
            release_lock(3);
            release_lock(2);
        }
    }
    if (pid!=0){
        exit();
    }
    else{
        wait();
    }
    exit();
}