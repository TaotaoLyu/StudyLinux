#include"comm.hpp"

int main()
{
    key_t key=getkey();
    int shmid=createShm(key);
    printf("key: 0x%x ,shmid : %d\n",key,shmid);

    char *start=(char*)attachShm(shmid);
    printf("address : %p\n",start);

    shmid_ds ds;
    shmctl(shmid,IPC_STAT,&ds);
    printf("size: %d, pid: %d myself: %d key: 0x%x\n", ds.shm_segsz, ds.shm_cpid, getpid(), ds.shm_perm.__key);


    while(true)
    {
        //printf("hello");
        printf("client say: %s\n",start);

        sleep(1);
    }


    detachShm(start);
    delShm(shmid);
    //printf("delShm sucess!!\n");
    return 0;
}