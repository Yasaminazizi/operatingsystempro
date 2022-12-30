// C Program to design a shell in Linux
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<readline/readline.h>
#include<readline/history.h>




// Greeting shell during startup
void init_shell()
{
	printf("\033[H\033[J");
	printf("\n\n\n\n");
	printf("\n\n\n\t****Welcome****");
	printf("\n\n\n\n");
	printf("\n\n\n if you don't know what commands to use,");
	printf("\nplease enter the 'help' command first.");
	printf("\n\n\n");
}

// Function to take input & history
int takeInput(char* str)
{
	char* buf;
	buf = readline("\n>>> ");
	FILE *history;
	history = fopen("history.txt" , "a");
	if (strlen(buf) != 0) {
		add_history(buf);
		strcpy(str, buf);
		fprintf(history,"%s\n", buf);
		fclose(history);
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
			printf("\nCould not execute command..");
		}
		exit(0);
	} else {
		// waiting for child to terminate
		wait(NULL);
		return;
	}
}

// Help command 
void openHelp()
{
	puts("\n***WELCOME TO MY SHELL ***"
		
		"\nList of Commands supported:"
		"\n>cd"
		"\n>ls"
		"\n>pwd"
		"\n>exit"
		"\n>crtl+c:The program does not end with crtl+c"
		"\n>firstWord:Returns the first part of each line separated by spaces in the file"
		"\n>repeated:Display the most repeated word in the file"
		"\n>noComment:show lines that are not comment of the file"
		"\n>noSpace:Remove all empty spaces from the file and display it"
		"\n>line:Display the number of file lines"
		"\n>tenLine:Display the first ten lines of the file"
		"\n>history:entered commands are stored in a file called history  "
		"\n and by pressing the up & down button finds the previous commands."
		"\n>all other general commands available in UNIX shell");
	
	return;
}

// Display the number of file lines
void line(char address[])
{
	
	FILE *fp;
	char c;
	int count = 0;
	fp = fopen(address, "r");
	if (fp== NULL){  
       		printf("File not found");  
       		return;  
    	}
	for(c = getc(fp); c != EOF; c = getc(fp))
		if(c == '\n')
			count = count + 1;
	fclose(fp);
	printf("Lines: %d\n",count);
	return;	
}

// crtl+c
void sigintHandler(int sig_num)
{	
 	signal(SIGINT, sigintHandler);
        printf("\n Cannot be terminated using Ctrl+C \n");
        printDir();
        fflush(stdout);
        printf("\n>>> ");
}

