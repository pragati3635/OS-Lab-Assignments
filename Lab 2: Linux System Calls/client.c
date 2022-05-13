#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#define MAX_VAL 256


typedef struct message_buffer {
    long message_type;
    int round;
    char line[MAX_VAL];
} message;
 
message* createMessage(long message_type,char* m, int round);

int main()
{
    
    char line[MAX_VAL];
    int question_ID,client_ID;
    printf("Specify Client ID: ");
    scanf("%d",&client_ID);

    question_ID = msgget(ftok("client.c", client_ID), 0666 | IPC_CREAT);
  
    message received;
    message* x;
    while (true){
        msgrcv(question_ID, &received, sizeof(received),1,0);
        printf("---Round %d ---\n Question received for round %d is: %s\n",received.round,received.round,received.line);
        printf("Specify your reply for this round: ");
        scanf("%s",line);
        x=createMessage(2,line,received.round);
        msgsnd(question_ID,x,sizeof(message),0);
        msgrcv(question_ID, &received, sizeof(received),3,0);
        if(received.line[0]=='T'){
            printf("Game Over\n Terminating...\n");
            msgctl(question_ID, IPC_RMID, NULL);
            break;
        }
    }
    return 0;
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