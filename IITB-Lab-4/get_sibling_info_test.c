#include "types.h"
#include "user.h"

int main(){
    for (int i=0; i<5; i++){
        int pid = fork();
        if (pid==0){
            if (i==4){
                get_sibling_info();
            }
            exit();
        }
    }
    wait();
    exit();
}