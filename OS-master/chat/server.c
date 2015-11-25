#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <float.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/un.h>
#include <unistd.h>
#include <sys/stat.h>
 
#define FIFO_Name "FIFO"
 
int main()
{
    remove(FIFO_Name);
    remove("input.bin");
     
    int inbin;
    FILE *bin;
    FILE *outasc;
 
    int conta = 0;
     
     
    int ff = mkfifo(FIFO_Name, O_CREAT|O_EXCL | 0777);
    if (ff < 0)
    {
        perror("\nFIFO NOT CREATED\n");
        exit(1);
    }
    else
    {
        printf("\nFIFO CREATED! CONECTING...\n");
    }
 
     
    int ffread = open(FIFO_Name,O_RDONLY);
    if(ffread < 0)
    {
        perror("\nISN'T POSSIBLE READ THE FIFO!\n");
        exit(1);
    }
    else
    {
        printf("\nEND OF READING.\n");
    }
 
    float TEMP;
     
     
    if((inbin = open("input.bin",O_WRONLY|O_TRUNC|O_CREAT,0600)) < 0)
    {
        perror("\nERROR NO input.bin!\n");
        exit(1);
    }
     
    else
    {
        printf("\n FILE input.bin READ\n");
        printf("-------------------------\n\n");
        while(TEMP != FLT_MAX)
        {
            read(ffread, &TEMP, sizeof(TEMP));
            if(TEMP<FLT_MAX){                
                write(inbin,&TEMP,sizeof(TEMP));
                printf("DATA RECEIVED IN CLIENT: %f\n",TEMP);
                conta++;
            }                               
             
        }
    }
     
     
    int closeinbin = close(inbin);
    if(closeinbin < 0)
    {
        perror("\nERROR CLOSE THE FILE input.bin\n");
        exit(1);
    }
    float tab[conta];
 
     
    int readinbin = fread(&tab,sizeof(tab),1,(bin=fopen("input.bin","r")));
    if(bin < 0 || readinbin < 0)
    {
        perror("\nERROR TRY TO OPEN FILE input.bin\n");
        exit(1);
    }
     
    if(fclose(bin) < 0)
    {
        perror("\nERROR CLOSE THE FILE input.bin!\n");
        exit(1);
    }
    remove(FIFO_Name);
     
 
 
    if((outasc=fopen("output.asc","a")) < 0) /
    {
        perror("ERROR OPEN output.asc");
        exit(1);
    }
 
     
    int i;
     
    printf("\n\n IN FILE ASCII WAS WRITE:\n");
    printf("-------------------------------------------\n");
    for(i = 0; i < conta; i++)
    {
        char BUFFER[20]; 
        tab[i] = 3.0*tab[i];    
        sprintf(BUFFER,"%lf",tab[i]);
        fputs(BUFFER,outasc);
        fputs("\n",outasc);
        printf("WRITE %f \n",tab[i]);
    }
 
    if(fclose(outasc) < 0)
    {
        printf("ERROR CLOSE THE FILE");
        exit(1);
    }
     
    return 0;
}
