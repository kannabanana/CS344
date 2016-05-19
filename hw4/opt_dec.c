/*File Name: opt_enc.c
 *
 *
 *PURPOSE OF FILE:
 *Gets plaintext, key and port sends to opt_enc_d, gets ciphertext back. This file prints it to standand out. It has three possible output methods. There are various error catches such as if the ciphertext is smaller or larger than the plaintext or if the port does not exist. Furthermore this file can connect to opt_enc_d, but opt_dec_d cannot connect to this file.
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


#define buffer 10000		//buffer size constant

//error function from client
void error(const char *msg)
{
    perror(msg);
    exit(1);
}



//function to tell if the plain text and key are equal lengths
int compare(char plaintext[],char key[]);
void isbad(char file[]);



int main(int argc, char * argv[])
{
	//need at least four arguments (opt_enc, plain, key, port)
	if(argc == 4)
	{


		//create struct for server
		int sockfd;
		int portno;
		int n;
		int i;
		int buffIdx;
		int outBytes;
		int len;	//length of buffer input
		int plaincf;

		//create an instrace of a internet server
		struct sockaddr_in serv_addr;
		struct hostent * server;

		//arrays to copy over what's inside user input
		char plain[buffer];	//set to null so it's not initialized to garbage
		char key[buffer];	// initalize to NULL to it's not garbage
		char cipher[buffer];	
	
		//read and makes sure these files user entered are valid
		FILE * pf;		//plain->cipher
		FILE * kf;		//key

		//create and open connections to make sure user input is valid		
		pf = fopen(argv[1],"r");		//make sure it exists and not corrupt
		if(pf < 0)
		{
			fprintf(stderr,"unable to open plaintext file for reading\n");
			exit(1);
		}

		if((fgets(plain,buffer,pf))==NULL)	//didn't work
		{
			fprintf(stderr,"couldn't read %s\n",argv[1]);
			exit(1);
		}


		fclose(pf);	//close because we're done


		//reads from the file (fp) until the number of ints (buffer) and stores into plain		
		kf = fopen(argv[2],"r");	//opt_enc plain key port
		if(kf < 0)
		{
			fprintf(stderr,"unable to open key file for reading\n");
			exit(1);
		}
		if((fgets(key,buffer,kf))==NULL)	//didn't work
		{
			fprintf(stderr,"couldn't read %s\n",argv[2]);
			exit(1);
		}

		fclose(kf);	//close because we're done

		//check if plaintext is as long as key
		len = compare(plain,key);
		--len;		//decrement buffer so you can add null terminator

		key[len] = '\0';		//set null termination
		plain[len] = '\0';
		isbad(key);
		isbad(plain);


		//////////////////////////////////////code from client.c 
		//to create port and establish a TCP connection with server
		portno = atoi(argv[3]);
		sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if (sockfd < 0) 
			error("ERROR opening socket");
	
		server = gethostbyname("localhost");
		if (server == NULL) {
			fprintf(stderr,"ERROR, no such host\n");
			exit(0);
		}

		bzero((char *) &serv_addr, sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
		bcopy((char *)server->h_addr, 
				(char *)&serv_addr.sin_addr.s_addr,
				server->h_length);
		serv_addr.sin_port = htons(portno);
		if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
			error("ERROR connecting");
		
		////////////////////////////// code from client.c

		n = write(sockfd,plain,strlen(plain));
		if (n < 0) 
			error("ERROR writing to socket");
		n = read(sockfd,&plaincf,sizeof(plaincf));		//modify it so that you get the size of the buffer
		if (n < 0) 
			error("ERROR reading from socket");


			
		n = write(sockfd,key,strlen(key));
		if (n < 0) 
			error("ERROR writing to socket");
		n = read(sockfd,cipher,buffer-1);		//modify it so that you get the size of the buffer
		if (n < 0) 
			error("ERROR reading from socket");

		//printf the cipher to stdout like instructions ask
		fprintf(stdout,"%s\n",cipher);
		close(sockfd);
		return 0;

	}
	else
	{
		fprintf(stderr,"Not enough arguments\n");
	}
	return 0;

}



//checks if function has correct characters (a-z and space atleast)
//INPUT: character array
//OUTPUT: nothing
void isbad(char file[])
{
	int i;
	int len;
	len = strlen(file);		//assign to a value
	for(i =0;i<len;++i)
	{
		if(file[i] != ' ' && !isalpha(file[i]))	//if they're not a-z and || some mistake probably occrued
		{
			fprintf(stderr,"file contains bad characters\n");
			exit(1);		//kick them out
		}
	}


}



//function to compare the two and exit if they are not the same length
//INPUT two character arrays to copy over
//output integer which indicates length
int compare(char plaintext[], char key[])
{
	int textlen;		//initalize because c can't do it on the same line
	int keylen;		
	textlen = strlen(plaintext);	//get length
	keylen = strlen(key);
	if(keylen < textlen) {		//check if key is less than input, if it is kick them out because an error has occured (they should be the same if the cipher text worked properly)
		fprintf(stderr,"The length of the key was less than the text\n"); 
		exit(1);
	}
	return keylen;	//otherwise return the length because you're good to keep going
}
