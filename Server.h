#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <dirent.h>

#define PACKETSIZE 100 // max number of bytes we can get at once
#define MYPORT 3490 // the port users will be connecting to
#define BACKLOG 10 // how many pending connections queue will hold
#define MESSAGESENDING "recieve data"
#define MESSAGERECIEVING "send data"
#define STOPSENDING "stop sending"
#define ERROR "Error has occured"
#define LS "startls"
#define LLS "startlls"
#define CD "Directory_changed"
#define LCD "Client_Directory_Changed"
#define GET "get"
#define MGET "mget"
#define PUT "put"
#define MPUT "mput"

#define waitTillInput while((numbytes=recv(new_fd, buf, PACKETSIZE-1, 0))==0 || numbytes==-1)
#define sendData(new_fd,str,len) send(new_fd, str, len, 0)

void ls();
void sendDataForCommand(int,char *);
char **getKeywords(char *str);
void sendPacket(int new_fd,char *packet);

char buf[PACKETSIZE];
int numbytes;

int wordCount(char *k)
{
	int lc,lk=0,i;
	char s[50];
	strcpy(s,k);
	strcat(s," ");
	lc=strlen(s);
	for(i=0;s[i]!='\0';i++)
	{
		if(s[i]==' ')
		lk++;
	}
	
	return lk;
}
char *substring(char *s,int start,int end)
{
	char *w=(char *)malloc(sizeof(char)*(end-start+1));
	int c=0,i;
	for(i=start;i<end;i++)
	{
		w[c]=s[i];
		c++;
	}
	w[c]='\0';
	return w;
}
char **getKeywords(char *str)
{
	int lc,i,k=0,lk=0,s=0,c=0;
	char **keyw;
	char com[50];
	strcpy(com,str);
	lk=wordCount(com);
	strcat(com," ");
	lc=strlen(com);
	//printf("%d",lk);
	//exit(1);
	keyw=(char **)malloc(lk*sizeof(char *));
	s=0;
	c=0;
	k=0;
	for(i=0;com[i]!='\0';i++)
	{
		if(com[i]==' ')
		{
			keyw[c]=substring(com,s,i);
			c++;
			s=i+1;
		}
	}
	return keyw;
}
int command(char *key)
{
	if(strcmp(key,"ls")==0)
	return 1;
	else if(strcmp(key,"lls")==0)
	return 2;
	else if(strcmp(key,"cd")==0)
	return 3;
	else if(strcmp(key,"lcd")==0)
	return 4;
	else if(strcmp(key,"get")==0)
	return 5;
	else if(strcmp(key,"mget")==0)
	return 6;
	else if(strcmp(key,"put")==0)
	return 7;
	else if(strcmp(key,"mput")==0)
	return 8;
	else if(strcmp(key,"exit")==0)
	return 9;
	else
	return -1;
}
void clearBuffer()
{
	int i=0;
	for(i=0;i<PACKETSIZE;i++)
	buf[i]='\0';
	//printf("Buffer Cleared\n");
}
int sendall(int s, char *buf, int *len)
{
	int total = 0;

	int bytesleft = *len;
	int n;
	while(total < *len) 
	{
		n = sendData(s, buf+total, bytesleft);
		if (n == -1) 
		{
			break; 
		}
		total += n;
		bytesleft -= n;
	}
	*len = total; // return number actually sent here
	return n==-1?-1:0; // return -1 on failure, 0 on success
}

	
