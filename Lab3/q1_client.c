//1. include header files
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <sys/stat.h>

#include<sys/wait.h>

#define max_lim 100
int main(){
	int s,r,recb,sntb,x;
	printf("INPUT port number:");
	scanf("%d",&x);
	struct sockaddr_in server;
	char buff[max_lim];

	//2. create socket
	s=socket(AF_INET,SOCK_STREAM,0);
	if(s==-1){printf("\nSocket creation error.");exit(0);}
	printf("\nSocket created.");

	//3. initialize server
	server.sin_family=AF_INET;
	server.sin_port=htons(x);
	server.sin_addr.s_addr=inet_addr("127.0.0.1");
	
	//4. connect to server
	r=connect(s,(struct sockaddr*)&server,sizeof(server));
	if(r==-1){printf("\nConnection error.");exit(0);}
	printf("\nSocket connected.\n");
	printf("\n");
	
	int c;
	while((c = getchar()) != '\n' && c != EOF);

	int ff=fork();

	while(1){
		if(ff==0){
			//send msg to server
			scanf("%[^\n]%*c",buff);
			sntb=send(s,buff,sizeof(buff),0);
			if(sntb==-1){printf("\nMessage Sending Failed");close(s); exit(0);}
			if(!strcmp(buff,"BYE")){close(s);kill(getppid(),9);break;}
		}
	
		else{
			//recive msg from server
			recb=recv(s,buff,sizeof(buff),0);
			if(recb==-1){printf("\nReply Recieving Failed");close(s); exit(0);}
			printf("\nServer: %s",buff);
			if(!strcmp(buff,"BYE")) break;
		}
	}
	printf("\n\n");
	close(s);
}
