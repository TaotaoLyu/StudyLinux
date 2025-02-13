#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<assert.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>

#define SIZE 1024
#define SYNC_NOW 1
#define SYNC_LINE 2
#define SYNC_FULL 4



typedef struct FILE_
{
    int flag;   //refresh policy
    int fileno; //file descriptor
    int size;   //buffer effective size
    int cap;    //buffer capacity
    char buffer[SIZE];
}FILE_;

FILE_* fopen_(const char* pathname,const char* mode);
void fwrite_(const char* ptr,int nums,FILE_* fp);
void fflush_(FILE_* fp);
void fclose_(FILE_* fp);

