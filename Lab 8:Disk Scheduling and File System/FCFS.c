#include<stdio.h>
#include <stdlib.h>
#define fr(a,b) for(int i = a; i < b; i++)
#define rep(i,a,b) for(int i = a; i < b; i++)

typedef struct pair{
    int a,b;
}pair;

int comparator(const void *p, const void *q) {
    int l = ((pair *)p)->b;
    int r = ((pair *)q)->b; 
    return (l - r);
}

int main(){
    int requestSize;
    int head;
    int cylinder;

    printf("Enter the no. of cylinder: \n");
    scanf("%d",&cylinder);

    printf("Enter request sequence size: ");
    scanf("%d", &requestSize);

    int inputSequence[requestSize];
    int arrival_time[requestSize];
    fr(0,requestSize){
        printf("Enter the cylinder no. : \n");
        scanf("%d",inputSequence+i);
        printf("Enter the arrival time of this cylinder request : \n");
        scanf("%d",arrival_time+i);
    }
    printf("Enter initial head position: ");
    scanf("%d",&head);

    pair req[requestSize];
    fr(0,requestSize){
        req[i].a = inputSequence[i];
        req[i].b = arrival_time[i];
    }
    int size = sizeof(req) / sizeof(req[0]);
    qsort((void*)req, size, sizeof(req[0]), comparator);


    printf("The sequence of request to be executed is : ");
    fr(0,requestSize){
        printf("%d ",req[i].a);
    }
    printf("\n");

    int waitingTime[cylinder];
    int turnAroundTime[cylinder];

    int time = req[0].b;

    int avg_wait_time = 0;
    int avg_tat = 0;
    int total_cylinder_movement = 0;
    fr(0,requestSize){
        if(time>=req[i].b){
        }
        else{
            time = req[i].b;
        }
        time += abs(req[i].a - head);
        total_cylinder_movement += abs(req[i].a - head);
        head = req[i].a;
        waitingTime[req[i].a] = time - req[i].b;
        avg_wait_time += waitingTime[req[i].a];
        time += 100;
        turnAroundTime[req[i].a] = time - req[i].b;
        avg_tat += turnAroundTime[req[i].a];
    }
    printf("Wait time for processes: \n");
    fr(0,requestSize){
        printf("Process %d : %d\n",req[i].a,waitingTime[req[i].a]);
    }

    avg_wait_time/=requestSize;
    avg_tat /= requestSize;

    printf("average wait time is : %d\n",avg_wait_time);

    printf("Turnaround time for processes: \n");
    fr(0,requestSize){
        printf("Process %d : %d\n",req[i].a,turnAroundTime[req[i].a]);
    }

    printf("average turn around time is : %d\n",avg_tat);

    printf("Total cylinder movement : %d\n",total_cylinder_movement);
}