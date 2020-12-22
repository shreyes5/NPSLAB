//1. include header files
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>

#include<sys/wait.h>
#include<ctype.h>

#define max_lim 500
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


	recb=recv(ns,buff,sizeof(buff),0);
	if(recb==-1){printf("\nMessage Recieving Failed");close(s); close(ns); exit(0);}
	printf("\nEntered string: %s",buff);
	
	int n=strlen(buff),ff,i,j,ac=0,nc=0;
	char alpha[max_lim],num[max_lim],ch;
	for(i=0;i<n;i++){
		if(isdigit(buff[i])!=0){nc++;num[nc]=buff[i];}
		else{ac++;alpha[ac]=buff[i];}
	}
	
	ff=fork();
	
	if(ff==0){
		for(i=1;i<nc;i++)
			for(j=0;j<nc-i;j++)
				if(num[j]>num[j+1]){ch=num[j]; num[j] = num[j+1]; num[j+1]=ch;}
		sprintf(buff,"Output at the child process of the server: %s",num);
		sntb=send(s,buff,sizeof(buff),0);
		if(sntb==-1){printf("\nMessage Sending Failed");close(s); exit(0);}
	}
	else{
		for(i=1;i<ac;i++)
			for(j=0;j<ac-i;j++)
				if(alpha[j]<alpha[j+1]){ch=alpha[j]; alpha[j] = alpha[j+1]; alpha[j+1]=ch;}
		sprintf(buff,"Output at the parent process of the server: %s",num);
		sntb=send(s,buff,sizeof(buff),0);
		if(sntb==-1){printf("\nMessage Sending Failed");close(s); exit(0);}
	}
	
	wait(NULL);
	printf("\n");
	close(ns); close(s);
}
