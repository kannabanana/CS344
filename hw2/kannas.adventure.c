/*NAME: SR KANNA
 *DATE: 1/31/2016
 *PROGRAM #: 2
 *BENJAMIN BREWSTER
 *CLASS: OPERATING SYSTEMS (CS344)
 *
 *
 *PROGRAM DESCRIPTION: 
	THIS PROGRAM IS A SPINOFF OF THE OLD ADVENTURE GAMES. WE WILL RANDOMLY GENERATE 7 ROOM NAMES FROM A SELECTION OF 10. THEY WILL HAVE A MINIMUM OF THREE TO SIX CONNECTIONS. THE CONNECTIONS WILL BE LOADED INTO AN ARRAY OF STRUCTS (THE DATA STRUCTURE FOR THIS PROGRAM). IF A CONNECTS TO B, THEN B MUST CONNECT TO A, BUT A CANNOT CONNECT TO A. THE START, MID AND END ROOMS ARE RANDOMLY ASSIGNED. A DIRECOTRY IS GENERATED AND FILES ARE CREATED FROM THE ARRAY OF STRUCTS. AS PER THE INSTRUCTIONS OF INSTRUCTOR, THE FILE WILL BE USED TO READ BACK INTO A NEW ARRAY OF STRUCTS. TO PLAY THE GAME, THE USER WILL ENTER THE ROOM THEY WISH TO GO TO. IF THEY END UP IN THE END ROOM, THEY WIN THE GAME. THE GAME WILL KEEP TRACK OF THE ROOMS THEY WENT TO AND THE NUMBER OF STEPS IT TOOK THEM TO REACH THE END ROOM. THE USER HAS 25 TRIES TO GET TO THE END ROOM. IF THEY DO NOT, THE GAME WILL SAY THEY FAIL. 
 *
 *
 * C Tips: There are no functions in structs and very bizzare pass by copy/reference- be careful (what's the point of C?)
 * 
 * Notes: This program was to be submitted in one file which is why there is not multiple .h's
 */




#include <pwd.h>
#include <assert.h>
#include <stdbool.h>
#include <errno.h>	
#include <unistd.h>		//getpid
#include <sys/types.h>		//pid_t
#include <stdio.h>		//fgets, fopen, fclose, fseek
#include <stdlib.h>		//random function
#include <sys/stat.h>		//stats
#include <string.h>
#include <time.h>
//#include <fcnt1.h>



//CONSTANTS
#define MAX_CON	6			//maximum number of connections
#define MIN_CON	3			//minimum number of connections
#define MAX_ROOM 10		//maximum number of rooms
#define ROOM_TOTAL 7		//total number of rooms that are available in the game


struct room{
	char room_name[12];			//room  name character array
	int connection;				//max number of connection
	int random_connection;			//random number assigned to room
	char connections[MAX_CON][12];		//there can be only 6 connections
	char type[30];				//says what kind of room (start, mid, end)
	int count;				//keep track of where you are in the connection array
};

void mkdirectory();
void introduction();
void initialize(struct room a_room[7]);
void populateconnections(struct room a_room[7], char place_holder[7][10],int i,int random);
void print(struct room a_room[7]);
void createfile(struct room a_room[7]);
void gameload(struct room g_room[7],struct room a_room[7]);
void play_game(struct room a_room[7]);


//main block	
int main()
{
	srand(time(NULL));		//seed random generator

//struct that represents each room

	struct room a_room[7];		//array of struct rooms of 7
	struct room g_room[7];	
	
	char *game_room[] = {"kitchen","bathroom","garage","living room","gym","hall","garden","wine cellar","studio","pantry"};
	char *type[] = {"start_room","mid_room","end_room"};		//room types that  will be randomly assigned
	char *file_name[] = {"file1.txt","file2.txt","file3.txt","file3.txt","file4.txt","file5.txt","file6.txt","file7.txt"};		//array of file names


	introduction();	//call introduction function
	mkdirectory();			//make a directory with name, room and PI #
	initialize(a_room);		//initialize struct
//	print(a_room);			//test function to print into file
	createfile(a_room);	//files are created and put into directory


	gameload(g_room,a_room);
	play_game(a_room);	

	//play game

	return 0;
}



/*
 *FUNCTION: introduction
 *PURPOSE: Introduce the program to the player
 *INPUT: none
 *OUTPUT: none
 *
 */

