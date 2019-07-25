#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <ctype.h>
void error(const char *msg)
{
	perror(msg);
	exit(1);
}


int main(int argc, char *argv[])
{

	if(argc< 2)
	{
		fprintf(stderr, "port no not provided. Program terminated \n");
		exit(1);
	}

	int sockfd, newsockfd, portno, n;
	char buffer[255];

	struct sockaddr_in serv_addr, cli_addr;
	socklen_t clilen;

	sockfd= socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd<0)
	{
		error("Error opening socket");
	}

	bzero((char*) &serv_addr, sizeof(serv_addr));
	portno= atoi(argv[1]);

	serv_addr.sin_family= AF_INET;
	serv_addr.sin_addr.s_addr= INADDR_ANY;
	serv_addr.sin_port= htons(portno);

	if(bind(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr))<0)
		error("binding Failed.");

	listen(sockfd,5);

	clilen= sizeof(cli_addr);

	newsockfd= accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

	if(newsockfd< 0)
		error("Error on Accepted");

	int data[10];
	printf("Please input 4 bits of data\n");
	scanf("%d", &data[0]);
	scanf("%d", &data[1]);
	scanf("%d", &data[2]);
	scanf("%d", &data[4]);

	data[6]= data[4]^data[2]^data[0];
	data[5]= data[4]^data[1]^data[0];
	data[3]= data[6]^data[2]^data[1];

	for(int i=0; i<7 ; i++)
	{
		n= write(newsockfd, &data, sizeof(int));
		if(n<0)
			printf("Error while transmitting the data");

	}
	printf("the data sent is: ");
	for(int i=0; i<7; i++)
		printf("%d", data[i]);
	close(newsockfd);
	close(sockfd);
	return 0;

}
