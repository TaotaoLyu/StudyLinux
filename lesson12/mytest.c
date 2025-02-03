#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>

int main()
{

    printf("hello world");
    sleep(5);
    _exit(88);
   // exit(66);
    //for(int i=0;i<200;++i)
    //    printf("%d: %s\n",i,strerror(i));

    return 99;
}
