/*SR Kanna
 *HW3
 *2/13/2016
 *BENJAMIN BREWSTER
 *
 *PROGRAM DESCRIPTION: This program is a writing our own shell that mimicks the bash shell provided by inux (bourne again shell). For example, the user executes the program and the shell executes, there are three built in commands - cd (to a location or home). The other commands allow for file input or export and are handled by the path/execution. Background and foreground processes are hadled. Backgorund processes output all go to the /dev/nulll. If the background doesn't specifiy a file for input, then the file must be rediecte dfor null. 
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
#define maxarg	512		//costant for number of arguments (this was given to us  because previous students asked for this so they can have an approximate idea of how many characters)
#define maxchar	2048		//constant for the maximum number of characters (this was given to us  because previous students asked for this so they can have an approximate idea of how many characters)


//prototypes for functions
char ** readuser();						//read the input from  the user - this function reads and parses input from the user so you can tell what they have have entered and execute the command
int decision(char ** read_input,int usr_continue,int bg,int * exit2);	//this command executes the command so the user's command gets executed (the basic commands and more complex ones) Input: the array, if the user wants to continue, if its a background process
int find(char ** read_user,char * c);				//this command finds if certain characters have been entered in the user input for file iput/redirect I: the user array, character we are looking for
//int files(char ** read_input);					


//main program
int main()
{
int exit2[1] = {5};
char ** read_input1 = NULL;		//first time reading input is being set to null so there's no garbage (c will do that)
char answer[maxchar];			//array that holds the maximum number of characters that the answer can contain
int usr_continue;			//this varaible holds onto whether or not the user wants to continue (0 yes, 1 no)
int bg = -1;				//says if this a background process (-1 no, 0+ yes)
//int arg;				
usr_continue = 0;			//initialize usr_continue so it's not garabge
//	char c = '\0';
do{
	//echo to test because c doesn't have pass by reference
	//	printf("the exit2 value now is %d\n",exit2[0]);
		printf("\n:");			//prompts the user input (colon like the bash input)
	//do the read in here
		fflush(stdin);			//flush  the prompt

		read_input1 = readuser();		//catch the user's answer using variable
		bg = find(read_input1,"&");		//catch if it's a background process or not (this function returns -1 where it is in the array)
		usr_continue = decision(read_input1,usr_continue,bg,exit2);		//this function returns whether to continue and executes the command the user origionally asks for (main point of the smallsh)
}
	while(usr_continue != 1);	//1 quit smallsh, othewise keep going (this way you can keep prompting infinetely)
	free(read_input1);		//must free the user input because it was dynamically created

	return 0;

}



/*
 *
 * Notes: I learned about read_user[i]!=NULL (very cool)
 *I also thought strncmp was a deprciated version, but I hadn't realized there's a diffrent way to use it (is not used here - but for future use)
 *
 *FUNCTION: find a specific character in the array
 *INPUT: user input array, character to locate
 *OUYPUT: Where the character is indexed in the array

 */
int find(char ** read_user,char * c)
{
	int k;
	for(k =0;read_user[k]!=NULL;++k)
	{
		if(strcmp(read_user[k],c)==0)
		//if(strncmp(read_user[k],c,1)==0)
			return k;		//returns the index so you know where the character is located (for example in the < or >)
	}
	
	return -1;		//otherwise return -1 if it's not found
}





/*
 *Function: Takes user input from the stream into a buffer and returns it as an array
 *INPUT: void
 *OUTPUT: returns an array of parsed input so commands can executed
 */

//takes user input and stores
char ** readuser()
{

	//ssize_t getline(char ** lineptr,size_t *n,file *stream);			//the getline function paramaters
	
	int i = 0;
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
	free(word);		//deallocate word so there are no memory leaks
	return array;

		
//	return read_input;
}



/*FUNCTION:
 *PURPOSE: function executes the command the user wants and checks if the user wants to keep going (if the user types exit)
 *INPUT:takes an array that's been deliminated, exit value (0 or 1), whether it's a background process
 *OUTPUT: integer that returns if the user wants to continue
 */
