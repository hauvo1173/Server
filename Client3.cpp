#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <ctype.h>

void error(const char *msg)
{
	perror(msg);
	exit(1);
}


int main(int argc, char *argv[])
{

	

	int sockfd,  portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	
	char buffer[256];
	if(argc < 3)
	{
		fprintf(stderr, " usage %s hostname port \n", argv[0]);
		exit(0);	
	}

	portno= atoi(argv[2]);
	sockfd= socket(AF_INET, SOCK_STREAM, 0);

	if(sockfd< 0)
		error("error opening socket");

	server = gethostbyname(argv[1]);
	if(server == NULL)
	{
		fprintf(stderr, "Error, no such host");
	}
	bzero((char*) &serv_addr, sizeof(serv_addr));
	
	serv_addr.sin_family= AF_INET;
	bcopy((char *) server->h_addr, (char*) &serv_addr.sin_addr.s_addr, server-> h_length);
	serv_addr.sin_port= htons(portno);

	if(connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))< 0)
		error("Connection Failed");
	
	int dataatrec[10];
	int test[10];
	int p, p1, p2, p4;
	printf("The Data received is:");
	for(int i=0; i<7; i++)	
	{
		n=read(sockfd, &dataatrec[i], sizeof(int));
		if(n<0)
			error("Error on receiving data");
			printf("%d", dataatrec[i]);
	}

	printf("please enter the data to be tested");
	for(int i=0; i<7; i++)
		scanf("%d", &test[i]);

	p1= test[6]^test[4]^test[2]^test[0];
	p2= test[5]^test[4]^test[1]^test[0];
	p4= test[3]^test[0]^test[2]^test[1];

	p= (4*p4) + (2*p2) + p1;

	if(p==0)
	{
		printf("no error");
	}
	else
	{
		printf("\n the error is at position %d" ,p);
		printf("the correct data is:");
		if(test[7- p]== 0)
			test[7- p]= 1;
		else
			test[7- p]=0;

		for(int i=0; i<7; i++)
		{
			printf("%d", test[i])
		}
	}


	close(sockfd);
	return 0;

}
