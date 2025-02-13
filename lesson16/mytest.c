#include<stdio.h>
#include<unistd.h>
#include<string.h>

int main()
{

    printf("hello printf\n");
    fprintf(stdout,"hello fprintf\n");
    fputs("hello fputs\n",stdout);

    char buffer[1024]="hello write\n";
    write(stdout->_fileno,buffer,strlen(buffer));

    fork();
    return 0;
}