void introduction()
{
	printf("\n");
	printf("Welcome to the adventure game remake!\n");
	printf("\n");	
	printf("There are seven rooms; three of which have been randomly assigned to be the start, middle and end room. If you make it to the end room, you win the game! We'll keep track of the number of steps and the route you take. Good luck! \n\n");

}


/*
 *FUNCTION: mkdirectory
 *Purpose: this function creates an empty directory to store the files 
 *INPUT:; None
 *OUTPUT: None

Outline:
//gets the processid from the id
//if the directory doesn't exist (stats checks)
//create it
//if it does say error and exit program
 */

void mkdirectory()
{

	int processid = getpid();		//how to get the process ID
	char filename[50];			//create filename
	sprintf(filename,"./kannas.rooms.%d",processid);	//sprintf takes the formated

	struct stat st = {0};			//stack overflow
	
	if (stat(filename,&st)== -1){
		int result=mkdir(filename,0700);		//create direcotry with executable functions
	}
	else 
	{
		printf("error");		//don't clean up directories
		exit(1);			//exit from the 
	}
}


/*
 * function: initialize
 *PURPOSE: initialize the array of structs and sets the name array for the struct,  calls the populate structure function to populate each index in the structure (passes it a  shallow copy)
 *INPUT: ARRAY OF STRUCTS
 *OUTPUT:NONE
 */

void initialize(struct room a_room[7])
{
srand(1);
int total;
int i;
int j;
int b;
int z;
int x;
int random;
int random2;
char place_holder[7][12];
char *game_room[] = {"kitchen","bathroom","garage","living room","gym","hall","garden","wine cellar","studio","pantry"};
char *sme[] = {"start","mid","mid","mid","mid","mid","end"};					//array that is looped through randomly to determine which room is start or ending point
char name[12];
char thisname[12];
char temp_name[12];

	for(i =0;i<7;++i)
	{
		a_room[i].count = 0;		//count variable initialized to zero - keeps track of where you are in the array index of connections
		//initializing the variables
		for(j=0;j<6;++j)
		{
			strcpy(a_room[i].connections[j],"none");		//each connection array under the array of structures contains the variable none		

		}


		//initializing room name
		do{
			random=(rand()%10);	//finds a random number between 0 and 9
		}
			
		while(strcmp(game_room[random],"apple")==0);		
			strcpy (a_room[i].room_name, game_room[random]);		//set the room value to what the game_random value was
			strcpy (place_holder[i],game_room[random]); 		
	//		printf("the name of the place holder is %s\n",place_holder[i]);
			game_room[random] = "apple";

	}
						

	//reset for loop because you need the names all  to be set before you can access the entire array of stucts
	for(i=0;i<7;++i)
	{		
		
			do{
				random=(rand()%7);	//finds a random number between 0 and 9
				total=random+a_room[i].count;
			
			}	
				while(random>3 && total>=7);		//search again if it's not greater  than 3
		
			//	printf("the count in the beginning of the index is %d\n",a_room[i].count);
			//	printf("The random count for the index is %d\n\n",random);
			//	total=random+a_room[i].count;		
			//	printf("the total is count+random is %d\n",total);
		

			if(a_room[i].count < 3)		//only call the function if count function is greater than - this way you don't need to run the function unnecessarily and double check
				{
			
					for(b=0;b<random;++b)		//loop through 3-6 times
					{
			
			//	do{
						do{
							random2=(rand()%7);	//finds a random index between 0 and 9
						}

						while(strcmp(place_holder[random2],"apple")==0 || strcmp(place_holder[i], place_holder[random2])==0); //|| strcmp(place_holder[i],place_holder[random2]==0));		
				
						strcpy(name, place_holder[random2]);		//get a temporary variable to hold the place of 
						//printf("the name chosen is%s\n",name);
						strcpy(place_holder[random2],"apple");
					//	printf("The name of the old index is now %s\n",place_holder[random2]);


				//		printf("\n");

						//go to the connection array and double check that the array doesn't  already exist
						//if it does (do while) - this double checks the array -
						//		tell=0;
						//		for(k=0;i<7;++k)
						//		{
						//			printf("the name  of random connection list is %s\n",a_room[random].connection);		//pick a  random  index in the array, anc display 
						//			if (strcmp(a_room[random].connections[k],name)==0)
						//				printf("The name already exists as a connection! Redo! \n");
						//				++tell;
						//		}

						//		while(tell != 0);	//the name already exists as a connection


						//check the count for array connection current i and insert the name inside that index

						strcpy(temp_name,a_room[i].room_name);	//temporary to hold the place of the origional name
					//	printf("The name of temp is %s\n",temp_name);
						strcpy(a_room[i].connections[a_room[i].count],name);		//insert the random index inside the connection array
					//	printf("The name  of the connection in the origional index is %s\n",a_room[i].connections[a_room[i].count]);
						strcpy(a_room[random2].connections[a_room[random2].count],temp_name);	//get that room name inside the connections
					//	printf("The name  of the connection in the random index is %s\n",a_room[random2].connections[a_room[random2].count]);


						++(a_room[i].count);		//incrememnt count of the index i
										//	printf("The count for  the first index is now %d\n",a_room[i].count);
						++(a_room[random2].count);	//increment the random  index's count
					//	printf("The count for  the random index is now %d\n",a_room[random2].count);

					}
					
				}

			//	printf("the count at the end for the given index is %d\n",a_room[i].count);
			//	printf("\n\n");
				//for loop and strcpy the place_holder array
				for(x=0;x<7;++x)
				{
					strcpy(place_holder[x],a_room[x].room_name);
				}

	}		



	//loop through array and set the type of the room to a random type
	for(z=0;z<7;++z)
	{	
		srand(time(NULL));
		do{
			random=(rand()%7);	//finds a random number between 0 and 9
		}
			
		while(strcmp(sme[random],"apple")==0);		
			strcpy (a_room[z].type, sme[random]);		//set the room value to what the game_random value was
			sme[random] = "apple";
	}

}