//display the first ten line of file
void tenLine(char address[])
{
	
	FILE *fp;
	char c;
	char s[1000] = {'\0'};
	int count = 0;
	int n = 0;
	fp = fopen(address, "r");
	if (fp== NULL){  
       		printf("File not found");  
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
	printf("First 10 lines of file:\n");
	puts(s);
	return;	
}

//display lines of file with no space
void noSpace(char address[])
{
	
	FILE *fp;
	char c;
	char s[1000] = {'\0'};
	int count = 0;
	int n = 0;
	fp = fopen(address, "r");
	if (fp== NULL){  
       		printf("File not found");  
       		return;  
    	}
	for(c = getc(fp); c != EOF ; c = getc(fp))
	{
		if(c != '\n' && c != '\t' && c != ' ')
		{
			s[n] = c;
			n = n + 1;
		}
	}
	fclose(fp);
	printf("File with no spaces:\n");
	puts(s);
	return;	
}

//display the first word of line in file
void firstWord(char address[])
{
	
	FILE *fp;
	char c;
	char s[1000] = {'\0'};
	int count = 0;
	int n = 0;
	fp = fopen(address, "r");
	if (fp== NULL){  
       		printf("File not found");  
       		return;  
    	}
	for(c = getc(fp); c != EOF ; c = getc(fp))
	{
		
		if(c == '\t' || c == ' ')
		{
			count = 1;
		}
		if(c == '\n')
		{
			count = 0;
		}
		if(count == 0)
		{
			s[n] = c;
			n = n + 1;
		}
	}
	fclose(fp);
	printf("First words of file:\n");
	puts(s);
	return;	
}

//display the most repeated word of file
void repeated(char address[])
{
	FILE *fp;
 	char ch, *line;  
   	size_t len = 0, read;  
     	char words[1000][1000], word[20];  
     	int i = 0, j, k, maxCount = 0, count;  
	fp = fopen(address,"r"); 
  	if (fp== NULL){  
       		printf("File not found");  
       		return;  
    	}	  
 
    	while ((read = getline(&line, &len, fp)) != -1) {      
	       for(k=0; line[k]!='\0'; k++){     
            		if(line[k] != ' ' && line[k] != '\n' && line[k] != ',' && line[k] != '.' ){  
            			   words[i][j++] = tolower(line[k]);  
            		}  
            		else{  
                		   words[i][j] = '\0';   
                		   i++;  
                		   j = 0;
                        }  
       		 }		  
    	}  
      
    	int length = i;    
    		for(i = 0; i < length; i++){  
        		count = 1;   
        		for(j = i+1; j < length; j++){  
            			if(strcmp(words[i], words[j]) == 0 && (strcmp(words[i]," ") != 0)){  
                			count++;  
            			}   
        		}   
        		if(count > maxCount){  
            			maxCount = count;  
            			strcpy(word, words[i]);  
        		}  
    		}  
        printf("Most repeated word: %s", word);  
        fclose(fp);  
        return ;
}

//display file with no comment
void noComment(char address[])
{
	FILE *fp;
	char c;
	int m=0;
	char s[10000] = {'\0'};
	int count = 0;
	int n = 0;
	fp = fopen(address, "r");
	if (fp== NULL){  
       		printf("File not found");  
       		return;  
    	}
	for(c = getc(fp) ; c != EOF ; c = getc(fp))
	{
		if(count == 0)
		{
			if(c == '#')
			{
				m=1;
			}
			count = 1;
		}
		if(m==0)
		{
			s[n] = c;
			n = n + 1;
		}
		if(c == '\n'){
			count = 0;
			m=0;
		}	
	}
	fclose(fp);
	printf("File with no comments:\n");
	puts(s);
	return;	
}

// Function to execute builtin commands
int ownCmdHandler(char** parsed)
{
	int NoOfOwnCmds = 9, i, switchOwnArg = 0;
	char* ListOfOwnCmds[NoOfOwnCmds];
	char* username;

	ListOfOwnCmds[0] = "exit";
	ListOfOwnCmds[1] = "cd";
	ListOfOwnCmds[2] = "help";
	ListOfOwnCmds[3] = "firstWord";
	ListOfOwnCmds[4] = "line";
	ListOfOwnCmds[5] = "tenLine";
	ListOfOwnCmds[6] = "repeated";
	ListOfOwnCmds[7] = "noComment";
	ListOfOwnCmds[8] = "noSpace";

	for (i = 0; i < NoOfOwnCmds; i++) {
		if (strcmp(parsed[0], ListOfOwnCmds[i]) == 0) {
			switchOwnArg = i + 1;
			break;
		}
	}
	pid_t pid = fork();

	if (pid == -1) {
		printf("\nFailed forking child..");		
		return 1;
	} else if (pid == 0) {
	
	switch (switchOwnArg) {
	case 1:
		printf("\nGoodbye\n");
		kill(0,SIGKILL);
		exit(0);
	case 2:
		chdir(parsed[1]);
		return 1;
		exit(0);
	case 3:
		openHelp();
		exit(0);
		return 1;
	case 4:
		firstWord(parsed[1]);
		exit(0);
		return 1;
	case 5:
		line(parsed[1]);
		exit(0);
		return 1;
	case 6:
		tenLine(parsed[1]);
		exit(0);
		return 1;
	case 7:
		repeated(parsed[1]);
		exit(0);
		return 1;
	case 8: 
		noComment(parsed[1]);
		exit(0);
		return 1;
	case 9: 
		noSpace(parsed[1]);
		exit(0);
		return 1;
		
	default:
		break;
	}
	
	}
	else
	{
	// waiting for child to terminate
		wait(NULL);
		return 1;
	}

	return 0;
}

// function for parsing command words
void parseSpace(char* str, char** parsed)
{
	int i;

	for (i = 0; i < 100; i++) {
		parsed[i] = strsep(&str, " ");

		if (parsed[i] == NULL)
			break;
		if (strlen(parsed[i]) == 0)
			i--;
	}
}

int processString(char* str, char** parsed)
{
	parseSpace(str, parsed);
	if (ownCmdHandler(parsed))
		return 0;
	else
		return 1;
}

int main()
{
	FILE *history;
	history = fopen("history.txt" , "w");
	
	
	char inputString[100], *parsedArgs[100];
	int execFlag = 0;
	init_shell();
	signal(SIGINT, sigintHandler);

	while (1) {
		
		printDir();
		
		if (takeInput(inputString))
			continue;
		
		execFlag = processString(inputString,
		parsedArgs);
		
		if (execFlag == 1)
			execArgs(parsedArgs);
	}
	return 0;
}
