//This program implements reading bash files and differentiating between commands to know how many pipes are in each line
//Simple bash file reading and command execution were possible, but there were problems

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>

#define MAXSIZE 1024


void onlyOne(char *command[])
{
    execlp(command[0], command[0], command[1], command[2], command[3], command[4], command[5], NULL);

}

void parent(int *fd, char *command[]){ // A writer
    close(fd[0]);
    dup2(fd[1], 1); 
    close(fd[1]); 
    execlp(command[0], command[0], command[1], command[2], command[3], command[4], command[5],NULL);
}
void child(int *fd, char *command[]){ // A reader
    close(fd[1]);
    dup2(fd[0], 0); 
    close(fd[0]); 
    execlp(command[0], command[0], command[1], command[2], command[3], command[4], command[5],NULL);
}

void threeCommand(int *fd, char *command[]){ // A writer
    dup2(fd[1], 1); 
    dup2(fd[0], 0); 
    close(fd[0]); 
    execlp(command[0], command[0], command[1], command[2], command[3], command[4], command[5],NULL);
}






int main(int argc, char *argv[]){
    char buffer[MAXSIZE];
    char oneLine[MAXSIZE];       //ALL CHARS IN A LINE EXCEPT'\N'
    char lineCommand[MAXSIZE];   // remove the beginning blank
    char command[20][MAXSIZE];     //save all of the strings in a line
    char onecommand[MAXSIZE];   //save one string
    int strNumber[MAXSIZE];     //string numbers in different commands
    char *final[20] ={NULL};





    FILE *fd;
    fd=fopen(argv[1],"rb");

    

    int len = 0;           //get the length of first line
    if(fgets(buffer,MAXSIZE,fd)!=NULL)
    {
        len = strlen(buffer);
    }

    //read from the second line of bash file
    int fp = open(argv[1], O_RDONLY);
    lseek(fp,len, SEEK_SET); 
    int n = read(fp,buffer,MAXSIZE);    //read the file 
    printf("%i\n",n);
    
    /*Separate each line in the document
According to the "|" and ";" divisional command, according to the space to distinguish the string*/
    for(int p = 0; p<20; p++)
    {
        memset(command[p], 0, sizeof(command[p]));
    }

    for(int i=0, j=0;i<n;i++)
    {
        if(buffer[i] != '\n')       //
        {
            oneLine[j] = buffer[i];
            j++;
        }
        else
        {
            oneLine[j] = buffer[i];
            if(strlen(oneLine) != 1)
            {
                //printf("%s\n", oneLine);
                if(oneLine[0] != ' ')
                {
                    for(int k=0; k<sizeof(oneLine); k++)
                    {
                        lineCommand[k] = oneLine[k];
                    }
                    printf("%s\n", lineCommand);
                }
                else
                {
                    int k=0;
                    for(; oneLine[k] == ' '; )
                    {
                        k++;
                    }

                    for(int y=0; y<sizeof(oneLine);y++)
                    {
                        lineCommand[y] = oneLine[k+y];
                    }
                    printf("%s\n", lineCommand);
                }

                int number = 0;    //how many command in a line
                int strOfOneCommand = 1; // how many strings in a command

                int strNum = 0;    // how many string in a line
                for(int k = 0, z = 0; k<sizeof(lineCommand); k++)
                {
                    if(lineCommand[k] != '|'&& lineCommand[k] != ';' && lineCommand[k] != '\n' )
                    {
                        //printf("%i\n",3);
                       if(lineCommand[k] != ' ' )
                       {
                           onecommand[z] = lineCommand[k];
                           //printf("%i%c\n",0,onecommand[z]);
                            z++;
                       }
                       else
                       {
                           
                           for(int w = 0; w<strlen(onecommand);w++)
                           {
                               command[strNum][w] = onecommand[w];
                           }
                           
                            //printf("%i %s\n", 1,command[strNum]);
                            final[strNum] = command[strNum];
                            ///printf("%i %s\n", 888,final[strNum]);
                           memset(onecommand, 0, sizeof(onecommand));
                           z=0;
                           strOfOneCommand++;
                           strNum++;
                       }
                       
                    }
                    else
                    {
                        strNumber[number]=strOfOneCommand;

                        //printf("%i%i\n", 6,strNumber[number-1]);  
                        for(int w = 0; w<strlen(onecommand);w++)
                           {
                               command[strNum][w] = onecommand[w];
                           }
                        //printf("%i %s\n", 2,command[strNum]);
                        final[strNum] = command[strNum];
                        //printf("%i %s\n", 888,final[strNum]);
                        memset(onecommand, 0, sizeof(onecommand));

                        strOfOneCommand=1;
                        strNum++;
                        z=0;
                        number++;
                        

                    }
                    
                }

                if(number == 1)
                {
                    final[1]=NULL;
                    if(fork>0)

                    onlyOne(final);
                        
                }
                else if(number == 2)
                {
                    int fd[2];
                    char *secondfinal[10]={NULL};
                    int a = strNumber[0];
                    for(int b = 0; b<strNumber[1];b++)
                    {
                        secondfinal[b] = final[a+b];
                        final[a+b] = NULL;
                    }
                    pipe(fd);
                    if(fork() > 0)
                    parent(fd, final);
                    else 
                    child(fd, secondfinal);

                }
                else if(number == 3)
                {
                    int fd[2];
                    pipe(fd);
                    if(fork() > 0)
                    {
                        parent(fd, final);
                        threeCommand(fd, final);
                    }
                    
                    else 
                    child(fd, final);

                }


                printf("%s %i \n", "number", number);  
                for(int q = 0; q<number; q++)
                {
                    printf("%i\n", strNumber[q]);  
                    strNumber[q]=0;
                }
                number=0;
                
            }

           

            j = 0;
            memset(oneLine, 0, sizeof(oneLine));
            memset(lineCommand, 0, sizeof(lineCommand));

            for(int p = 0; p<20; p++)
            {
                memset(command[p], 0, sizeof(command[p]));
            }

            for(int p=0;p<6;p++)
            {
                final[p]=NULL;
            }
            
            printf("\n\n\n");
        }
            
    }

    //printf("%i\n",j);

}