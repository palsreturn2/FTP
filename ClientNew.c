#include "client.h"
#include "ClientFunc.c"
int main(int argc, char *argv[])
{
	int sockfd, numbytes;
	char buf[PACKETSIZE];
	struct hostent *he;
	char in[PACKETSIZE];
	
	struct sockaddr_in their_addr; // connector’s address information
	if (argc != 2) 
	{
		fprintf(stderr,"usage: client hostname\n");
		exit(1);
	}	
	if ((he=gethostbyname(argv[1])) == NULL) 
	{
		perror("gethostbyname");
		exit(1);
	}
// get the host info
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
	{
		perror("socket");
		exit(1);
	}
	their_addr.sin_family = AF_INET;
	// host byte order
	their_addr.sin_port = htons(PORT); // short, network byte order
	their_addr.sin_addr = *((struct in_addr *)he->h_addr);
	memset(&(their_addr.sin_zero),'\0',8); // zero the rest of the struct
	if (connect(sockfd, (struct sockaddr *)&their_addr,sizeof(struct sockaddr)) == -1) 
	{
		perror("connect");
		exit(1);
	}
	char input[PACKETSIZE];
	int i;
	int no;
	while(1)
	{
		//**********************This is the client UI program***********************
		printf("myftp>");
		for(i=0;i<PACKETSIZE && (input[i]=getchar())!='\n';i++);
		input[i]='\0';
		char **k=getKeywords(input);
		
		//printf("%s\n",input);
		if(sendData(sockfd,input,strlen(input))==-1)
		perror("send");
		if(strcmp(k[0],"mput")==0 && strcmp(k[1],"*.*")==0)
		{
			sendFiles(sockfd);
		}
		/*else if(strcmp(k[0],"mput")==0 && k[1][0]=='*')
		{
			sendFilesdot(k[1],sockfd);
		}*/
		//clearBuffer();
		waitTillInput;
		buf[numbytes]='\0';
		
		//printf("%s\n",buf);
		parseCommand(sockfd,buf);
	}
}
	
		
		
		
		
		
		
		
		
		
		
