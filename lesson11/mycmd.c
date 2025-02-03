#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#define MY_VAL "myval"

int main(int argc,char* argv[])
{
    int i=0;
    for(;argv[i];++i)
        printf("%s\n",argv[i]);
    /*
    extern char** environ;
    int i=0;
    for(;environ[i];++i)
        printf("env[%d]:%s\n",i,environ[i]);
    */

    /*
    if(argc==1)
    {
        printf("Usage:\n\t./mycmd [-a/-b/-ab]\n");
    }
    else if(argc==2)
    {
        if(strcmp(argv[1],"-a")==0)
            printf("a功能\n");
        else if(strcmp(argv[1],"-b")==0)
            printf("b功能\n");
        else if(strcmp(argv[1],"-ab")==0)
            printf("ab功能\n");
        else 
            printf("error\n");

    }
    else 
        printf("error\n");
    */

    /*char *usr=getenv("USER");
    if(strcmp(usr,"root")!=0)
    {
        printf("%s 没有权限\n",usr);
    }
    else 
        printf("运行进入\n");
    */


   // char *pwd=getenv("PWD");
   // printf("%s\n",pwd);


   // char *myval=getenv(MY_VAL);
   // printf("%s:%s\n",MY_VAL,myval);

    return 0;
}
