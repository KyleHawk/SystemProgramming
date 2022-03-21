#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<math.h>

#include<string.h>
#include<stdlib.h>

#define MAXSIZE 1024

// the name of original picture saved in argv[1]
// The file name after the scroll down is saved in argv[2]
int main(int argc, char *argv[])
{
    int fd1,fd2;
    char buffer[MAXSIZE];        //use for save data
    int nbLines, nbCols;   

    FILE *fp;
    int threelinelen = 0; //how many characters the first three rows have
	int bufferlen = 0;


    /*use function fgets() get the first three rows and calculate how many characters they have*/
   if((fp=fopen(argv[1],"rb"))==NULL){
		printf("this file cannot open\n");
		exit(1);
	}

    for(int i = 1; i<=3; i++)
    {
        int len = 0;
        if(fgets(buffer,MAXSIZE,fp)!=NULL)
        {
            len = strlen(buffer);
        }
        threelinelen = threelinelen + len;

        if(i==2)          //The rows and columns of the image are saved in the second row
        {
            sscanf(buffer,"%d %d",&nbLines,&nbCols);
            printf("The number of lines of the picture: %d\n", nbLines);
            printf("The number of columns of the picture: %d\n", nbCols);
        }
    }
	fclose(fp);


    /*Open two files for read() and write() operations*/
    if((fd1 = open(argv[1], O_RDONLY)) == -1 || ((fd2 = open(argv[2], O_CREAT|O_WRONLY|O_TRUNC, 0700)) == -1))
    {
        perror("file problem ");
        exit(2);
    }

    read(fd1,buffer,threelinelen);         //Read the information about the picture from the m1.pgm file and write it to m2.pgm
    write(fd2, buffer, threelinelen);

    lseek(fd1,threelinelen, SEEK_SET);    // the pointer is set to end of 3rd line and read() will start from 4th line
    for(int i = nbLines - 1; i>=0 ;i--)   //Read from the last line of m1.pgm and write the read data to m2.pgm
    {
        lseek(fd1,threelinelen+i*nbCols,SEEK_SET);

        read(fd1, buffer, nbCols);
        
		bufferlen=strlen(buffer);

        lseek(fd2,0, SEEK_END);
        write(fd2, buffer, bufferlen);
	}
    close(fd1);
    close(fd2);

    printf("The up-and-down image has been saved to: %s\n", argv[2]);
    exit(0);

}//
