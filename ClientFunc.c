
void ls(int sockfd)
{
	FILE *f=fopen("listoffiles","w");
	if(sendData(sockfd,MESSAGERECIEVING,strlen(MESSAGERECIEVING))==-1)
	perror("send");
	printf("List of files in the directory of the server\n");
	//clearBuffer();
	while(1)
	{
		waitTillInput;
		buf[numbytes]='\0';
		//puts(buf);
		if(buf[0]=='1')
		{
			puts(buf+1);
			fwrite(buf+1,1,numbytes-1,f);
			break;
		}
		puts(buf+1);
		fwrite(buf+1,1,numbytes-1,f);
		//clearBuffer();
		if(sendData(sockfd,MESSAGERECIEVING,strlen(MESSAGERECIEVING))==-1)
		perror("send");
	}
	clearBuffer();
	if(sendData(sockfd,MESSAGERECIEVING,strlen(MESSAGERECIEVING))==-1)
	perror("send");
	fclose(f);
	return;
}
void lls(int sockfd)
{
	DIR *dp;
  	struct dirent *ep;     
	dp = opendir ("./");
	if (dp != NULL)
  	{
    	while (ep = readdir (dp))
   		puts (ep->d_name);
		puts("\t");
   		(void) closedir (dp);
  	}
  	else
    perror ("Couldn't open the directory");
    if(sendData(sockfd,MESSAGERECIEVING,strlen(MESSAGERECIEVING))==-1)
    perror("send");
  	return;
}
void get(int sockfd)
{
	if(sendData(sockfd,MESSAGERECIEVING,strlen(MESSAGERECIEVING))==-1)
	perror("send");
	waitTillInput;
	buf[numbytes]='\0';
	//printf("%d",numbytes);
	FILE *f=fopen(buf,"w");
	if(f==NULL)
	{
		printf("cannot open file");
		exit(1);
	}
	if(sendData(sockfd,MESSAGERECIEVING,strlen(MESSAGERECIEVING))==-1)
	perror("send");
	clearBuffer();
	while(1)
	{
		waitTillInput;
		//buf[numbytes]='\0';
		//puts(buf);
		if(buf[0]=='1')
		{
			fwrite(buf+1,1,numbytes-1,f);
			break;
		}
		fwrite(buf+1,1,numbytes-1,f);
		clearBuffer();
		if(sendData(sockfd,MESSAGERECIEVING,strlen(MESSAGERECIEVING))==-1)
		perror("send");
	}
	clearBuffer();
	if(sendData(sockfd,MESSAGERECIEVING,strlen(MESSAGERECIEVING))==-1)
	perror("send");
	fclose(f);
	return;
}
void put(int sockfd)
{
	int filled=1;
	if(sendData(sockfd,MESSAGESENDING,strlen(MESSAGESENDING))==-1)
	perror("send");
	waitTillInput;
	buf[numbytes]='\0';
	//puts(buf);
	FILE *f=fopen(buf,"r");
	clearBuffer();
	while(!feof(f))
	{
		if(filled+1<PACKETSIZE)
		{
			buf[filled]=fgetc(f);
			filled++;
			//buf[0]='0';
		}
		else
		{
			buf[0]='0';
			if(sendData(sockfd,buf,filled)==-1)
			perror("send");
			waitTillInput;
			buf[numbytes]='\0';
			//puts(buf);
			clearBuffer();
			filled=1;
		}
	}
	if(filled<PACKETSIZE)
	{
		buf[0]='1';
		if(sendData(sockfd,buf,filled)==-1)
		perror("send");
		waitTillInput;
		clearBuffer();
	}
	fclose(f);
	return;
}
void mget(int sockfd)
{
	if(sendData(sockfd,MESSAGERECIEVING,strlen(MESSAGERECIEVING))==-1)
	perror("send");
	waitTillInput;
	buf[numbytes]='\0';
	do
	{
		get(sockfd);
		clearBuffer();
		//if(sendData(sockfd
		waitTillInput;
		buf[numbytes]='\0';
		//puts(buf);
	}
	while(strcmp(buf,GET)==0);
	return;
}
void mput(int sockfd)
{
	if(sendData(sockfd,MESSAGESENDING,strlen(MESSAGESENDING))==-1)
	perror("send");
	waitTillInput;
	buf[numbytes]='\0';
	//puts(buf);
	do
	{
		put(sockfd);
		clearBuffer();
		waitTillInput;
		buf[numbytes]='\0';
		//puts(buf);
	}
	while(strcmp(buf,PUT)==0);
	return;
}
void lcd(int sockfd)
{
	if(sendData(sockfd,MESSAGERECIEVING,strlen(MESSAGERECIEVING))==-1)
	perror("send");
	waitTillInput;
	buf[numbytes]='\0';
	printf("Directory changed to");
	puts(buf);
	if(chdir(buf)==-1)
	{
		printf("Error:Could not change to dir");
	}
	if(sendData(sockfd,STOPSENDING,strlen(STOPSENDING))==-1)
	perror("send");
	return;
}
void parseCommand(int sockfd,char *com)
{
	char **keywords=getKeywords(com);
	int ch=command(keywords[0]);
	//printf("%d",ch);
	switch(ch)
	{
		//*****************************************************************************************************************//
		case 1:
		ls(sockfd);
		break;
		//*****************************************************************************************************************//
		case 2:
		printf("List of files in the directory of the client\n");
		lls(sockfd);
		break;
		case 3:
		puts(com);
		if(sendData(sockfd,MESSAGERECIEVING,strlen(MESSAGERECIEVING))==-1)
		perror("send");
		break;
		case 4:
		lcd(sockfd);
		break;
		//*****************************************************************************************************************//
		case 5:
		get(sockfd);
		break;
		case 6:
		mget(sockfd);
		break;
		case 7:
		put(sockfd);
		break;
		case 8:
		mput(sockfd);
		break;
		case 9:
		printf("Client closed\n");
		exit(0);
		default:
		printf("wrong message\n");
	}
}
void sendFiles(int sockfd)
{
	waitTillInput;
	buf[numbytes]='\0';
	puts(buf);
	clearBuffer();
	int n,i;
	char **k=getK(&n);
	for(i=1;i<n;i++)
	{
		if(sendData(sockfd,k[i],strlen(k[i]))==-1)
		perror("send");
		waitTillInput;
		clearBuffer();
	}
	if(sendData(sockfd,STOPSENDING,strlen(STOPSENDING))==-1)
	perror("send");
	return;
}
void sendFilesdot(char *in,int sockfd)
{
	waitTillInput;
	buf[numbytes]='\0';
	puts(buf);
	clearBuffer();
	int n,i;
	char **k=getKK(&n,in);
	for(i=1;i<n;i++)
	{
		if(sendData(sockfd,k[i],strlen(k[i]))==-1)
		perror("send");
		waitTillInput;
		clearBuffer();
	}
	if(sendData(sockfd,STOPSENDING,strlen(STOPSENDING))==-1)
	perror("send");
	return;
}
