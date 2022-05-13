// Taking arrival time same
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

    int waiting_time[cylinder];
    int turn_around_time[cylinder];
    
    printf("The seq is : \n");
    int total_cylinder_movement = 0;
    int time =0;
    int avg_wt = 0;
    int avg_tat = 0;
    fr(0,requestSize){
        int process;
        int min_seek_time = INT_MAX;
        rep(j,0,requestSize){
            if(!req[j].visited){
                int seek_time = abs(req[j].a-head);
                if(seek_time<min_seek_time){
                    process = j;
                    min_seek_time = seek_time;
                }
            }
        }
        req[process].visited = true;
        
        total_cylinder_movement += abs(req[process].a-head);
        time += abs(req[process].a-head);
        waiting_time[req[process].a] = time - req[process].b;
        time += 100;
        turn_around_time[req[process].a] = time - req[process].b;
        head = req[process].a;
        printf("Cylinder req %d exe with W.T %d and TAT %d\n",req[process].a,waiting_time[req[process].a],turn_around_time[req[process].a]);
        avg_wt += waiting_time[req[process].a];
        avg_tat += turn_around_time[req[process].a];
    }
    printf("\nTotal cylinder movement is %d\n",total_cylinder_movement);

    avg_wt /= requestSize;
    avg_tat /= requestSize;

    printf("Average Waiting time : %d\n",avg_wt);
    printf("Average Turnaround time : %d\n",avg_tat);
    
}