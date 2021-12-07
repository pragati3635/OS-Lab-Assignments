#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdbool.h>
#include <ctype.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]){

	int sockfd, ret;
	 struct sockaddr_in serverAddr;

	int newSocket;
	struct sockaddr_in newAddr;

	socklen_t addr_size;

	char buffer[1024];
	pid_t childpid;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Server Socket is created.\n");
    int portno = atoi(argv[1]);
	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(portno);
    
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Error in binding.\n");
		exit(1);
	}
	printf("[+]Bind to port %s\n", argv[1]);

	if(listen(sockfd, 10) == 0){
		printf("[+]Listening....\n");
	}else{
		printf("[-]Error in binding.\n");
	}


	while(1){
		newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
		if(newSocket < 0){
			exit(1);
		}
		printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));

		if((childpid = fork()) == 0){
			close(sockfd);

			while(1){
				recv(newSocket, buffer, 1024, 0);
				if(strcmp(buffer, ":exit") == 0){
					printf("Disconnected from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
					break;
				}
				else{
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
					send(newSocket, buffer, strlen(buffer), 0);
					bzero(buffer, sizeof(buffer));
				}
			}
		}

	}

	close(newSocket);


	return 0;
}