#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<assert.h>
#include<string.h>
#include<unistd.h>
#include<string.h>


#define NUM 1024
#define OPT_NUM 64
char lineCommand[NUM];
char* argv_[OPT_NUM];
int lastcode=0;
int lastsig=0;



int main()
{
    while(1)
    {
        printf("[ltt@test_bash lesson13]$ ");
        char *s=fgets(lineCommand,sizeof(lineCommand)-1,stdin);
        assert(s!=NULL);
        (void)s;
        lineCommand[strlen(lineCommand)-1]='\0';

        //printf("%s\n",lineCommand);
        argv_[0]=strtok(lineCommand,(const char*)" ");
        if(argv_[0]==NULL) continue;
        int i=1;
        if(strcmp(argv_[0],"ls")==0)
            argv_[i++]=(char*)"--color=auto";
        while(argv_[i++]=(char*)strtok(NULL,(char*)" ")) ; 
        
        if(strcmp(argv_[0],"cd")==0)
        {
            if(argv_[1]!=NULL)
                chdir(argv_[1]);
            continue;
        }
        if(strcmp(argv_[0],"echo")==0)
        {
            if(argv_[1]!=NULL&&strcmp(argv_[1],"$?")==0)
            {
                printf("exit code: %d, sig num: %d\n",lastcode,lastsig);
                continue;
            }   
        }





        #ifdef DEBUG
        for(i=0;argv_[i];++i) 
            printf("%s\n",argv_[i]);
        #endif
        pid_t id=fork();
        assert(id!=-1);
        if(id==0)
        {
            execvp(argv_[0],argv_); 
            exit(1);
        }

        int status=0;
        pid_t ret=waitpid(id,&status,0);
        (void)ret;
        assert(ret>0);
        //printf("sig num: %d,exit code: %d \n",status&0x7F,(status>>8)&0xFF);
        //printf("sig num: %d,exit code: %d \n",WIFEXITED(status),WEXITSTATUS(status));
        lastcode=(status>>8)&0xFF;
        lastsig=status&0x7F;


        if(!WIFEXITED(status)||WEXITSTATUS(status)==1)
            printf("command fail\n");




    }
    return 0;
}
