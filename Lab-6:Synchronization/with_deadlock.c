#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#define fr(a,b) for(int i = a; i < b; i++)

sem_t room;
sem_t spoon[5];
bool left[5];
bool right[5];
int count = 0;

void * philosopher(void * num){
    srand ( time(NULL) );
	int identity=*(int *)num;

	sem_wait(&room);
    printf("\nPhilospher %d is thinking",identity);

    sleep((rand() % 5) + 1);
	sem_wait(&spoon[identity]);
    left[identity] = true;
    printf("\nPhilosopher %d is has taken the left spoon %d ",identity,identity);

    sleep(10);

	sem_wait(&spoon[(identity+1)%5]);
    right[identity]=true;
    printf("\nPhilosopher %d is has taken the right spoon %d",identity,(identity+1)%5);

	printf("\nPhilosopher %d is eating",identity);
	sleep((rand() % 5) + 1);
	printf("\nPhilosopher %d has finished eating",identity);

	sem_post(&spoon[(identity+1)%5]);
    right[identity]= false;
	sem_post(&spoon[identity]);
    left[identity] = false;
	sem_post(&room);
    
}

void * detect_deadlock(void * num){
    srand ( time(NULL) );
    while(count<1){
        bool indi = true;
        fr(0,5){
            if(left[i]==true && right[i]==false){

            }
            else{
                indi = false;
            }
        }
        // If there is deadlock
        if(indi){
            printf("\nParent detects deadlock going to initiate recovery");
            int philo = rand() % 5;
            sem_post(&spoon[philo]);
            left[philo] = false;
            printf("\nParnet prempts %d (releases %d left fork)",philo,philo);
            count++;
        }
    }
}

int main(){
	fr(0,5){
        left[i] = false;
        right[i] = false;
	}
	int i,a[5];
    int x=2;
	pthread_t tid[5];
    pthread_t dl;
    pthread_create(&dl,NULL,detect_deadlock,(void *)&x);
	
	sem_init(&room,0,5);
	
	fr(0,5){
		sem_init(&spoon[i],0,1);
	}	
	fr(0,5){
		a[i]=i;
		pthread_create(&tid[i],NULL,philosopher,(void *)&a[i]);
	}
	fr(0,5){
        pthread_join(tid[i],NULL);
    }		
}
