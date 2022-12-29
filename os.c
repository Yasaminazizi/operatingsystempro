// C Program to design a shell in Linux
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<readline/readline.h>
#include<readline/history.h>

#define MAXCOM 1000 // max number of letters to be supported
#define MAXLIST 100 // max number of commands to be supported

// Clearing the shell using escape sequences
#define clear() printf("\033[H\033[J")

// Greeting shell during startup
void init_shell()
{
	clear();
	printf("\n\n\n\n");
	printf("\n\n\n\t****Linux Shell****");
	printf("\n\n\n\n");
	char* username = getenv("USER");
	printf("\n\n\nWelcome @%s", username);
	printf("\n");
	sleep(1);
	clear();
}

// Function to take input
int takeInput(char* str)
{
	char* buf;

	buf = readline("\n>>> ");
	if (strlen(buf) != 0) {
		add_history(buf);
		strcpy(str, buf);
		return 0;
	} else {
		return 1;
	}
}

// Function to print Current Directory.
void printDir()
{
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));
	printf("\nDir: %s", cwd);
}

// Function where the system command is executed
void execArgs(char** parsed)
{
	// Forking a child
	pid_t pid = fork();

	if (pid == -1) {
		printf("\nFailed forking child..");
		return;
	} else if (pid == 0) {
		if (execvp(parsed[0], parsed) < 0) {
			printf("\nCould not execute command.. avali");
		}
		exit(0);
	} else {
		// waiting for child to terminate
		wait(NULL);
		return;
	}
}

// Function where the piped system commands is executed
void execArgsPiped(char** parsed, char** parsedpipe)
{
	// 0 is read end, 1 is write end
	int pipefd[2];
	pid_t p1, p2;

	if (pipe(pipefd) < 0) {
		printf("\nPipe could not be initialized");
		return;
	}
	p1 = fork();
	if (p1 < 0) {
		printf("\nCould not fork");
		return;
	}

	if (p1 == 0) {
		// Child 1 executing..
		// It only needs to write at the write end
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);

		if (execvp(parsed[0], parsed) < 0) {
			printf("\nCould not execute command 1.. dovomi");
			exit(0);
		}
	} else {
		// Parent executing
		p2 = fork();

		if (p2 < 0) {
			printf("\nCould not fork");
			return;
		}

		// Child 2 executing..
		// It only needs to read at the read end
		if (p2 == 0) {
			close(pipefd[1]);
			dup2(pipefd[0], STDIN_FILENO);
			close(pipefd[0]);
			if (execvp(parsedpipe[0], parsedpipe) < 0) {
				printf("\nCould not execute command 2..");
				exit(0);
			}
		} else {
			// parent executing, waiting for two children
			wait(NULL);
			wait(NULL);
		}
	}
}

// Help command builtin
void openHelp()
{
	puts("\n***WELCOME TO MY SHELL HELP***"
		"\nCopyright @ Suprotik Dey"
		"\n-Use the shell at your own risk..."
		"\nList of Commands supported:"
		"\n>cd"
		"\n>ls"
		"\n>pwd"
		"\n>exit"
		"\n>Display the number of file lines"
		"\n>Display the first ten of the file"
		"\n>all other general commands available in UNIX shell"
		"\n>pipe handling"
		"\n>improper space handling");

	return;
}


void line(char address[])
{
	pid_t pid = fork();

	if (pid == -1) {
		printf("\nFailed forking child..");
		return;
	} else if (pid == 0) {
		FILE *fp;
		char c;
		int count = 0;
		fp = fopen(address, "r");
		if (fp==NULL)
		{
			return;
		}
		for(c = getc(fp); c != EOF; c = getc(fp))
			if(c == '\n')
				count = count + 1;
		fclose(fp);
		printf("%d\n",count);
		return;
			exit(0);
	} else {
		// waiting for child to terminate
		wait(NULL);
		return;
	}
	
}
void sigintHandler(int sig_num)
{
	
    /* Reset handler to catch SIGINT next time.*/
	
    signal(SIGINT, sigintHandler);
    printf("\n Cannot be terminated using Ctrl+C \n");
    fflush(stdout);
	printf(">>> ");
	
}

