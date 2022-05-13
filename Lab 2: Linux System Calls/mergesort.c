#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdbool.h>

void merge(int arr[],int l,int mid,int r){
    int left[mid-l+1];
    int right[r-mid];
    int pos1 = 0,pos2 = 0, pos=l;
    for(int i=l;i<=mid;i++){
        left[pos1++] = arr[i];
    }
    for(int i=mid+1;i<=r;i++){
        right[pos2++] = arr[i];
    }
    pos2=0;pos1=0;
    while(pos1<(mid-l+1) && pos2<(r-mid)){
        if(left[pos1]<=right[pos2]){
            arr[pos++] = left[pos1++];
        }
        else{
            arr[pos++] = right[pos2++];
        }
    }
    while(pos1<(mid-l+1) ){
        arr[pos++] = left[pos1++];
    }
    while(pos2<(r-mid)){
        arr[pos++] = right[pos2++];
    }
}
void mergeSort(int arr[],int l,int r){
    int n = (r-l)+1;
    if(n>=2){
        int mid=(l+r)/2;
        pid_t id1 = fork(),id2;
        if(id1==0){
            mergeSort(arr,l,mid); 
            _exit(0);  
            
        }
        else{
            id2 = fork();
            if(id2 ==0){
                mergeSort(arr,mid+1,r);
                _exit(0); 
            }
        }
        int status;
        waitpid(id1, &status, 0);
        waitpid(id2, &status, 0);
        merge(arr,l,mid,r);
    }
    return;
}
bool issorted(int arr[],int n){
    for(int i=1;i<n;i++){
        if(arr[i]<arr[i-1]){
            return false;
        }
    }
    return true;

}
int main(int argc,char* argv[]){
    int* arr; 
    FILE* fp;
    /*int arr[100];
    for(int i=0;i<100;i++){
        arr[i] = rand()%10000;
    }
    fp = fopen("input.txt", "w");
    for(int i=0;i<100;i++){
        fprintf(fp,"%d ",arr[i]);
    }
    fclose(fp);*/
    int shmid;
    key_t key = IPC_PRIVATE;
    int l = 100;
    size_t SHM_SIZE = sizeof(int)*l;
    if ((shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666)) < 0){
        perror("shmget");
        _exit(1);
    }
    if ((arr = shmat(shmid, NULL, 0)) == (int *) -1){
        perror("shmat");
        _exit(1);
    }
    fp = fopen(argv[1],"r");
    fseek(fp,0,SEEK_END);
    int size = ftell(fp);
    fclose(fp);
    if(size==0){
        printf("File is empty");
    }
    else{
        fp = fopen(argv[1],"r");
        for(int i=0;i<100;i++){
            fscanf(fp,"%d",arr+i);
        }
        fclose(fp);
        if(issorted(arr,100)){
            printf("Its already sorted");
        }
        else{
            mergeSort(arr,0,99);
            fp = fopen(argv[2],"w");
            for(int i=0;i<100;i++){
                fprintf(fp,"%d ",arr[i]);
            }
            fclose(fp);
            if (shmdt(arr) == -1){
                perror("shmdt");
                _exit(1);
            }
            if (shmctl(shmid, IPC_RMID, NULL) == -1){
                perror("shmctl");
                _exit(1);
            }
        }
    }
    
    return 0;
}