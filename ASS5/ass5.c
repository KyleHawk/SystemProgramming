#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

//Author:Kyle


int main(int argc, char *argv[])
{
    //Prompts the message "This program is simple calculator"
    printf("This program is simple calculator.\n");

    //Gets in an infinite loop 
    while(1)
    {
        char* ch;
        int fd;
        //1. Writes the message "Enter an expression, e.g., 134.5 + 456> ",
        ch = "Enter an expression, e.g., 134.5 + 456> ";
        int len = strlen(ch);
        write(1,ch,len);
        
        //2. Use "read()" to read the whole input line as a string.
        char str[1024];
        len = read(0,str,1024);




        pid_t pid;
        int status;
        //3. Forks
        pid = fork();

        if(pid==0)
        {
            // 3.2 – the child process calls the function childFunction(char *) and never returns.
            childFunction(str);
        }
        else if(pid > 0)
        {
            //3.1 – the parent writes the message "Created a child to perform task, waiting...", 
            // then calls wait() to wait for its child.
            ch = "Created a child to perform task, waiting...\n";
            write(1,ch, strlen(ch));
            wait(&status);
            
            // 5.1 Once the child terminates, the parent checks the returned status value
            if(WEXITSTATUS(status) == 55)
            {
                //5.2 if it is 55, writes on the screen "Wrong expression" and goes back to 1.
                ch = "Wrong expression\n\n";
                write(1,ch, strlen(ch));
            }
            else
            {
                // 6.1 Otherwise, the parent opens file result.txt for reading, 
                double result = 0;
                char sum[8];
                fd = open("result.txt", O_RDONLY);

                // 6.2  reads the value (type double),
                read(fd,sum,8);

                // 6.3 prints it on the screen, closes result.txt and goes back to 1.
                result = atof(sum);
                printf("The final result:%f\n\n", result);
                close(fd);
                

            }

        }

    }
    exit(0);



}

void childFunction(char *lines)
{
    // 4.1 – writes the message "I am a child working for my parent"
    char* ch = "I am a child working for my parent\n";
    write(1,ch,strlen(ch));

    double value1 = 0;
    double value2 = 0;
    double sum;
    char operator;
    int fd;

    //4.2 – extracts the two values and operator (it is not necessary that there is a blank 
    //to separate the operator from operands, e.g. 23+11 or 23 + 11 are both valid),
    int number = sscanf(lines, "%lf %c %lf", &value1, &operator, &value2);

    if(number == 3)
    {
        if( operator == '+')
        sum = value1 + value2;
        else if ( operator == '-')
            sum = value1 - value2;
        else if ( operator == '*')
            sum = value1 * value2;
        else if ( operator == '/')
            sum = value1 / value2;
        else
            exit(55);  //4.3 – in case of input error, the child calls exit(55),
    }
    else
    {
        exit(55);     //4.3 – in case of input error, the child calls exit(55),
    }


    char result[8];
    sprintf(result,"%lf",sum);
    // 4.4 – opens a file called result.txt, for writing and truncates it, to get rid of old contents,
    fd=open("result.txt", O_WRONLY|O_CREAT);

    // 4.5 – writes the value of the expression in result.txt, then closes the file.
    write(fd, result, 8);
    close(fd);
    
    // 4.6 – calls exit(0)
    exit(0);

}