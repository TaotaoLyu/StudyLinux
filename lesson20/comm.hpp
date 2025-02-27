#ifndef _COMM_H_
#define _COMM_H_

#include<iostream>
#include<cstring>
#include<unistd.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>

#define MAX_SIZE 4096
#define PATHNAME "."
#define PROJ_ID 0x66


key_t getkey()
{
    key_t key=ftok(PATHNAME,PROJ_ID);
    if(key==-1)
    {
        std::cerr<<"function getkey error. errno : "<<errno<<" "<<
        strerror(errno)<<std::endl;
        exit(1);
    }
    return key;
}




int getShmHelper(key_t key,int flags)
{
    int shmid=shmget(key,MAX_SIZE,flags);
    if(shmid==-1)
    {
        std::cerr<<"function getShmHelper error. errno : "<<errno<<" "<<
        strerror(errno)<<std::endl;
        exit(2);
    }
    return shmid;
}




int getShm(key_t key)
{
    return getShmHelper(key,IPC_CREAT);
}


int createShm(key_t key)
{
    return getShmHelper(key,IPC_CREAT|IPC_EXCL|0600);
}

void *attachShm(int shmid)
{
    void *add=shmat(shmid,nullptr,0);
    if((long long)add==-1LL)
    {
        std::cerr<<"function attachShm error. errno : "<<errno<<" "<<
        strerror(errno)<<std::endl;
        exit(3);
    }
    return add;
}

void detachShm(const void* shmaddr)
{
    if(shmdt(shmaddr)==-1)
    {
        std::cerr<<"function delShm error. errno : "<<errno<<" "<<
        strerror(errno)<<std::endl;
    }
}


void delShm(int shmid)
{
    if(shmctl(shmid,IPC_RMID,nullptr)==-1)
    {
        std::cerr<<"function delShm error. errno : "<<errno<<" "<<
        strerror(errno)<<std::endl;
    }
}

#endif