/*
 *DEBUGGING FUNCTION - print things out to make sure everything works inside array of structs
 *
 *INPUT: array of structs
 *OUTPUT: prints array of structs and each of the data variables (name, type and connections)
 */
void print(struct room a_room[7])
{
int i;
int j;

printf("\n\n\n");
	for(i=0;i<7;++i)
	{
		printf("name: %s\n",a_room[i].room_name);
//		printf("connection number is %d\n",a_room[i].count);
		for(j=0;j<a_room[i].count;++j)
		{
			printf("CONNECTION: %s\n",a_room[i].connections[j]);
		}
		printf("type: %s\n\n",a_room[i].type);
	}

}


/*
 *
 *FUNCTION: create seven files from the array of structs into the kannas.rooms.PI directory
 *INPUT: array of structs
 *OUTPUT: 7 Files that have the connections to other rooms
 *
 */
void createfile(struct room a_room[7])
{

int process_id = getpid();		//the process id
char filename[256];
FILE *fp;		//filename
int i;
int j;

for(i=0;i<7;++i)		//loop through each index and create a file for each index
{	
		
//-- creates room#i under /kannas.rooms.processid direcotry and stores into filename char
	sprintf(filename,"./kannas.rooms.%d/room#%d",process_id,i);

	fp = fopen(filename,"w");		//open with writeproperties
	
	if(fp != NULL) 			//check if the file is empty
	{
		fprintf(fp,"ROOM NAME: %s\n",a_room[i].room_name);
			for(j=0;j<a_room[i].count;++j)				//loop through until the maximum number of connections are played
			{
				fprintf(fp,"CONNECTION %d: %s\n",j+1, a_room[i].connections[j]);
			}
			fprintf(fp, "ROOM TYPE: %s\n\n",a_room[i].type);
			fclose(fp);
	}
	else		//if it's not empty then exit
		exit(0);
}
}


/*
 *FUNCTION: load the array of structs used to be in the game
 *INPUT: empty array of structs
 *OUTPUT: full array of structs
Function very similar to outputing/creating files
 */

