/*SR Kanna
 *CS344 - OPERATING SYSTEMS I
 *HW4
 *
 *FILE TYPE: .C
 *FILE PURPOSE: The purpose of the purpose is to generate a random key. The key can be any random number length (the user's chocie). It will be generated using a random number generator and loop through a an array. This simply generates a random key in the cipher/decipher process.
 *
 *
 */



#include <stdio.h>		//file input
#include <string.h>		//for string class
#include <stdlib.h>		//exit, malloc, ssize etc		
#include <time.h>		//random number generator
#include <unistd.h>		//forking


//PROTOTYPES
char * keygen(char * randomkey);		//input:


//argc = number of inputs
//argv = input parameters given by user
//input is stored in argv
int main(int argc, char *argv[])
{

//make sure there are two arguments
//create dynamic array
//loop through array of possible letters
//assign each array based on that
//print the array
//free memory

srand(time(NULL));	//seed random number genator
//char store[29] = {"a","b","c","d","e","f","g","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z"," "};
//can use that version if i did single quotes
static const char store[] ="abcdefghijklmnopqrstuvwxyz ";
	if(argc == 2)	//must be two arguments (./keygen & length of key generated)
	{	//creates a dynamic array the size of the user input plus the null terminator
		char * random_key = NULL;	//set to null so it's not garbage
		random_key = (char*)malloc(sizeof(char)*(atoi(argv[1])+1));
		int i;				//create and initialize variables seperately because c is not efficient
		int random;
		for(i =0;i< atoi(argv[1]);++i)
		{
			random = (rand()%27);	//find a random number between 0 and 26				
			random_key[i] = store[random];
		//	strcpy(random_key[i],store[random]);			(i don't get c (no deep copies aaaah))
		}
		printf("%s\n",random_key);			//print to display
		free(random_key);			//deallocate :D
	}
	else
	{
		fprintf(stderr,"Entered too few/many arguments. Please enter the length of key you would like\n");		//if you odn't have the corrent number of arguments
		exit(1);	//exit the program
	}
	return 0;
}

