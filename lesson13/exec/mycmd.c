#include<stdio.h>
#include<stdlib.h>

int main(int argc,char* argv[])
{
    /*if(argv[0]==NULL)
        printf("第一个参数为空\n");
    else 
        printf("argv[0]: %s\n",argv[0]);
    */
    printf("HOME: %s\n",getenv("HOME"));
    printf("PWD: %s\n",getenv("PWD"));
    printf("MYENV: %s\n",getenv("MYENV"));
    printf("我是一个c程序\n");
    printf("我是一个c程序\n");
    printf("我是一个c程序\n");
    printf("我是一个c程序\n");
    printf("我是一个c程序\n");
    printf("我是一个c程序\n");
    printf("我是一个c程序\n");
    printf("我是一个c程序\n");
    return 0;
}
