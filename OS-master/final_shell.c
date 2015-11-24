#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <fcntl.h>
#define BUFSIZE 1024
#define TOKSIZE 64
#define DELIMS " \t\r\n\a"
char *BASEPATH;

char *read_line()
{
  char *line = NULL;
  ssize_t bufsize = 0;
  getline(&line, &bufsize, stdin);
  return line;
}

char **split_line(char *line)
{
  int bufsize = TOKSIZE, position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token;

  if (!tokens) {
    fprintf(stderr, "shell: Allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, DELIMS);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += TOKSIZE;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) {
        fprintf(stderr, "shell: Allocation error\n");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, DELIMS);
  }
  tokens[position] = NULL;
  return tokens;
}

char* ioredirect_input(char** args)
{
        int i;
        char* ip_file=(char*)malloc(sizeof(char)*100);
        for(i=0; args[i]; i++){
                if(!strcmp(args[i],"<")){
                        if(args[i+1]!=NULL){
                                ip_file=strcpy(ip_file,args[i+1]);
                                return ip_file;
                        }else{
                                free(ip_file);
                                return NULL;
                        }
                        break;
                }
        }
        return NULL;
}

char* ioredirect_output(char** args)
{
        int i;
        char* op_file=(char*)malloc(sizeof(char)*100);
        for(i=0; args[i]; i++){
                if(!strcmp(args[i],">")){
                        if(args[i+1]!=NULL){
                                op_file=strcpy(op_file,args[i+1]);
                                return op_file;
                        }else{
                                free(op_file);
                                return NULL;
                        }
                        break;
                }
        }
        return NULL;
}

void ioredirect_remove_tokens(char **args)
{
        int i=0;
        while(args[i]!=NULL)
        {
                if(!strcmp(args[i],"<") || !strcmp(args[i],">"))
                {
                        args[i]=NULL;
                        return;
                }
                i++;
        }
}

int lsh_launch(char **args)
{
  pid_t pid, wpid;
  int status;

  pid = fork();
  if (pid == 0) {
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
                                dup2(op_fd,STDOUT_FILENO);
                                close(op_fd);
                        }
                }
                ioredirect_remove_tokens(args);


        if (execvp(args[0], args) == -1) {
      perror("shell");
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    perror("shell: ");
  } else {
    do {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}


int cd(char **args);
int help(char **args);
int exit1(char **args);

char *builtin_str[] = {
  "cd",
  "help",
  "exit"
};

int (*builtin_func[]) (char **) = {
  &cd,
  &help,
  &exit1
};

int lsh_num_builtins() {
  return sizeof(builtin_str) / sizeof(char *);
}

int cd(char **args)
{
  if (args[1] == NULL) {
    fprintf(stderr, "shell: Expected argument to \"cd\"\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("shell: ");
    }
  }
  return 1;
}

int help(char **args)
{
  int i;
  printf("Type program names and arguments, and hit enter.\n");
  printf("The following are built in:\n");
  for (i = 0; i < lsh_num_builtins(); i++) {
    printf("  %s\n", builtin_str[i]);
  }
  printf("Use the man command for information on other programs.\n");
  return 1;
}

int exit1(char **args)
{
  return 0;
}

int execute(char **args)
{
  int i;
  if (args[0] == NULL) {
    return 1;
  }
  for (i = 0; i < lsh_num_builtins(); i++) {
    if (strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  }
  return lsh_launch(args);
}

void display(char *BASEPATH){

}

void loop()
{
  char *line;
  char **args;
  int status;
  do {
    printf(">> ");
    line = read_line();
    args = split_line(line);
    status = execute(args);
    free(line);
    free(args);
  } while (status);
}

int main(int argc, char **argv)
{
  loop();
  return EXIT_SUCCESS;
}

