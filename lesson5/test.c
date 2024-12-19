#include<stdio.h>

#define M 1234

int main()
{
    printf("hello world\n");
    //printf("hello");

#ifdef SHOW 
    printf("hello show");
#else
    printf("%d",M);
#endif 

    return 0;
}
