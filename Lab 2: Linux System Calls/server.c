#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#define MAX_VAL 256

typedef struct message_buffer{
    long message_type;
    int round;
    char line[MAX_VAL];
}message;

typedef struct Client{
    int ID;
    int score;
    int question_ID;
}client;


int h_val(char* line);
message* createMessage(long message_type,char* m,int round);
client* createClient(int ID);

int main()
{
    // no. of clients
    int k;
    printf("Please specify no. of clients: ");
    scanf("%d",&k);
    client* keys[k];
    for(int i=0;i<k;i++){
        keys[i]= createClient(i+1);
    }
    int round=1;
    char line[MAX_VAL];
    message* x;
    message received;
    int scores[k];

    int index;

    while(true){
        printf("---Round %d ---\nSpecify question for this round : ",round);
        scanf("%s",line);
        index=0;
        x=createMessage(1,line,round);
        for(int i=0;i<k;i++){
            msgsnd(keys[i]->question_ID,x,sizeof(message),0);
            msgrcv(keys[i]->question_ID, &received, sizeof(received),2,0);
            printf("Reply from client %d is : %s\n",keys[i]->ID,received.line);
            scores[i]=abs(h_val(x->line)-h_val(received.line));
            if(scores[i]<scores[index])index=i;
        }


        keys[index]->score+=5;
        if(keys[index]->score>=50){
            printf("Winner for round %d is %d\n",round,index+1);
            printf("Current scores after round %d\n",round);
            for(int i=0;i<k;i++){
                printf("%d ",keys[i]->score);
            }
            printf("\n");

            printf("Wohooo, You are the winner! Client %d\n",index+1);
            for(int i=0;i<k;i++){
                x=createMessage(3,"Terminate",round);
                msgsnd(keys[i]->question_ID,x,sizeof(message),0);
            }
            printf("Works completed.\nTerminating...");
            break;
        }
        else{

            printf("Winner for round %d is %d\n",round,index+1);
            printf("Scores after round %d\n",round);
            for(int i=0;i<k;i++){
                x=createMessage(3,"Continue...",round);
                msgsnd(keys[i]->question_ID,x,sizeof(message),0);
                printf("%d ",keys[i]->score);
            }
            printf("\n");
            round++;
        }
    }
  
    return 0;
}

int h_val(char* line){
    int out=0;
    int i=0;
    while(line[i]!='\0'){
        out+=line[i];
        i++;
    }
    return out%i;
}

client* createClient(int ID){
    client* temp=(client*) malloc(sizeof(client));
    temp->question_ID=msgget(ftok("client.c",ID),0666 | IPC_CREAT);
    temp->ID=ID;
    temp->score=0;
    return temp;
}

message* createMessage(long message_type,char* m, int round){
    message* temp= (message*) malloc(sizeof(message));
    temp->message_type=message_type;
    temp->round=round;
    int i=0;
    while(m[i]!='\0'){
        temp->line[i]=m[i];
        i++;
    }
    temp->line[i]=m[i];
    return temp;
}
