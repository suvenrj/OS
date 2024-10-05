#include "types.h"
#include "stat.h"
#include "user.h"

int main(){
    init_counters();
    for (int i=0; i<10; i++){
        int pid = fork();
        if (pid==0){
            for(int j=0; j<1000; j++){
                acquire_lock(i);
                set_var(i, get_var(i)+1);
                release_lock(i);
            }
            exit();
        }
        else{
            wait();
        }
    }
    for (int j=0; j<1000; j++){
        for (int k=0; k<10; k++){
            acquire_lock(k);
            set_var(k, get_var(k)+1);
            release_lock(k);
        }
    }
    for (int k=0; k<10; k++){
        printf(1, "counter %d value %d\n", k, get_var(k));
    }
    exit();
}