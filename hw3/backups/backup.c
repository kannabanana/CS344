/*SR Kanna
 *HW3
 *2/13/2016
 *BENJAMIN BREWSTER
 *
 *PROGRAM DESCRIPTION:
 *
 *
 */


/*
 *NOTES: Exit not working
 *
 * When program exits smallsh, it keeps re-entering program
 */

#include <sys/stat.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>


//CONSTANTS
#define maxarg	512
#define maxchar	2048


//prototypes
char ** readuser();		//read the input from  the user
int decision(char ** read_input,int usr_continue,int bg);
int find(char ** read_user,char * c);
int files(char ** read_input);



int main()
{
char ** read_input1 = NULL;		//first time reading input
char answer[maxchar];		//array 
int usr_continue;
int bg;
int arg;
usr_continue = 0;
//	char c = '\0';
do{
	printf("\n:");			//print the colon because that is the  prompt
	//do the read in here

	fflush(stdin);			//flush  the prompt
	read_input1 = readuser();		//catch the user's answer
		bg = find(read_input1,"&");
		//printf("bg is %d\n",bg);
		//printf("read_input[0] is %s\n",read_input1[0]);
		//printf("read_input[1] is %s\n",read_input1[1]);	
		usr_continue = decision(read_input1,usr_continue,bg);
}
	while(usr_continue != 1);
	free(read_input1);		//must free the user input because it was dynamically created

	return 0;

}



//find a specific character in the array
int find(char ** read_user,char * c)
{
	int i;
	for(i =0;read_user[i]!=NULL;++i)
	{
		if(strncmp(read_user[i],c,1)==0)
			return i;		//returns the index 
	}
	
	return -1;
}




/*
 *Function: Takes user input from the stream into a buffer and returns it as an array
 *INPUT: void
 *OUTPUT: returns an array
 */

//takes user input and stores
char ** readuser()
{
	int i = 0;
	//ssize_t getline(char ** lineptr,size_t *n,file *stream);			//the getline function paramaters
	char * read_input = NULL;
	ssize_t size = 0;

	//get_line takes the file_i/o and stuffs it into a buffer and adds a new line
	//pass by reference because it's being passed back
	getline(&read_input,&size,stdin);
//	printf("%s\n",read_input);	//is being read into buffer

	
	
	char ** array = malloc(sizeof(char*)*512);	//array that takes the because the command can only have 512 arguments


	char * word = NULL;		//catches the strtok
	word = strtok(read_input,"\n");	//check if there's nothing in the buffer
	if(word == NULL)
	{
		array[0] = NULL;
		return array;				
	}


	word = strtok(read_input," "); //loop through and 
	while (word != NULL)
	{
		array[i] = word;	//copy over what the segmented fragment is to the array index
		++i;
		word = strtok(NULL," ");	//update the strtoko
	}
	array[i] = NULL;	//set the NULL terminator
	free(word);		//deallocate word
	return array;

		
//	return read_input;
}




/*FUNCTION:
 *
 *INPUT:takes an array that's been deliminated
 *OUTPUT: void
 */
