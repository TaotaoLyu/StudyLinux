#include<stdio.h>
#include<time.h>


int AddToVal(int begin,int end)
{
    int sum=0;
    for(int i=begin;i<=end;++i)
    {
        sum+=i;
    }
    return sum;
}

void Print(int val)
{
    time_t t=time(NULL);
    printf("result=%d timestamp=%d\n",val,t);
}
int main()
{
    int sum = AddToVal(0,100);
    Print(sum);
    printf("hello %d\n",1);
    printf("hello %d\n",2);
    printf("hello %d\n",3);
    printf("hello %d\n",4);
    printf("hello %d\n",5);
    printf("hello %d\n",6);
    printf("hello %d\n",7);
    printf("hello %d\n",8);
    printf("hello %d\n",9);
    printf("hello %d\n",10);
    return 0;
}
