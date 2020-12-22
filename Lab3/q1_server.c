//1. include header files
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>

#include<sys/wait.h>

#define max_lim 100
int main(){
	int s,r,recb,sntb,x,ns,a=0;
	printf("INPUT port number:");
	scanf("%d",&x);
	socklen_t len;
	struct sockaddr_in server,client;
	char buff[max_lim];

	//2. create socket
	s=socket(AF_INET,SOCK_STREAM,0);
	if(s==-1){printf("\nSocket creation error.");exit(0);}
	printf("\nSocket created.");
	
	//3. initialize server
	server.sin_family=AF_INET;
	server.sin_port=htons(x);
	server.sin_addr.s_addr=htonl(INADDR_ANY);
	
	//4. bind address to socket
	r=bind(s,(struct sockaddr*)&server,sizeof(server));
	if(r==-1){printf("\nBinding error.");exit(0);}
	printf("\nSocket binded.");
	
	//5. make socket passive(announce willingness to accept connection) 
	r=listen(s,1);
	if(r==-1){close(s); exit(0);}
	printf("\nSocket listening.");
	len=sizeof(client);
	
	//6. block until connection request arrives from client
	ns=accept(s,(struct sockaddr*)&client,&len);
	if(ns==-1){close(s); exit(0);}
	printf("\nSocket accepting.\n");
	printf("\n");

	int c;
	while((c = getchar()) != '\n' && c != EOF);

	int ff=fork();
	
	while(1){
		if(ff==0){
			//recieve msg from client
			recb=recv(ns,buff,sizeof(buff),0);
			if(recb==-1){printf("\nMessage Recieving Failed");close(s); close(ns); exit(0);}
			printf("\nClient: %s",buff);
			if(!strcmp(buff,"BYE")){close(ns);close(s);kill(getppid(),9);break;}
		}
		else{
			//send reply to client
			scanf("%[^\n]%*c",buff);
			sntb=send(ns,buff,sizeof(buff),0);
			if(sntb==-1){printf("\nMessage Sending Failed");close(s); close(ns); exit(0);}
			if(!strcmp(buff,"BYE")) break;
		}
	}
	printf("\n");
	close(ns); close(s);
}
