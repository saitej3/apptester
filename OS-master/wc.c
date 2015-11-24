#include <stdio.h>
#include <stdlib.h>

void countchar(char *file)
{
	FILE *fp=fopen(file,"r");
	char ch;
	int charc=0;
	while((ch=fgetc(fp))!=EOF)
		{
			charc++;
		}
	printf("%s\t%d","charecter count is",charc);
}

void countline(char *file)
{
	FILE *fp=fopen(file,"r");
	char ch;
	int linec=0;
	while((ch=fgetc(fp))!=EOF)
		{
			if(ch=='\n')
				{
					linec++;
				}
		}
	printf("%s\t%d","line count is",linec);
}

void countword(char *file)
{
	FILE *fp=fopen(file,"r");
	char ch;
	int wordc=0;
	while((ch=fgetc(fp))!=EOF)
		{
			if(ch=='\t')
				{
					wordc++;
				}
		}
	printf("%s\t%d","word count is",wordc);
}

int main(int argc , char *argv[])
{
        int i;
	if(argc<2)
	{
		perror("the format is wc [file1] [file2].....");
	}
		if(argv[1][0]=='c')
		{
			for(i=2;i<argc;i++)
				{		
	
					if(argv[i]!='\0')
						{
							countchar(argv[i]);
						}
				}
		}
		else if(argv[1][0]=='l')
		{
			for(i=2;i<argc;i++)
				{		
	
					if(argv[i]!='\0')
						{
							countline(argv[i]);
						}
				}
		}
		
 	        else if(argv[1][0]=='w')
		{
			for(i=2;i<argc;i++)
				{		
	
					if(argv[i]!='\0')
						{
							countword(argv[i]);
						}
				}
		}
		else
		{
                for(i=1;i<argc;i++)
				{		
	
					if(argv[i]!='\0')
					{
					countchar(argv[i]);
					countline(argv[i]);
					countword(argv[i]);
					}
				}
		}
}
		

	
