// Taking arrival time same
// and assuming movement is from head towards last cylinder then backwards t0 0 then from 0 to cylinder req in increasing order
#include<stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#define fr(a,b) for(int i = a; i < b; i++)
#define rep(i,a,b) for(int i = a; i < b; i++)

typedef struct pair{
    int a,b;
    bool visited;

}pair;

int comparator(const void *p, const void *q) {
    int l = ((pair *)p)->a;
    int r = ((pair *)q)->a; 
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

    printf("Enter the cylinder seq : \n");
    fr(0,requestSize){
        
        scanf("%d",inputSequence+i);
    }
    printf("Enter initial head position: ");
    scanf("%d",&head);

    pair req[requestSize];
    fr(0,requestSize){
        req[i].a = inputSequence[i];
        req[i].visited = false;
        req[i].b = 0;
    }
    int size = sizeof(req) / sizeof(req[0]);
    qsort((void*)req, size, sizeof(req[0]), comparator);

    int waiting_time[cylinder];
    int turn_around_time[cylinder];
    
    printf("The seq is : \n");
    int total_cylinder_movement = 0;
    int time =0;
    int avg_wt = 0;
    int avg_tat = 0;
    int count=0;
    int head1 = head;
    fr(0,requestSize){
        if(req[i].a>=head){
            time += (req[i].a - head1);
            waiting_time[req[i].a] = time - req[i].b;
            head1 = req[i].a;
            time += 100;
            turn_around_time[req[i].a] = time - req[i].b;
            avg_tat += turn_around_time[req[i].a];
            avg_wt += waiting_time[req[i].a];
            printf("Cylinder req %d executed with TAT %d and W.T %d\n",req[i].a,turn_around_time[req[i].a],waiting_time[req[i].a]);
        }
    }
    time += (cylinder-1 - 0);
    head1 = 0;
    int temp = -90;
    fr(0,requestSize){
        if(req[i].a<head){
            time += abs(req[i].a-head1);
            waiting_time[req[i].a] = time - req[i].b;
            head1 = req[i].a;
            time += 100;
            turn_around_time[req[i].a] = time - req[i].b;
            avg_tat += turn_around_time[req[i].a];
            avg_wt += waiting_time[req[i].a];
            printf("Cylinder req %d executed with TAT %d and W.T %d\n",req[i].a,turn_around_time[req[i].a],waiting_time[req[i].a]);
        }   
        else{
            if(i!=0 && temp == -90){
                temp = (i-1);
            }
        }
    }
    total_cylinder_movement += ((cylinder-1) - head);
    total_cylinder_movement += (cylinder-1);
    total_cylinder_movement += (temp != -90) ? (req[temp].a) : 0;
    printf("\nTotal cylinder movement is %d\n",total_cylinder_movement);

    avg_wt /= requestSize;
    avg_tat /= requestSize;

    printf("Average Waiting time : %d\n",avg_wt);
    printf("Average Turnaround time : %d\n",avg_tat);
    
}