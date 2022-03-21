#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<math.h>

#include<string.h>
#include<stdlib.h>

#define MAXSIZE 1024000
int main(int argc, char *argv[])
{
    int fd1,fd2;
    char nbuffer[MAXSIZE];
    long int n1;

    if((fd1 = open(argv[1], O_RDONLY)) == -1 ||
        ((fd2 = open(argv[2], O_CREAT|O_WRONLY|O_TRUNC,
        0700)) == -1))
    {
        perror("file problem ");
        exit(1);
    }

    FILE *fp;
	char buffer[1000];
	int bufferlen;
	int lineCount=0;

   if((fp=fopen(argv[1],"rb"))==NULL){
		printf("文件不能打开\n");
		exit(0);
	}

    while(fgets(buffer,100000,fp)!=NULL){
		bufferlen=strlen(buffer);
		lineCount++;
            printf("%d\n",bufferlen);
        lseek(fd2,0, SEEK_END);
        write(fd2, buffer, bufferlen);
	}
	fclose(fp);
	printf("行数：%d\n",lineCount);

 
/*
    while(lineCount >= 0)
    {
        lineCount = lineCount -1;
        lseek(fd1,(lineCount*bufferlen),SEEK_SET);
        read(fd1,nbuffer,bufferlen);
        lseek(fd2,0, SEEK_END);
        write(fd2, nbuffer, bufferlen);
    }*/




    /*if((n1=read(fd1, buffer, MAXSIZE)) > 0)
       {
        printf("%d\n",n1);
        double n ;
        n =  sqrt(n1);
        printf("%f\n",n);
        
        for(int x = n - 1 ;x>=0;x--)
        {
            char another[1024];
            for(int i=0; i<n;i++)
            {
                another[i] = buffer[(int)(n*x)+i];
            }
            lseek(fd2,0, SEEK_END);
            write(fd2, another, (int)n+1);
            //printf("%c\n",buffer[(int)(n*x)]);
        }

       }*/

    //printf("%s\n",buffer);

    exit(0);

}//