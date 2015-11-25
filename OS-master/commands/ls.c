#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <dirent.h>
#include <time.h>
#include <string.h>

char base[256];
int baseChange;

void displayall(struct dirent* d){
	struct stat buf;
	if( !(stat(d->d_name,&buf))){
	    printf( (S_ISDIR(buf.stm_ode)) ? "d" : "-");
	    printf( (buf.st_mode & S_IRUSR) ? "r" : "-");
	    printf( (buf.st_mode & S_IWUSR) ? "w" : "-");
	    printf( (buf.st_mode & S_IXUSR) ? "x" : "-");
	    printf( (buf.st_mode & S_IRGRP) ? "r" : "-");
	    printf( (buf.st_mode & S_IWGRP) ? "w" : "-");
	    printf( (buf.st_mode & S_IXGRP) ? "x" : "-");
	    printf( (buf.st_mode & S_IROTH) ? "r" : "-");
	    printf( (buf.st_mode & S_IWOTH) ? "w" : "-");
	    printf( (buf.st_mode & S_IXOTH) ? "x" : "-");
	    printf(" %2d",(int)buf.st_nlink );
	    printf(" %5d",(int)buf.st_size );
	    printf(" %10s",d->d_name );
		printf("\n");
	}
}

void display(DIR* d, int all, int list){
	if( d==NULL ){
		printf("No such directory exists!\n");
		return;
	}
	struct dirent* dir;
	while( (dir=readdir(d))!=NULL ){

		if( all == 0 ){
			if( dir->d_name[0]!='.' ){
				if(list==0) printf("%s\t",dir->d_name);
				else		displayall(dir);
			}
		}else{
			if(list==0) printf("%s\t",dir->d_name);
			else		displayall(dir);	
		}
	}
	printf("\n");
}

int main(int argc, char** argv){
	DIR* d = opendir(".");
	int all = 0, list = 0, i, j;
	for(i=1; i<argc; i++){
		if( argv[i][0]=='-' ){
			int l = strlen(argv[i]);
			for(j=1; j<l; j++){
				if( argv[i][j]=='a' )	all = 1;
				else if( argv[i][j]=='l' )	list = 1;
				else{
					printf("Invalid options!\n");
					return 0;
				}
			}
		}
		else if( baseChange == 0 ){
			d = opendir(argv[i]);
			baseChange = 1;
		}else{
			printf("Too many operands!\n");
		}
	}
	display(d, all, list);
	return 0;
}