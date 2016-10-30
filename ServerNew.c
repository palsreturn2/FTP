#include "Server.h"
#include "serverFunc.c"

void sigchld_handler(int s)
{
	while(wait(NULL) > 0);
}
int main(void)
{
	int sockfd, new_fd; // listen on sock_fd, new connection on new_fd
	struct sockaddr_in my_addr;
	char buf[PACKETSIZE];
	char *sendInf;
	// my address information
	struct sockaddr_in their_addr; // connector’s address information
	int sin_size;
	struct sigaction sa;
	int yes=1;
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
	{
		perror("socket");
		exit(1);
	}
	if (setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1) 
	{
		perror("setsockopt");
		exit(1);
	}
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(MYPORT);
	my_addr.sin_addr.s_addr = INADDR_ANY;
	memset(&(my_addr.sin_zero), '\0', 8);
	if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr))== -1) 
	{
		perror("bind");
		exit(1);
	}
	if (listen(sockfd, BACKLOG) == -1) 
	{
		perror("listen");
		exit(1);
	}
	printf("Listened\n");
	sa.sa_handler = sigchld_handler; // reap all dead processes
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1) 
	{
		perror("sigaction");
		exit(1);
	}
	while(1)
	{
		sin_size = sizeof(struct sockaddr_in);
		if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr,&sin_size)) == -1) 
		{
			perror("accept");
			continue;
		}
		printf("server: got connection from %s\n",inet_ntoa(their_addr.sin_addr));
		if (fork()==0) 
		{ 
			close(sockfd);  	
			while(1)
			{
				waitTillInput;
				buf[numbytes] = '\0';
				printf("Command Received: %s\n",buf);
				parseCommand(new_fd,buf);
			}	
		}
		close(new_fd);
	}
	return 0;
}

