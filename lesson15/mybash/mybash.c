#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<assert.h>
#include<string.h>
#include<unistd.h>
#include<string.h>
#include<ctype.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>


#define trimSpace(start) do{\
    while(isspace(*start)) start++;\
    }while(0)

#define NO_REDIR 0
#define OUTPUT_REDIR 1
#define APPEND_REDIR 2
#define INPUT_REDIR 3

#define NUM 1024
#define OPT_NUM 64



char lineCommand[NUM];
char* argv_[OPT_NUM];
int lastcode=0;
int lastsig=0;

int redirType=NO_REDIR;
const char* redirFile=NULL; 






void commandcheck(char* command)
{
    assert(command);
    //ls -a -l > log.txt
    char* start=command;
    char* end=command+strlen(command);
    while(start<end)
    {
        if(*start=='>')
        {
            *start='\0';
            start++;
            redirType=OUTPUT_REDIR;
            if(*start=='>')
            {
                *(start)='\0';
                start++;
                redirType=APPEND_REDIR;
            }
            trimSpace(start);
            redirFile=start;
            break;
        }
        else if(*start=='<')
        {
            //cat < log.txt;
            *start='\0';
            redirType=INPUT_REDIR;
            start++;
            trimSpace(start);
            redirFile=start;
            break;
        }
        else 
            start++;
    }
}




int main()
{
    while(1)
    {
        redirType=NO_REDIR;
        redirFile=NULL;
        printf("[ltt@test_bash lesson13]$ ");
        char *s=fgets(lineCommand,sizeof(lineCommand)-1,stdin);
        assert(s!=NULL);
        (void)s;
        lineCommand[strlen(lineCommand)-1]='\0';
        commandcheck(lineCommand);

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
            switch (redirType)
            {
                case INPUT_REDIR:
                    {
                        int fd=open(redirFile,O_RDONLY);
                        if(fd<0)//hangdling error conditions
                        {
                            perror("open");
                            exit(errno);// return error code
                        }
                        dup2(fd,0);
                    }
                    break;
                case OUTPUT_REDIR:
                case APPEND_REDIR:
                    {
                        int flags=O_WRONLY|O_CREAT;
                        if(redirType==OUTPUT_REDIR)
                            flags=flags|O_TRUNC;
                        else 
                            flags|=O_APPEND;
                        int fd=open(redirFile,flags,0666);
                        if(fd<0)
                        {
                            perror("open");
                            exit(errno);
                        }
                        dup2(fd,1);
                    }
                    break;
                case NO_REDIR:
                    //do nothing
                    break;
                default://it can't be this value under normal circumstances
                    printf("bug!!!\n");
                    break;
            }



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
