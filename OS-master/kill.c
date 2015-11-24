#include <sys/types.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>	
#include <unistd.h>

int main(int argc, char** argv){
	if (argc < 2)
    {
        printf("Enter process to kill\n");
    }else{
    	int i, j, signo = 0, pids = 0;
    	char pid[10][256];
    	for(i=1; i<argc; i++){
			if( argv[i][0]=='-' ){
				int l = strlen(argv[i]);
				for(j=1; j<l; j++){
					if( argv[i][j]>='0' && argv[i][j]<='9' )	signo = (signo*10) + argv[i][j];
					else{
						printf("Invalid signal number!\n");
						return 0;
					}
				}
			}
			else{
				strcpy(pid[pids++],argv[i]);
			}
		}
		for(j=0; j<pids; j++){
			kill(atoi(pid[j]), SIGKILL);
		}
    }
    return 0;
}