int decision(char ** read_input,int exit_val,int bg,int * exit2)
{
	int status;

	//code provided in the slides that is the signal handling
	struct sigaction mainCatch, child;
	mainCatch.sa_handler = SIG_IGN;
	child.sa_handler = SIG_DFL;
	sigaction(SIGINT,&mainCatch,NULL);


	//just a comment - so you don't have to do anything
	if(strcmp(read_input[0],"#")==0)
		return 0;
	else if(strcmp(read_input[0],"exit")==0)
		return 1;		//exit the program - free the memory
	//if it's a cd
	else if(strcmp(read_input[0],"cd")==0)
	{
		//deals with absolute and relative paths
		//means you have only an argument to go to argument
		if(read_input[1] != NULL)
		{
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
		//exit status
		if(WIFEXITED(status))
			printf("Exit status %d\n", exit2[0]);
			//printf("Exit status: %d\n",exit_val);
		//	printf("Exit status: %d\n",WEXITSTATUS(exit2[0]));
		//prin
		else if(WIFSIGNALED(status))
			printf("Signal being stopped %d\n",WSTOPSIG(status));
		//print the terminating signal
		else if(WTERMSIG(status))
			printf("Terminating signal: %d\n",WTERMSIG(status));
		return 0;
	}
	else
	{
		//as per instructions, make sure to fork it before determining if you need to do file redirection/output
		//int exit;	//check what exit was like 
		
		//must create variables seperate than initialization because of dumb c rules
		int ok;
		pid_t processid = fork();		//fork the process
		pid_t wpid;				//catches the vlaue if process is over  without hanging or trace
		int in;		//file input
		int out;	//file output
		int bgnull;	//bg file input
		int read_file;	//variable to hold after opening file
		int exchange;	//variable to hold after dup2
		int write_file;	//variable to hold after opening file
		

		//Three options in forking (parent, child or error)

			//error in the forking returns -1, so we look for this
		if(processid < 0)
		{
			perror("Error in forking\n");
			//exit(1);		//the assignment doesn't want us to use exit
			return 1;
		}
	
		else if(processid == 0)		//child process
		{
			//printf("inside kid\n");
		
			in = find(read_input,"<");		//checks the entire command to see if file in was there - could theoretically be anywhere and in multiple places
			out = find(read_input,">");		//checks the entire commadn to see if file out was there

			//interrupt foreground processes
			if(bg == -1)
			{
				child.sa_handler = SIG_DFL;
				child.sa_flags = 0;
				sigaction(SIGINT,&child,NULL);		//signal for child (code from slides)
			}			

			if(bg >= 0)		
			{
				int i;
				read_input[bg] = NULL; //replace bg with NULL because the command can't understand & at the end
			}

			//file input/output
			if (in != -1)
			{
				//printf("file in <\n");		
			
				//open the file, if it's input make sure to only read
				read_file = open(read_input[in+1],O_RDONLY);

				//print to standard error if the file could not be opened	
				if (read_file == -1)
				{
					fprintf(stderr,"smallsh: cannot open file for input\n");
					return 1;		//return 1 so you can exit
				}

				else{
					//use dup2 to exchange stanardin with the third position (file)
					exchange =dup2(read_file,0);	//in,out, error (0,1,2)
					if(exchange == -1)
					{
						fprintf(stderr,"smallsh: cannot perform dup2\n");
						return 1;		//return 1 so you can exit	
					}
					close(read_file);		//close the file because you opened it
				}
				read_input[in] =NULL;		//set it to NULL so it can be read by the executive command
				read_input[in+1] = NULL;
			}
			

			//there's a background process, set the file to /dev/null and make sure redirection is used
			//if standard input is used, it's rewritten :)		
			if(bg >= 0 && in != -1)
					{
						//there are three arguments (in order to have file redirection), if three arguments then the third must have file input
						int len;
						if(read_input[3] ==NULL)
						{				
						//this is not tested by the p3gradingscript
						//if background process doesn't specify file, then use /dev/null 
						//printf("inside bg w/std in <\n");
						bgnull = open("/dev/null",O_RDONLY);	
						if(bgnull == -1)
						{
							fprintf(stderr,"smallsh: cannot open file for input\n");
							return 1;
						}
						//printf("i can open bg\n");
						//how to check if a file isn't specified by the user??
						exchange = dup2(bgnull,0);
						if (exchange == -1)
						{
							fprintf(stderr,"smallsh: cannot perform dup2\n");
							return 1;
						}
						close(bgnull);
					read_input[in] = NULL;			//set the file redirection to NULL so the execution can happen
					
					}
				}


			//OUTPUT REDIRECTION
			if (out != -1)
			{
				//printf("file out >\n");
				write_file = open(read_input[out+1],O_WRONLY|O_CREAT|O_TRUNC,0664);		//write, create or truncate - set umask to 0664 -
				//if you can't write to the file
				if (write_file == -1)
				{
					fprintf(stderr,"smallsh: error in writing, creating or truncating file\n");
					return 1;
					//exit(1);
				}
				else
				{
					//printf("About to dup2(), fd is: %d\n", write_file);
					exchange = dup2(write_file,1);
					if(exchange == -1)
					{
						fprintf(stderr,"smallsh: error in dup2\n");
						return 1;
						//	exit(1);
					}
					close(write_file);
					//write_file = NULL;
				}
				read_input[out] = NULL;		//set the file redirection to null so it can be used by the exeuctive command
				read_input[out+1] =NULL;
			}
				//printf("call fork for child!\n");
				//execute command - slides
				//use execvp since you have the array and the command to execute is on the first index, and the entire array is already stored)		

				if(execvp(read_input[0],read_input) == -1)
				{
					fprintf(stderr,"%s: no such file or directory ",read_input[0]);
					//fprintf(stderr,"smallsh: error in execution\n");
					fflush(stdout);
					return 1;
				}
		}
		
		else		//parent process
		{
			//print background process & return to the command prompt
			if(bg >= 0)
			{
				printf("background process id is %d\n",processid);
			}

			else	//it's not a background proeccess
			{

			
			//WIFIEXITED = returns 0 if exited normally
			//wifsignal = returns non zero if terminated by signal
			//check if you have any processes hanging (comparing to status and zero) - you need it to finish
			//keep going if process not terminated or termintaed by signal
				
			/*	while(!WIFEXITED(status) && !WIFSIGNALED(status)) 
					{
						//status report
						wpid = waitpid(processid,&status,WUNTRACED);

					}

				exit2 = WEXITSTATUS(status);
			*/

				do{
					waitpid(processid,&status,0);
				} while(!WIFEXITED(status) && !WIFSIGNALED(status));
				
					exit2[0] = WEXITSTATUS(status);	

				
			}
				//according to instructions had to display if the any process was terminated by signal
				//wifisginal - displays if exited by signal
				int val;
				if(WIFSIGNALED(status))
				{
					val = WTERMSIG(status);
					printf("terminated by signal %d\n",val);	
				}
		
		}
		
	}
		int i;
		int processid;

		//do this at the end (after exiting function of executing the process) so you can tell if the background pocess is over
		//tell if the background process is over
		processid = waitpid(-1,&status,WNOHANG);
		while(processid > 0){
			//print out the process id, if it was terminated by signal or exit status according to the isntructions
			printf("backgound process complete. ID is %d\n",processid);
			if(WIFEXITED(status))
				printf("Exit status: %d\n",WEXITSTATUS(status));
			else
				printf("Terminating signal %d\n",WTERMSIG(status));

			processid = waitpid(-1,&status,WNOHANG);		//keep checking it (can be dowhile)
		}
	//return 0;		//return 0 to keep going in the loop

	//echo to test
	//printf("exit status is at the end of foreground %d\n",exit2[0]);
				
}