void tenLine(char address[])
{
	pid_t pid = fork();

	if (pid == -1) {
		printf("\nFailed forking child..");
		return;
	} else if (pid == 0) {
		FILE *fp;
		char c;
		char s[1000] = {'\0'};
		int count = 0;
		int n = 0;
		fp = fopen(address, "r");
		if (fp==NULL)
		{
			return;
		}
		for(c = getc(fp); c != EOF && count < 10; c = getc(fp))
		{
			s[n] = c;
			n = n + 1;
			if(c == '\n')
				count = count + 1;
		}
		fclose(fp);
		puts(s);
		return;
			exit(0);
	} else {
		// waiting for child to terminate
		wait(NULL);
		return;
	}
	
}
//repeated word
void repeated(char address[]){
pid_t pid = fork();

	if (pid == -1) {
		printf("\nFailed forking child..");
		return;
	} else if (pid == 0) {
	 FILE *fp;
	 char ch, *line;  
     size_t len = 0, read;  
     char words[1000][1000], word[20];  
     int i = 0, j, k, maxCount = 0, count;  
	 fp = fopen(address,"r"); 
	  if (fp== NULL){  
        printf("File not found");  
        exit(EXIT_FAILURE);  
    }  
      
    //Since, C doesn't provide in-built function,   
    //following code will split content of file into words  
    while ((read = getline(&line, &len, fp)) != -1) {  
          
        for(k=0; line[k]!='\0'; k++){  
            //Here, i represents row and j represents column of two-dimensional array words   
            if(line[k] != ' ' && line[k] != '\n' && line[k] != ',' && line[k] != '.' ){  
                words[i][j++] = tolower(line[k]);  
            }  
            else{  
                words[i][j] = '\0';  
                //Increment row count to store new word  
                i++;  
                //Set column count to 0  
                j = 0;  
            }  
        }  
    }  
      
    int length = i;  
      
    //Determine the most repeated word in a file  
    for(i = 0; i < length; i++){  
        count = 1;  
        //Count each word in the file and store it in variable count  
        for(j = i+1; j < length; j++){  
            if(strcmp(words[i], words[j]) == 0 && (strcmp(words[i]," ") != 0)){  
                count++;  
            }   
        }  
        //If maxCount is less than count then store value of count in maxCount   
        //and corresponding word to variable word  
        if(count > maxCount){  
            maxCount = count;  
            strcpy(word, words[i]);  
        }  
    }  
      
    printf("Most repeated word: %s", word);  
    fclose(fp);  
      
    //return 0;  
 }  
}	

// Function to execute builtin commands
int ownCmdHandler(char** parsed)
{
	int NoOfOwnCmds = 7, i, switchOwnArg = 0;
	char* ListOfOwnCmds[NoOfOwnCmds];
	char* username;

	ListOfOwnCmds[0] = "exit";
	ListOfOwnCmds[1] = "cd";
	ListOfOwnCmds[2] = "help";
	ListOfOwnCmds[3] = "hello";
	ListOfOwnCmds[4] = "line";
	ListOfOwnCmds[5] = "tenLine";
	ListOfOwnCmds[6] = "repeated";

	for (i = 0; i < NoOfOwnCmds; i++) {
		if (strcmp(parsed[0], ListOfOwnCmds[i]) == 0) {
			switchOwnArg = i + 1;
			break;
		}
	}

	switch (switchOwnArg) {
	case 1:
		printf("\nGoodbye\n");
		exit(0);
		exit(0);
	case 2:
		chdir(parsed[1]);
		return 1;
	case 3:
		openHelp();
		return 1;
	case 4:
		username = getenv("USER");
		printf("\nHello %s.\nMind that this is "
			"not a place to play around."
			"\nUse help to know more..\n",
			username);
		return 1;
	case 5:
		line(parsed[1]);
		return 1;
	case 6:
		tenLine(parsed[1]);
		return 1;
	case 7:
		repeated(parsed[1]);
		return 1;
	default:
		break;
	}

	return 0;
}

// function for finding pipe
int parsePipe(char* str, char** strpiped)
{
	int i;
	for (i = 0; i < 2; i++) {
		strpiped[i] = strsep(&str, "|");
		if (strpiped[i] == NULL)
			break;
	}

	if (strpiped[1] == NULL)
		return 0; // returns zero if no pipe is found.
	else {
		return 1;
	}
}

// function for parsing command words
void parseSpace(char* str, char** parsed)
{
	int i;

	for (i = 0; i < MAXLIST; i++) {
		parsed[i] = strsep(&str, " ");

		if (parsed[i] == NULL)
			break;
		if (strlen(parsed[i]) == 0)
			i--;
	}
}

int processString(char* str, char** parsed, char** parsedpipe)
{

	char* strpiped[2];
	int piped = 0;

	piped = parsePipe(str, strpiped);

	if (piped) {
		parseSpace(strpiped[0], parsed);
		parseSpace(strpiped[1], parsedpipe);

	} else {

		parseSpace(str, parsed);
	}

	if (ownCmdHandler(parsed))
		return 0;
	else
		return 1 + piped;
}

int main()
{
	char inputString[MAXCOM], *parsedArgs[MAXLIST];
	char* parsedArgsPiped[MAXLIST];
	int execFlag = 0;
	init_shell();
	signal(SIGINT, sigintHandler);

	while (1) {
		// print shell line
		printDir();
		// take input
		if (takeInput(inputString))
			continue;
		// process
		execFlag = processString(inputString,
		parsedArgs, parsedArgsPiped);
		// execflag returns zero if there is no command
		// or it is a builtin command,
		// 1 if it is a simple command
		// 2 if it is including a pipe.

		// execute
		if (execFlag == 1)
			execArgs(parsedArgs);

		if (execFlag == 2)
			execArgsPiped(parsedArgs, parsedArgsPiped);
	}
	return 0;
}
