void ls(int new_fd)
{
	DIR *dp;
  	struct dirent *ep;     
  	dp = opendir ("./");
	int filled=1;
  	if (dp != NULL)
  	{
    	while (ep = readdir (dp))
    	{
    		//puts (ep->d_name);
    		if((filled+strlen(ep->d_name))<PACKETSIZE)
    		{
    			strncpy(buf+filled,ep->d_name,strlen(ep->d_name));
    			filled+=strlen(ep->d_name);
    			buf[filled]=' ';
    			filled++;
    			buf[0]='0';
    		}
    		else
    		{
    			buf[0]='0';
    			
    			if(sendData(new_fd,buf,filled)==-1)
    			perror("send");
    			filled=1;
    			waitTillInput;
    			buf[numbytes]='\0';
    			//puts(buf);
    			if(strcmp(buf,MESSAGERECIEVING)!=0)
    			break;
    			clearBuffer();
    			strncpy(buf+filled,ep->d_name,strlen(ep->d_name));
    			filled+=strlen(ep->d_name);
    			buf[filled]=' ';
    			filled++;
    		}
		}
		
		if(filled<PACKETSIZE)
		{
			buf[0]='1';
			//printf("%d\n",buf[0]);
			if(sendData(new_fd,buf,filled)==-1)
			perror("send");
			waitTillInput;
			buf[numbytes]='\0';
			puts(buf);
    		clearBuffer();
		}
    	(void) closedir (dp);
  	}
  	else
    perror ("Couldn't open the directory");
	/*waitTillInput;
	buf[numbytes]='\0';
	puts(buf);
	clearBuffer();*/
  
	return;
}
void getFile(int new_fd,char *filename)
{
	char *m=GET;
	clearBuffer();
	if(sendData(new_fd,m,strlen(m))==0)
	perror("send");
	waitTillInput;
	buf[numbytes]='\0';
	puts(filename);
	if(sendData(new_fd,filename,strlen(filename))==-1)
	perror("send");
	waitTillInput;
	buf[numbytes]='\0';
	puts(buf);
	FILE *f=fopen(filename,"r");
	int filled=1;
	//if(f==NULL)
	//return;
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
			if(sendData(new_fd,buf,filled)==-1)
			perror("send");
			waitTillInput;
			buf[numbytes]='\0';
			if(strcmp(buf,MESSAGERECIEVING)!=0)
			break;
			clearBuffer();
			filled=1;
		}
	}
	if(filled<PACKETSIZE)
	{
		buf[0]='1';
		if(sendData(new_fd,buf,filled)==-1)
		perror("send");
		waitTillInput;
		buf[numbytes]='\0';
		puts(buf);
		clearBuffer();
	}
	return;
}
void putFile(int new_fd,char *filename)
{
	char *m=PUT;
	if(sendData(new_fd,m,strlen(m))==-1)
	perror("send");
	waitTillInput;
	buf[numbytes]='\0';
	puts(buf);
	if(sendData(new_fd,filename,strlen(filename))==-1)
	perror("send");
	
	FILE *f=fopen(filename,"w");
	while(1)
	{
		waitTillInput;
		if(buf[0]=='1')
		{
			fwrite(buf+1,1,numbytes-1,f);
			break;
		}
		fwrite(buf+1,1,numbytes-1,f);
		clearBuffer();
		if(sendData(new_fd,MESSAGESENDING,strlen(MESSAGESENDING))==-1)
		perror("send");
	}
	clearBuffer();
	if(sendData(new_fd,MESSAGESENDING,strlen(MESSAGESENDING))==-1)
	perror("send");
	fclose(f);
	return;
}
void runcd(char *path)
{
	//printf("%s\n",path);
	if(chdir(path)==-1)
	{
		printf("Error:Could not change to dir");
		return;
	}
	return;
}
void runlcd(int new_fd,char *path)
{
	waitTillInput;
	clearBuffer();
	if(sendData(new_fd,path,strlen(path))==-1)
	perror("send");
	waitTillInput;
	buf[numbytes]='\0';
	puts(buf);
	clearBuffer();
	
	printf("Directory of the client changed to ");
	puts(path);
	return;
}
void mget(int new_fd,char **keywords,int n)
{
	int i;
	if(sendData(new_fd,MGET,strlen(MGET))==-1)
	perror("send");
	waitTillInput;
	buf[numbytes]='\0';
	for(i=1;i<n;i++)
	{
		getFile(new_fd,keywords[i]);
	}
	if(sendData(new_fd,STOPSENDING,strlen(STOPSENDING))==-1)
	perror("send");
	return;
}
void mput(int new_fd,char **keywords,int n)
{
	int i;
	if(sendData(new_fd,MPUT,strlen(MPUT))==-1)
	perror("send");
	waitTillInput;
	buf[numbytes]='\0';
	//puts(buf);
	for(i=1;i<n;i++)
	{
		putFile(new_fd,keywords[i]);
	}
	if(sendData(new_fd,STOPSENDING,strlen(STOPSENDING))==-1)
	perror("send");
	return;
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
   				printf("%c\n",ep->d_name[i]);
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
char **getKm(int new_fd,int *n)
{
	char **k;
	int i;
	k=(char **)malloc(sizeof(char *)*100);
	for(i=0;i<100;i++)
	k[i]=(char *)malloc(sizeof(char)*PACKETSIZE);
	if(sendData(new_fd,MESSAGERECIEVING,strlen(MESSAGERECIEVING))==-1)
	perror("send");
	waitTillInput;
	buf[numbytes]='\0';
	i=1;
	(*n)=i;
	while(1)
	{
		if(strcmp(buf,STOPSENDING)==0)
		{
			break;
		}
		strcpy(k[i],buf);
		if(sendData(new_fd,MESSAGERECIEVING,strlen(MESSAGERECIEVING))==-1)
		perror("send");
		waitTillInput;
		buf[numbytes]='\0';
		i++;
	}
	(*n)=i;
	for(i=1;i<(*n);i++)
	puts(k[i]);
	return k;
}
		
void parseCommand(int new_fd,char *com)
{
	char **keywords=getKeywords(com);
	int ch=command(keywords[0]),no;
	char *m;
	switch(ch)
	{
		//***************************************************************LS*************************************************//
		case 1:
		m=LS;
		printf("Showing list of files\n");
		if(sendData(new_fd,m,strlen(m))==-1)
		perror("send");
		waitTillInput;
		buf[numbytes]='\0';
		if(strcmp(buf,MESSAGERECIEVING)==0)
		{
			ls(new_fd);
		}
		//waitTillInput;
		//clearBuffer();
		break;
		//****************************************************************LLS************************************************//
		case 2:
		m=LLS;
		printf("Client:show list of files\n");
		if(sendData(new_fd,m,strlen(m))==-1)
		perror("send");
		waitTillInput;
		clearBuffer();
		//printf("Directory of the client changed\n");
		break;
		//****************************************************************cd************************************************//
		case 3:
		m=CD;
		if(sendData(new_fd,m,strlen(m))==0)
		perror("send");
		runcd(keywords[1]);
		waitTillInput;
		clearBuffer();
		break;
		case 4:
		m=LCD;
		if(sendData(new_fd,m,strlen(m))==0)
		perror("send");
		runlcd(new_fd,keywords[1]);
		break;
		//****************************************************************get************************************************//
		case 5:
		
		getFile(new_fd,keywords[1]);
		break;
		case 6:
		puts(keywords[1]);
		if(strcmp(keywords[1],"*.*")==0)
		{
			keywords=getK(&no);
			mget(new_fd,keywords,no);
			break;
		}
		else if(keywords[1][0]=='*')
		{
			keywords=getKK(&no,keywords[1]+1);
			mget(new_fd,keywords,no);
			break;
		}
		mget(new_fd,keywords,wordCount(com));
		break;
		case 7:
		putFile(new_fd,keywords[1]);
		break;
		case 8:
		if(strcmp(keywords[1],"*.*")==0)
		{
			keywords=getKm(new_fd,&no);
			mput(new_fd,keywords,no);
			break;
		}
		mput(new_fd,keywords,wordCount(com));
		break;
		case 9:
		if(sendData(new_fd,"exit",strlen("exit"))==-1)
		perror("send");
		break;
		default:
		printf("Wrong message\n");
		/*if(sendData(new_fd,"exit",strlen("exit"))==-1)
		perror("send");*/
	}
	return;
}
		
		