void gameload(struct room g_room[7],struct room a_room[7])
{

int process_id = getpid();		//the process id
char filename[256];
FILE *fp;
int i,j,z;
char * name;
char * connection;
char * type;

for(z=0;z<7;++z)
{
	g_room[z].count =0;		//set the count to zero	
}


for(i=0;i<7;++i)		//loop through each index and create a file for each index
{	
		
//-- creates room#i under /kannas.rooms.processid direcotry and stores into filename char
	sprintf(filename,"./kannas.rooms.%d/room#%d",process_id,i);

	fp = fopen(filename,"r");		//open with writeproperties
	fseek(fp,SEEK_SET,0);			//set the file pointer back to the top
	
	if(fp != NULL) 			//check if the file is empty
	{
		name = strtok(a_room[i].room_name,":");	
		while (name != NULL)
		{
			name = strtok (NULL,":");
		}
		strcpy(g_room[i].room_name,a_room[i].room_name);
			

		for(j=0;j<a_room[i].count;++j)				//loop through until the maximum number of connections are played
		{

			connection = strtok(a_room[i].connections[j],":");
			while (connection != NULL)
			{
		//		printf ("Connection for %i is %s\n",i, connection);
				connection = strtok (NULL,":");
			}
	
			strcpy(g_room[i].connections[j],a_room[i].connections[j]);
		}
							

	
		type = strtok(a_room[i].type,":");
		while(name != NULL);
		{
			type = strtok (NULL,":");
			
		}
		strcpy(g_room[i].type,a_room[i].type);

		fclose(fp);
	}
	else		//if it's not empty then exit
		exit(0);
}

}


/*FUNCTION TO PLAY THE GAME
 *
 *INPUT: array of structs
 *OUTPUT: array of structs
 *
 */
void play_game(struct room g_room[7])
{
char userguess[15];
int random;
int i;
int j;
int z;
int b;
int f;
char path[26][15];		//array of 25 that holds the path taken - 
int count;			//count holds the 
int invalid;

		//choose  starting point

		do{
			random=(rand()%7);	//find a random number between 0-6
		}	
		while(strcmp(g_room[random].type,"end")==0 || strcmp(g_room[random].type,"mid")==0);		//keep finding a new one if the type is end or mid
	
	i=0;
	while(i<25)
	{
		//check  if the random number is at the end - if  so,  break and display the path choosen by the user
		
		if(strcmp(g_room[random].type,"end")==0)
		{
			printf("YOU FOUND THE END ROOM. CONGRAGULATIONS!\n");
			printf("YOU TOOK %d STEPS TO VICTORY. YOUR PATH WAS: \n",i+1);
			strcpy(path[i],g_room[random].room_name);		//insert the name of the current room into the array								
			for(b=0;b<i+1;++b)
			{
				printf("%s\n",path[b]);
			}
			
				printf("\n");
			break;
		}	
		
		do{
			invalid=0;		//guess gets set if they answer invalid input
	

			
			printf("CURRENT LOCATION: %s\n",g_room[random].room_name);

	//loop through and get a connection through the connections
			printf("POSSIBLE CONNECTIONS: ");
			for(j=0;j<g_room[random].count;++j)
			{
				if(j != (g_room[random].count -1))
				{
					printf("%s, ",g_room[random].connections[j]);
				}
				else
					printf("%s.",g_room[random].connections[j]);
			}

			printf("\n");
			printf("WHERE TO?>");
			fgets(userguess,15,stdin);		//get user  input
			userguess[strcspn(userguess,"\n")]='\0';	//add null terminator to user input
		//check if it's valid input by looping through the connection array
		//keep a flag if it's not 
		//do while 
			printf("\n");
			for(z=0;z<g_room[random].count;++z)
			{
				if(strcmp(g_room[random].connections[z],userguess)!=0)		//if it doesn't match any of the connections
				{
					++invalid;	
				}
		//		printf("the invalid number is %d\n",invalid);
		//		printf("the count is %d\n", a_room[random].count);
			}
			if(invalid == g_room[random].count)		//it matches none of the connections
			{			
				printf("HUH? I DON'T UNDERSTAND THAT ROOM.  TRY AGAIN. \n\n");		
				//keep random the same
			}

			else	
			{
				strcpy(path[i],g_room[random].room_name);		//insert the name of the current room into the array				
			//	printf("the name chosen is %s\n",path[i]);
				//figure out which index the user choose for the next one
				for(f=0;f<7;++f)
				{
					if(strcmp(userguess,g_room[f].room_name)==0)
						random=f;
				}
				++i;	//increment the number of tries
			}
		}
		while(invalid == g_room[random].count);		//keep displaying the origional data and prompt the user

		if(i >= 25)
			printf("You exceeded greater than 25 times..you're very bad at this game!\n");

	}	
}
