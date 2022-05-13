#include <stdio.h> 
#include <string.h>   
#include <stdlib.h> 
#include <errno.h> 
#include <unistd.h>   
#include <arpa/inet.h>    
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <sys/time.h> 
#include <netdb.h>
#include <stdbool.h>
#include <ctype.h>
#include <arpa/inet.h>
     
#define TRUE   1 
#define FALSE  0 
#define PORT 8888 
     
int main(int argc , char *argv[])  
{  
    int opt = TRUE;  
    int masterSocket, addrlen, newSocket, clientSocket[30], max_clients = 30 , act, i , valread , sd;  
    int max_sd;  
    struct sockaddr_in address;  
         
    char buffer[1024];  
         
    int portno = atoi(argv[1]);
    fd_set readfds;  
         
    char *message = "ECHO Daemon v1.0 \r\n";  
     
    for (i = 0; i < max_clients; i++){  
        clientSocket[i] = 0;  
    }  
         
    if( (masterSocket = socket(AF_INET , SOCK_STREAM , 0)) == 0)  
    {  
        perror("socket failed");  
        exit(EXIT_FAILURE);  
    }  
     
  
    if( setsockopt(masterSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, 
          sizeof(opt)) < 0 )  
    {  
        perror("setsockopt");  
        exit(EXIT_FAILURE);  
    }  
     
  
    address.sin_family = AF_INET;  
    address.sin_addr.s_addr = INADDR_ANY;  
    address.sin_port = htons( portno );  
         
    
    if (bind(masterSocket, (struct sockaddr *)&address, sizeof(address))<0)  
    {  
        perror("bind failed");  
        exit(EXIT_FAILURE);  
    }  
    printf("Listener on port %d \n", portno);  
         
    
    if (listen(masterSocket, 3) < 0)  
    {  
        perror("listen");  
        exit(EXIT_FAILURE);  
    }  
         
    
    addrlen = sizeof(address);  
    puts("Waiting for connections ...");       
    while(TRUE){  
       
        FD_ZERO(&readfds);  
        
        FD_SET(masterSocket, &readfds);  
        max_sd = masterSocket;  
        
        for ( i = 0 ; i < max_clients ; i++)  
        {  
            
            sd = clientSocket[i];  
                 
            if(sd > 0)  
                FD_SET( sd , &readfds);  
                 
            
            if(sd > max_sd)  
                max_sd = sd;  
        }  
     
        
        act = select( max_sd + 1 , &readfds , NULL , NULL , NULL);  
       
        if ((act < 0) && (errno!=EINTR)){  
            printf("select error");  
        }  
             
        
        if (FD_ISSET(masterSocket, &readfds))  
        {  
            if ((newSocket = accept(masterSocket, 
                    (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)  
            {  
                perror("accept");  
                exit(EXIT_FAILURE);  
            }  
             
  
            for (i = 0; i < max_clients; i++)  {  
                if( clientSocket[i] == 0 )  {  
                    clientSocket[i] = newSocket;  
                    printf("Running Thread Number %d\n" , i+1);  
                         
                    break;  
                }  
            }  
        }      
        for (i = 0; i < max_clients; i++)  
        {  
            sd = clientSocket[i];  
                 
            if (FD_ISSET( sd , &readfds))  
            {  
                
                if ((valread = read( sd , buffer, 1024)) == 0)  
                {  
                    getpeername(sd , (struct sockaddr*)&address , \
                        (socklen_t*)&addrlen);  
                    printf("Host disconnected , ip %s , port %d \n" , 
                          inet_ntoa(address.sin_addr) , ntohs(address.sin_port));  
                         
                   
                    close( sd );  
                    clientSocket[i] = 0;  
                }
                else 
                {  
                    int pos = 0;
                    char num1[12];int pos1=0;
                    while(isdigit(buffer[pos])){
                        num1[pos1] = buffer[pos];
                        pos1++;pos++;
                    }
                    num1[pos1] = '\0';
                    int first = atoi(num1);
                    char op = buffer[pos];
                    pos++;pos1 = 0;
                    char num2[12];
                    while(isdigit(buffer[pos])){
                        num2[pos1] = buffer[pos];
                        pos1++;pos++;
                    }
                    num2[pos1] = '\0';
                    int sec = atoi(num2);
                    int ans;
                    if(op == '+'){
                        ans = first + sec;
                    }
                    else if(op == '-'){
                        ans = first - sec;
                    }
                    else if(op == '*'){
                        ans = first * sec;
                    }
                    else if(op == '/'){
                        ans = first / sec;
                    }
                    if(ans==0){
                        buffer[0] = '0';
                        buffer[1] = '\0';
                    }
                    else{
                        pos=0;
                        while(ans!=0){
                            buffer[pos] = (ans%10) + '0';
                            ans/=10;
                            pos++;
                        }
                        int l =0;
                        int r = pos-1;
                        while(l<r){
                            char temp = buffer[l];
                            buffer[l] = buffer[r];
                            buffer[r] = temp;
                            l++;r--;
                        }
                        buffer[pos]='\0';
                    } 
                    send(sd , buffer , strlen(buffer) , 0 );  
                    bzero(buffer, sizeof(buffer));
                }  
            }  
        }   
    }  
         
    return 0;  
}  