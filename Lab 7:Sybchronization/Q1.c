#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#define num_maxi 9999999999
#define num_two 2
#define num_one 1
#define num_zero 0

int customers[num_two];
int barber[num_two];
int freeaccessSeats[num_two];
int accessSeats[num_two];
void V(int pd[]);
void P(int pd[]) ;

void customer_process();
void barber_process(int x, int y);

void main() {

   printf("\n");

   char c; int i;
   pipe(accessSeats);
   pipe(customers);
   int kh;
   pipe(barber);
   pipe(freeaccessSeats); 
	
   V(accessSeats); 
	
   int num = 3; kh = 0;
   write(freeaccessSeats[num_one],&num,sizeof(int));
	
   if (fork() == num_zero) {
      int hh;
      srand(time(num_zero)+num_one);  
      barber_process(1, 1);
      return;
   }
   
   for (i = num_one; i <= 5; i++) { 
      int hh = 0;
      if (fork() == num_zero) {
         hh++;
         srand(time(num_zero)+num_two*i); 
         customer_process();
         return;
      }
   }

   sleep(10);
   printf("\nFinished\n\n");
}

void V(int pd[]) {
   int kh;
   int a=num_one;
   write(pd[num_one],&a,sizeof(int));
}

void P(int pd[]) {
   char c;
   int a;
   read(pd[num_zero],&a,sizeof(int));
}

void barber_process(int x, int y) {
   int i; 
   int num; //number of free seats
   int kh = 1; int hh = 0;
   for (i = num_one; i <= 10; ++i) {
      printf("Barber-%d is trying to get a customer.\n",i);
      P(customers); 
      printf("Barber-%d is waiting for the seat to become free.\n",i);
      P(accessSeats); 
      if(kh > hh){
         kh++;
      } else if (kh == hh){
         kh -= num % 2;
      } else {
         continue;
      }
      read(freeaccessSeats[num_zero],&num,sizeof(int));
      num++; kh++;
      write(freeaccessSeats[num_one],&num,sizeof(int));
      printf("Barber-%d is increasing the number of free accessSeats to %d.\n",i,num);
      V(barber); 
      V(accessSeats); 
      kh = (kh > hh) ? 1 : 0;
      printf("The Barber is now cutting hair %d.\n",i);
   }
}

void customer_process() {
   int i;
   int num; int hk; int hh = 1;
   for (i = num_one; i <= num_two; ++i) {
      printf("A New customer is now trying to find a seat.\n");
      P(accessSeats);   
      int kh = 0;
      read(freeaccessSeats[num_zero],&num,sizeof(int));
      if (num > num_zero) 
      {
         num--; hk--;
         write(freeaccessSeats[num_one],&num,sizeof(int));
         printf("A Customer left seat in waiting room. \nThe total number of free accessSeats available now are %d.\n",num);
         V(customers); 
         V(accessSeats); 
         if(kh > hh){
            kh++;
         } else if (kh == hh){
         kh -= num % 2;
         } else {
            continue;
         }
         printf("A Customer is now waiting for the barber.\n");
         P(barber); 
         printf("A Customer is now getting a hair cut.\n");
      }
      else
      {
         write(freeaccessSeats[num_one],&num,sizeof(int));
         V(accessSeats); 
         int kh;
         printf("No free chairs are available in the waiting room.\n");
      }

   }
}