int decision(char ** read_input,int exit_val,int bg)
{
	//just a comment - so you don't have to do anything
	if(strcmp(read_input[0],"#")==0)
		return 0;
	else if(read_input[0] == NULL)
		return 0;
	//if it's an exit command
	else if(strcmp(read_input[0],"exit")==0)
		return 1;		//exit the program - free the memory
	//if it's a cd
	else if(strcmp(read_input[0],"cd")==0)
	{
		//means you have only an argument to go to argument
		if(read_input[1] != NULL)
		{
			//CALL BASH SCRIPT TO CHANGE DIRECTORIES
			//
			//absolute or relative path?!
			printf("you have more than one argument!\n");
			//change the directory to what the user wants to change it to
			char * new_dir = read_input[1];
			//printf("the new directory name is %s\n",new_dir);
			chdir(new_dir);
						
		
			return 0;
		}

		//go to home directory
		else
		{
			//stack overflow recommended I find the getenv variable rather just hahrdcoding the home directory because then I'd always have my home directory
			//in you have the getenv variable, put into the chandir so it can change the directory
			char * home = getenv("HOME");
			chdir(home);
				
			//find out the home directory
			//change to that directory
			return 0;
		}
		
	}
	//check if there's one argument or none
	//if there's one - switch, none then go directly to home
	else if(strcmp(read_input[0],"status")==0)
	{
		//DO I NEED A SIGNAL INTEGER?
		printf("the exit value %d\n",exit_val);
		return 0;

	}
	else
	{
			//as per instructions, make sure to fork it before determining if you need to do file redirection/output
		int status;	//status to determine if we're still in the process
		int exit;	//check what exit was like 
		int ok;
		pid_t processid = fork();		//fork the process
		pid_t wpid;				//catches the vlaue if process is over  without hanging or trace
		
			//error in the forking
		if(processid < 0)
		{
			perror("Error in forking\n");
			//exit(1);		//the assignment doesn't want us to use exit
			return 1;
		}
	
		else if(processid == 0)		//child process
		{
			if(bg >= 0)	//background process
			{
				printf("the background process is %d\n",processid);
		
			}
			ok = files(read_input);	
			
			if (ok == 1)
				return 1;
			else if(bg >= 0)
			{
				int in;
				int bgnull;
				in = find(read_input,"<");
				//check if no files were specified by the command
				
				//there was a file input redirection
				if (in != -1)
				{
					bgnull = open("/dev/null",O_RDONLY);	
					//how to check if a file isn't specified by the user??
					dup2(bgnull,0);
				}
			}
			else
			{
				//printf("call fork for child!\n");
				//execute command - slides
				//use execvp since you have the array and the command to execute is on the first index, and the entire array is already stored)
				if(execvp(read_input[0],read_input) == -1)
				{
					fprintf(stderr,"error in executing command");
					return 1;
				}
				return 0;
			}		
			//do something background process
			//call function to handle execution of command

		}
		
		else		//parent process
		{
			printf("\nparent process\n");

			//if it's a background process - repromt
			if(bg >= 0)
			{

				//WIFEXITED returns true if exited, otherwise false (so the process hasn't exited)
				//WIFSIGNAL returns true if hasn't been terminated by signal (so either it's still in the process or it's been terminated by signa)
				while(!WIFEXITED(status) && !WIFSIGNALED(status)) 
				{
					//check to see if the process id has completed, otherwise return with zero 
					wpid = waitpid(processid,&status,WNOHANG |WUNTRACED);		
				}
			}
			else	//it's not a background proeccess
			{
				while(!WIFEXITED(status) && !WIFSIGNALED(status)) 
					{
						//status report
						wpid = waitpid(processid,&status,WUNTRACED);

					}

				exit = WEXITSTATUS(status);

				//once it's been terminated -
				//print background process, process id and exit value
				
			}
		}
				if(bg == 1)
				{
					printf("Background Process: Process Id is %d, Exit status is %d\n",processid,exit);
				}
			
		/*		//if contrl c 
				if(WIFSIGNALED(status)
				{
					WTERMSIG(status)
					
				}*/
			return 0;	
			
	}
}
//}

/*
 *FUNCTION: Files -
 *
 *
 *
 */
int files(char ** read_input)
{
int in;		//file input
int out;	//file output
int read_file;
int exchange;
int write_file;
in = find(read_input,"<");		//checks the entire command to see if file in was there
out = find(read_input,">");		//checks the entire commadn to see if file out was there

		
	if (in != -1)
	{
		printf("file in <\n");		
		
			//if background process doesn't specify file, then use /dev/null 	

		read_file = open(read_input[in+1],O_RDONLY);		
		
			//print to standard error if the file could not be opened	
			if (read_file == -1)
			{
				fprintf(stderr,"Could not open the file\n");
				//exit(1);	//exit with error status
				return 1;
			}
	
			else{
				//use dup2 to exchange stanardin with the third position (file)
				exchange =dup2(read_file,0);	//in,out, error (0,1,2)
				if(exchange == -1)
				{
					fprintf(stderr,"error in file\n");
					//exit(1);
					return 1;			
				}
			close(read_file);
			}
		return 0;

	}
				//check if the file is writable - if it's not, set the file to zero and return
	else if (out != -1)
	{
		printf("file out >\n");
		write_file = open(read_input[out+1],O_WRONLY|O_CREAT|O_TRUNC,0664);		//write, create or truncate - set umask to 0664 -
		//if you can't write to the file
		if (write_file == -1)
		{
			fprintf(stderr,"Error writing to file");
			return 1;
			//exit(1);
		}
		else
		{
			exchange= dup2(out,1);
			if(exchange == -1)
			{
				fprintf(stderr,"error writing to file");
				return 1;
			//	exit(1);
			}
		}
	
	}
		return 0;
}
