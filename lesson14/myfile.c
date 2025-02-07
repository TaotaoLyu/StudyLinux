#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>


#define FILE_NAME(num) "log.txt"#num


#define ONE (1<<0)
#define TWO (1<<1)
#define THREE (1<<2)
#define FOUR (1<<3)


void show(int flags)
{
    if(flags&ONE)
        printf("one\n");
    if(flags&TWO)
        printf("two\n");
    if(flags&THREE)
        printf("three\n");
    if(flags&FOUR)
        printf("four\n");

}



int main()
{
 
    //printf("stdin: %d\n",stdin->_fileno);
    //printf("stdout: %d\n",stdout->_fileno);
    //printf("stderr: %d\n",stderr->_fileno);



    //int fd1=open(FILE_NAME(1),O_WRONLY|O_CREAT,0666);
    //int fd2=open(FILE_NAME(2),O_WRONLY|O_CREAT,0666);
    //int fd3=open(FILE_NAME(3),O_WRONLY|O_CREAT,0666);
    //int fd4=open(FILE_NAME(4),O_WRONLY|O_CREAT,0666);
    //int fd5=open(FILE_NAME(5),O_WRONLY|O_CREAT,0666);
    //
    //printf("%d\n",fd1);
    //printf("%d\n",fd2);
    //printf("%d\n",fd3);
    //printf("%d\n",fd4);
    //printf("%d\n",fd5);

    //close(fd1);
    //close(fd2);
    //close(fd3);
    //close(fd4);
    //close(fd5);
    
    umask(0);
    int fd=open(FILE_NAME(1),O_WRONLY|O_CREAT,0666);

    close(fd);


    //int fd=open(FILE_NAME,O_WRONLY|O_CREAT|O_TRUNC,0666);//函数内部会 666 & ~umask
    //int fd=open(FILE_NAME,O_WRONLY|O_CREAT|O_APPEND,0666);//函数内部会 666 & ~umask
    //int fd=open(FILE_NAME,O_RDONLY);//函数内部会 666 & ~umask
    //if(fd==-1)
    //{
    //    perror("open");
    //    return 1;
    //}
    //int cnt=5;
    //char buffer[256];
    //ssize_t num=read(fd,buffer,sizeof(buffer)-1);
    //buffer[num]='\0';
    //printf("%s",buffer);
    ////puts(buffer);

    ////while(cnt)
    //{
    //    //write(fd,"aaaa\n",6),cnt--;
    //    //sprintf(buffer,"hello world! cnt: %d\n",cnt--);
    //    //write(fd,buffer,strlen(buffer)+1);//linuc不认识"\0"
    //    //write(fd,buffer,strlen(buffer));

    //}
    //close(fd);
    







    //printf("-------------------------\n");
    //show(ONE);
    //printf("-------------------------\n");
    //show(ONE|TWO);
    //printf("-------------------------\n");
    //show(ONE|TWO|THREE);
    //printf("-------------------------\n");
    //show(ONE|TWO|THREE|FOUR);
    //printf("-------------------------\n");



    /*
    //FILE *fp=fopen(FILE_NAME,"a");// r w r+ w+ a(append) a+
    FILE *fp=fopen(FILE_NAME,"r");// r w r+ w+ a(append) a+
    if(fp==NULL)
    {
        perror("fopen");
        return 1;
    }

    char buffer[64];
    while(fgets(buffer,sizeof buffer -1,fp))
    {
        buffer[strlen(buffer)-1]='\0';
        //printf("%s\n",buffer);
        puts(buffer);
    }

    //int cnt=5;
   // fprintf(fp,"hello world!");
    //while(cnt)
    //{
    //    fprintf(fp,"hello world! cnt: %d\n",cnt);
    //    cnt--;
    //}

    fclose(fp);
    */
    return 0;
}
