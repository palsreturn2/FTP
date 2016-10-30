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


#define PORT 3490 // the port client will be connecting to
#define PACKETSIZE 100 // max number of bytes we can get at once
#define max 100
#define MESSAGESENDING "recieve data"
#define MESSAGERECIEVING "send data"
#define STOPSENDING "stop sending"
#define LS "startls"
#define LLS "startlls"
#define CD "Directory_changed"
#define LCD "Client_Directory_Changed"
#define GET "get"
#define MGET "mget"
#define PUT "put"
#define MPUT "mput"

#define sendData(sockfd,str,len) send(sockfd, str, len, 0)
#define waitTillInput while((numbytes=recv(sockfd, buf, PACKETSIZE-1, 0))==0 || numbytes==-1)
#define RecievedData recv(sockfd, buf, PACKETSIZE-1, 0))

char **getKeywords(char *str);
void ls();

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
	if(strcmp(key,LS)==0)
	return 1;
	else if(strcmp(key,LLS)==0)
	return 2;
	else if(strcmp(key,CD)==0)
	return 3;
	else if(strcmp(key,LCD)==0)
	return 4;
	else if(strcmp(key,GET)==0)
	return 5;
	else if(strcmp(key,MGET)==0)
	return 6;
	else if(strcmp(key,PUT)==0)
	return 7;
	else if(strcmp(key,MPUT)==0)
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
char **getK(int *n)
{
	DIR *dp;
 	struct dirent *ep;
 	int i;
 	char flag='f';
 	char **k=(char **)malloc(sizeof(char *)*100);
  	for(i=0;i<100;i++)     
  	k[i]=(char *)malloc(sizeof(char)*100);
  	dp = opendir ("./");
	(*n)=1;
	
 	if (dp != NULL)
  	{
  		
    	while (ep = readdir (dp))
   		{
   			flag='f';
   			
   			for(i=0;ep->d_name[i]!=0;i++)
   			{
   				//printf("%c\n",ep->d_name[i]);
   				if(ep->d_name[i]!='.')
   				flag='t';
   				if(ep->d_name[i]=='.' && flag=='t')
   				{
   					strcpy(k[(*n)],ep->d_name);
   					
   					(*n)++;
   					break;
   				}
   			}
   		}
		(void) closedir (dp);
  	}
  	
  	else
    perror ("Couldn't open the directory");
    return k;
}
char **getKK(int *n,char *str)
{
	DIR *dp;
 	struct dirent *ep;
 	int i;
 	char flag='f';
 	char *s;
 	char **k=(char **)malloc(sizeof(char *)*100);
  	for(i=0;i<100;i++)     
  	k[i]=(char *)malloc(sizeof(char)*100);
  	dp = opendir ("./");
	(*n)=1;
	int l;
 	if (dp != NULL)
  	{
    	while (ep = readdir (dp))
   		{
   			l=strlen(ep->d_name);
   			flag='f';
   			for(i=1;i<=strlen(str);i++)
   			{
   				if(ep->d_name[l-i]!=str[strlen(str)-i])
   				{
   					flag='t';
   					break;
   				}
   			}
   			if(flag=='f')
   			{
   				strcpy(k[(*n)],ep->d_name);
   				(*n)++;
   			}
   		}
		(void) closedir (dp);
  	}
  	
  	else
    perror ("Couldn't open the directory");
    return k;
}
