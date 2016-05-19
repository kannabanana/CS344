/*File Name: opt_enc_d.c
 *
 *
 *PURPOSE OF FILE: encrypts the plaintext file given by opt_enc.c. Checks if the plaintext is as big as the key. It will then output the cipher, and errorcheck that the cipher is as big as the key (which the other side does). Also it error checks if it cannot be run due to a port error.

 1. once it makes a connection with opt_enc, it forks
 2. it encrypts in the process 
 3. five possible processes can run

 *
 *
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define buffer 10000	

///error from client side
void error(const char *msg)
{
	perror(msg);
	exit(1);
}

int encrypt(char character);

//main body ---- ALL CODE TAKEN FROM CLIENT
int main(char argc,char * argv[])
{


	int status;
	char alpha[] = "abcdefjklmnopqrstuvwxyz ";
	char buffer_1[buffer];
	char plain[buffer];		//plaintext - starting point
	char key[buffer];		//key used to encrpty
	char cipher[buffer];		//final enryption

	int sockfd, newsockfd, portno;
	socklen_t clilen;
	//char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
	int n;

	int i;
	int j;
	int value = 1;
	pid_t pid;		//fork process


	//make sure there are atleast 2 arguments
	//opt_enc_d port

	if (argc < 2) {
		fprintf(stderr,"ERROR, no port provided\n");
		exit(1);
	}


	//create TPC socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
		error("ERROR opening socket");


	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);


	//bind socket to port number
	if (bind(sockfd, (struct sockaddr *) &serv_addr,
				sizeof(serv_addr)) < 0) 
		error("ERROR on binding");


	//sockets can be reused
	setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&value,sizeof(int));


	//up to five socket connections
	listen(sockfd,5);

	while(1)
	{

		///////////////////////////
		////////////////////////////////


		clilen = sizeof(cli_addr);
		newsockfd = accept(sockfd, 
				(struct sockaddr *) &cli_addr, 
				&clilen);
		if (newsockfd < 0) 
			error("ERROR on accept");


		pid = fork();		//fork off as soon as connection is made because of instructions
	
		//child
		if (pid == 0)
		{

			//reading the message
			bzero(buffer_1,buffer);
			n = read(newsockfd,buffer_1,buffer);
			if (n < 0) 
				error("ERROR reading from socket");
	


			FILE * f;
			f = fopen(buffer_1,"r");
			{
				fprintf(stderr,"unable to open plaintext file\n ");
				exit(1);
			}
			
			//store the contents of file to plaintext
			fgets(cipher,buffer,f);
			//capitalize each index of the plain array
			for(i=0;i<strlen(plain);++i)
			{
				plain[i] = toupper(plain[i]);
			}
			fclose(f);
	//add null terminator
			plain[i+1]='\0';

			//reading the key
			bzero(buffer_1,buffer);
			n = read(newsockfd,buffer_1,buffer);
			if(n<0)
			{
				error("ERROR reading from socket.\n");
			}

			f = fopen(buffer_1,"r");
			{
				fprintf(stderr,"unable to open key file\n ");
				exit(1);
			}
			
			//copy over from the file itno key array until buffer
			fgets(key,buffer,f);
			fclose(f);
		
			char cipher2[buffer];
			int temp2;
			//do the encrpytion
			for(i = 0;i< strlen(plain);++i)
			{
			
				plain[i] = encrypt(plain[i]);
				key[i] = encrypt(key[i]);
				cipher[i] = plain[i] + key[i];
				if (cipher[i] > 26)
				{
					cipher[i] = cipher[i] -27;

				}
				cipher2[i] = alpha[cipher[i]];
			}	

				cipher2[strlen(buffer_1)-1]='\0';

			//code form client.c	
			//send to server
			bzero(buffer_1,buffer);
			snprintf(buffer_1,buffer,"%s",cipher2);
		
			n = write(newsockfd,buffer_1,strlen(buffer_1));
			if (n < 0)
				error("ERROR writing to socket. \n");
		}
		//error

		else if(pid < 0)	//error
		{
			error("error in forking\n");
			exit(1);
		}


	
		else{

			close(newsockfd);
				
		}
	}	//close while loop

			close(sockfd);
			return 0; 

}


int encrypt(char character) {
  switch (character) {
    case 'A':
      return 0;
    case 'B':
      return 1;
    case 'C':
      return 2;
    case 'D':
      return 3;
    case 'E':
      return 4;
    case 'F':
      return 5;
    case 'G':
      return 6;
    case 'H':
      return 7;
    case 'I':
      return 8;
    case 'J':
      return 9;
    case 'K':
      return 10;
    case 'L':
      return 11;
    case 'M':
      return 12;
    case 'N':
      return 13;
    case 'O':
      return 14;
    case 'P':
      return 15;
    case 'Q':
      return 16;
    case 'R':
      return 17;
    case 'S':
      return 18;
    case 'T':
      return 19;
    case 'U':
      return 20;
    case 'V':
      return 21;
    case 'W':
      return 22;
    case 'X':
      return 23;
    case 'Y':
      return 24;
    case 'Z':
      return 25;
    case ' ':
      return 26;	
  }
}
