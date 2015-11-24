#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <fcntl.h>
#include <signal.h>

const int BUFFER_SIZE = 100;
enum BOOL{
	FALSE, TRUE
};

enum ERROR
{
	EIO=-100,
	EMEMORY,
	EFAILURE,
	EFORK,
	ESYNTAX
};

void sigintHandler(int sig_num){
	return;
}

void display_prompt()
{
	char cwd[BUFFER_SIZE],*t,prompt[BUFFER_SIZE];
	strcpy(prompt,"kshell:~");
	if((t=getcwd(cwd,sizeof(cwd)))!=NULL)
	{
		strcat(prompt,t);
	}
	strcat(prompt,">>");
	printf("%s",prompt);
}

int count;

void read_line(char **args){
	char c, buf[100];
	int i;
	count = 0;
	do{
		for(i=0; i<100; i++){
			scanf("%c",&c);
			if( c==' ' ){
				buf[i] = '\0';
				args[count] = (char *) malloc(i+2);
				strcpy(args[count],buf);
				break;
			}else if( c=='\n' ){
				buf[i] = '\0';
				args[count] = (char *) malloc(i+2);
				strcpy(args[count++],buf);
				args[count] = (char *) malloc(2);
				strcpy(args[count],"\0");
				return;
			}	
			buf[i] = c;		
		}
		count++;
	}while( TRUE );

}

int is_background(char **args)
{
	return 1;
	int i=0;
	while(args[i]!=NULL)
	{
		if(!strcmp(args[i],"&"))
		{
			args[i]=NULL;
			return 1;
		}
		i++;
	}
	return 0;
}

char* ioredirect_input(char** args)
{
	int i;
	char* ip_file=(char*)malloc(sizeof(char)*BUFFER_SIZE);
	i=0;
	while(args[i]!=NULL)
	{
		if(!strcmp(args[i],"<"))
		{
			if(args[i+1]!=NULL)
			{
				ip_file=strcpy(ip_file,args[i+1]);
				
				return ip_file;
			}
			else 
			{
				free(ip_file);
				return NULL;
			}
			break;
		}
		i++;
	}
	return NULL;
}

char* ioredirect_output(char** args)
{
	int i;
	char* op_file=(char*)malloc(sizeof(char)*BUFFER_SIZE);
	i=0;
	while(args[i]!=NULL)
	{
		if(!strcmp(args[i],">"))
		{
			if(args[i+1]!=NULL)
			{
				op_file=strcpy(op_file,args[i+1]);
				return op_file;
			}
			else
			{
				free(op_file);
				return NULL;
			}
			break;
		}
		i++;
	}
	free(op_file);
	return NULL;
}

void ioredirect_remove_tokens(char **args)
{
	int i=0;
	while(args[i]!=NULL)
	{
		if(!strcmp(args[i],"<") || !strcmp(args[i],">"))
		{
			printf("got it\n");
			args[i]=NULL;
			return;
		}
		i++;
	}
}


int exec_child(char **args,int background)
{
	pid_t pid,wpid;
	int status;
	pid=fork();
	if(pid==0)
	{
		if(execvp(args[0],args)==-1)
			perror("exec_child");
		exit(EFAILURE);
	
/*
		char *ip_file,*op_file;
		int ip_fd=-1,op_fd=-1;

		ip_file=ioredirect_input(args);
		op_file=ioredirect_output(args);

		if(ip_file!=NULL)
		{
			ip_fd=open(ip_file,O_RDONLY);
			if(ip_fd==-1)
				perror("opening ip_file");
			else
			{
				//printf("success\n"); 
				dup2(ip_fd,STDIN_FILENO);
				close(ip_fd);
			}
		}
		if(op_file!=NULL)
		{
			op_fd=open(op_file,O_WRONLY|O_CREAT,0666);
			if(op_fd==-1)
				perror("opening op_file");
			else
			{
				//printf("success\n");
				dup2(op_fd,STDOUT_FILENO);
				close(op_fd);
			}
		}
		ioredirect_remove_tokens(args);
		//IO_REDIRECTION HANDLING ENDS//////////////////////////////////

		if(execvp(args[0],args)==-1)
			perror("exec_child");
		exit(EFAILURE);
*/	
	}
//	else if(pid<0)exit(EFORK);
	else
	{
		if(background==0)
		{
			do{
				wpid=waitpid(pid,&status,WUNTRACED);
			}while(!WIFEXITED(status) && !WIFSIGNALED(status));
		}
		else return 1;
	}
	return 1;
}

int execute(char **args)
{
	if(args==NULL || args[0]==NULL)
		return 1;
	
	int i=0,len;
	int background=is_background(args);
	int status=exec_child(args,background);
	return status;
}


void shell(){
	char *args[10];
	int status, i;
	status=TRUE;
	do{
    	display_prompt();
    	read_line(args);
    	for(i=0; i<=count; i++){
    		printf("%s\n",args[i]);
    	}
  		status=execute(args);
  	}while (status);
}

int main(){
	signal(SIGINT, sigintHandler);
	shell();
	return 0;
}