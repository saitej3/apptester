#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <float.h>
#include <fcntl.h>
#include <limits.h>
 
#define  FIFO_Name "FIFO"
 
int main()
{   
    FILE *inasc;
    float fltmax=FLT_MAX;
    float aux;
    int ffopen;
 
     
    if((ffopen = open(FIFO_Name,O_WRONLY)) < 0)
    {
        perror("\nFIFO not open\n");
        exit(1);
    }
     
    if((inasc=fopen("input.asc","rb")) < 0)
    {
        perror("\nERROR OPEN THE FILE input.asc\n");
        exit(1);
    }
 
     
    else
    {
        printf("\nOPEN THE FILE input.asc\n");
        while(!feof(inasc))
        {
            int ret=fscanf(inasc, "%f", &aux);  
            if(ret==1){         
                if(aux<FLT_MAX){
                    write(ffopen,&aux,sizeof(aux)); 
                    printf("DATA SEND TO FIFO %f\n",aux);
                }
            }               
             
        }
    /**/
    write(ffopen,&fltmax,sizeof(fltmax));
    close(ffopen);
    }
return 0;
}
