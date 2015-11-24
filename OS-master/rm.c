#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int removefiles(char *path)
{
 FILE *fp=fopen(path,"r");
 	if(fp==NULL)
	{
		perror("File not found!");
	}
	else
	{
		unlink(path);
	}
}

int main(int argc,char **argv)
{
 	int i;
	if(argc<2)
	{
		perror("nothing to remove");
	}
	else
	{
		if(argv[1][0]=='-' && argv[1][1]=='i')
		{
			printf("Enter y to continue");
			char ch;
			scanf("%c",&ch);
			if(ch=='y')
			{
				for(i=2;i<argc;i++)
				{
					removefiles(argv[i]);
				}
			}
			else
				return 0;
		}
		else{
			for(i=1;i<argc;i++)
			{
				removefiles(argv[i]);   					}
		    }
		}
	}
	
