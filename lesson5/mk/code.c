#include<stdio.h>
#include<unistd.h>
int main()
{
    int cnt=10;
    while(cnt)
    {
        printf("倒计时:%2d\r",cnt);
        fflush(stdout);
        sleep(1);
        cnt--;
    }
    return 0;
}
