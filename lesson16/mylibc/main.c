#include"myStdio.h"
#include<stdio.h>

int main()
{
    FILE_* fp=fopen_("log.txt","w");
    if(fp==NULL)
    {
        perror("fopen_");
        return 1;
    }
    char buffer[1024]="hello tao\n";
    
    fwrite_(buffer,strlen(buffer),fp);
    sleep(1);
    fclose_(fp);
    return 0;
}
