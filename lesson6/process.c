#include"process.h"


char styles[]={'.','*','-','#','+','>'};

void Process()
{
    char buffer[NUM];
    char label[4]={'|','/','-','\\'};
    int cnt=0;
    memset(buffer,'\0',sizeof(buffer));
    printf("\033[0m");
    printf("\033[1;31m");
    while(cnt<=100)
    {
        printf("[%-100s][%3d%%][%c]\r",buffer,cnt,label[cnt%4]);
        fflush(stdout);
        buffer[cnt++]=styles[N];
        //buffer[cnt++]=STYLE;
        usleep(50000);
    }
    printf("\033[0m");
    printf("\n");
}
