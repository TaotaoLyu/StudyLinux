#include"comm.hpp"
int main()
{
    key_t key=getkey();
    int shmid=getShm(key);
    char *start=(char*)attachShm(shmid);

    char buffer[1024]="hello server, i am another process that is communicating with you.";
    int num=0;
    while(true)
    {
        sleep(1);
        snprintf(start,MAX_SIZE,"%s[pid: %d][num: %d]",buffer,getpid(),num++);
        //printf("hello\n");
    }



    detachShm(start);
    return 0;
}