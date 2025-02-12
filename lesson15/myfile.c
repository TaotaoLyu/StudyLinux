#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>

int main()
{
   
    int fd=open("log.txt",O_WRONLY|O_TRUNC);
    pid_t id=fork();
    if(id==0)
    {
        int fd2=open("hello.txt",O_WRONLY|O_CREAT|O_TRUNC,0666);
        printf("child fd: %d\n",fd2);
        return 0;
        char* buffer=(char*)"i_love_china\n";
        int ret=write(3,buffer,strlen(buffer));
        if(ret==-1)
        {
            perror("write");
        }
        else 
        {
            printf("write bytes: %d\n",ret);
        }
        return 0;
    }
    close(fd);
    return 0;

    //read redirect code
    /*int fd=open("log.txt",O_RDONLY);
    if(fd<0)//handling error conditino
    {
        perror("open");
        return errno;
    }
    dup2(fd,0);//more concise to write this way
    

    char buffer[64];
    while(fgets(buffer,sizeof(buffer),stdin)) printf("%s",buffer);
    sleep(3);
    
    
    close(fd);
    */
    //write redirect code
    /*
    umask(0);
    int fd=open("log.txt",O_WRONLY | O_CREAT | O_TRUNC,0666);
    dup2(fd,1);//more concise to write this way
    printf("open: %d\n",fd);

    fflush(stdout);
    close(fd);
    */


    //this writing is not concise enough
    /*
    //close(2);
    close(1);
    umask(0);
    int fd=open("log.txt",O_WRONLY | O_CREAT | O_TRUNC,0666);
    printf("open: %d\n",fd);

    fflush(stdout);
    close(fd);
    */

    return 0;
}
