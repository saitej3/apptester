#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char buffer[500];

void catcommand(char *file)
{
	char buf[200];
	FILE *fp=fopen(file,"r");
	if(fp==NULL)
	{
		perror("File has no information!");
	}
	else	
	{
		while ((fgets(buf,200,(FILE *) fp)))
		{
			strcat(buffer,buf);                                                                                                                                                                                                            
		}
		printf("%s",buffer);
	}	
}

int main(int argc,char *argv[])
{
	int i;
	if(argc<2)
	{
		perror("Please specify file name");
	}
	else
	{
		for(i=1;i<argc;i++)
		{
			catcommand(argv[i]);
		}
	}
}