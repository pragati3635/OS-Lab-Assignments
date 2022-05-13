#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main(){
    FILE* fp;
    int arr[100];
    for(int i=0;i<100;i++){
        arr[i] = rand()%10000;
        //arr[i] = i+1;
    }
    fp = fopen("input.txt", "w");
    for(int i=0;i<100;i++){
        fprintf(fp,"%d ",arr[i]);
    }
    fclose(fp);
}

//printf("Data read from memory: %s\n",str);

//detach from shared memory 
//shmdt(str);
    
    // destroy the shared memory
    //shmctl(shmid,IPC_RMID,NULL);