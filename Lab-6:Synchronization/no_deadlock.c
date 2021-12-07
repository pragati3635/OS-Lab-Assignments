#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdbool.h>
#define fr(a,b) for(int i = a; i < b; i++)

sem_t room;
sem_t spoon[5];
bool eating[5];

void * philosopher(void * num){
	
int identity=*(int *)num;
	sem_wait(&room);
    printf("\nPhilospher %d is thinking",identity);
    sleep(2);
   
	/*if(!eating[(identity+1)%5] && !eating[(identity + 4) % 5]){
		sem_wait(&spoon[identity]);
        sem_wait(&spoon[(identity+1)%5]);
		eating[identity] = true;
	}
	else if(eating[(identity + 4) % 5]){
		sem_wait(&spoon[identity]);
	}
	else{
		sem_wait(&spoon[(identity+1)%5]);
	}*/
	sem_wait(&spoon[identity]);
	sem_wait(&spoon[(identity+1)%5]);
    printf("\nPhilosopher %d is has taken the spoon %d and %d",identity,identity,(identity+1)%5);
	printf("\nPhilosopher %d is eating",identity);
	sleep(4);
	printf("\nPhilosopher %d has finished eating",identity);
	sem_post(&spoon[(identity+1)%5]);
	sem_post(&spoon[identity]);
	//eating[identity] = false;
	
	sem_post(&room);
}

int main(){
	fr(0,5){
		eating[i] = false;
	}
	int i,a[5];
	pthread_t tid[5];
	
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



