#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<assert.h>
#include<sys/types.h>
#include<sys/wait.h>


int main(int argc,char* argv[])
{
    printf("process is running...\n");
    

    pid_t id=fork();
    assert(id!=-1);

    if(id==0)
    {
        execl("./mypy.py","mypy.py",NULL);
        //execl("./mybash.sh","mybash.sh",NULL);
       // execvp(argv[1],argv+1);

        //execlp("ls","ls","-a","-l","--color=auto",NULL);
        exit(1);//替换失败
    }
    int status=0;
    pid_t ret=waitpid(id,&status,0);
    (void)ret;
    if(ret>0)
        printf("wait success, sig num: %d, exit code: %d\n",status&0x7F,(status>>8)&0xFF);
    




    /*char* const argv_[5]={
       (char*)"ls",
        (char*)"-a",
        (char*)"-l",
        (char*)"--color=auto",
        NULL 
    };
    char* env_[]={
        "MYENV=6699"
    };
    putenv("MYENV=6699");
    extern char** environ;
    execvpe("./mycmd",argv_,environ);
    //execv("/usr/bin/ls",argv_);


    //execl("./mycmd",NULL);


    //execlp("ls","ls","-al","--color=auto",NULL);
    */
    printf("process running done...\n");
    return 0;
}
