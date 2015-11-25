#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>

void display(DIR* d, int all){
	if( d==NULL ){
		printf("Could not retrieve!\n");
		return;
	}
	struct dirent* dir;
	while( (dir=readdir(d))!=NULL ){

		printf("%s\t",dir->d_name);
		
	}
	printf("\n");
}

int main(int argc, char** argv){
	DIR* d = opendir("/proc");
	int all = 0, i, j;
	char user[256];
	for(i=1; i<argc; i++){
		if( argv[i][0]=='-' ){
			int l = strlen(argv[i]);
			for(j=0; j<l; j++){
				if( argv[i][j] == 'a' )	all = 1;
			}	
		}
		else{
			strcpy(user, argv[i]);
		}
	}
	display(d, all);
	return 0;
}