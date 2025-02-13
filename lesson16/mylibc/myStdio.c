#include"myStdio.h"


FILE_* fopen_(const char* pathname,const char* mode)
{
    assert(pathname);
    assert(mode);
    int flag=0;
    if(strcmp(mode,"r")==0)
    {
        flag=O_RDONLY;
    }
    else if(strcmp(mode,"w")==0)
    {
        flag=O_WRONLY|O_CREAT|O_TRUNC;
    }
    else if(strcmp(mode,"a")==0)
    {
        flag=O_WRONLY|O_CREAT|O_APPEND;
    }
    else 
    {
        return NULL;
    }
    int fd=0;
    if(flag & O_WRONLY) fd=open(pathname,flag,0666);
    else fd=open(pathname,flag);
    FILE_* fp=(FILE_*)malloc(sizeof(FILE_));
    if(fp==NULL)
        return NULL;
    fp->flag=SYNC_LINE;
    fp->fileno=fd;
    fp->size=0;
    fp->cap=SIZE;
    memset(fp->buffer,0,SIZE);
    return fp;
}
void fwrite_(const char* ptr,int nums,FILE_* fp)
{
    memcpy(fp->buffer+fp->size,ptr,nums);
    fp->size+=nums;
    if(fp->flag==SYNC_FULL)
    {
        if(fp->cap==fp->size)
            fflush_(fp);
    }
    else if(fp->flag==SYNC_LINE)
    {
        if(fp->size!=0&&fp->buffer[fp->size-1]=='\n')
            fflush_(fp);
    }
    else if(fp->flag==SYNC_NOW)
        fflush_(fp);
}
void fflush_(FILE_* fp)
{
    if(fp->size==0) return;
    write(fp->fileno,fp->buffer,fp->size);
    fsync(fp->fileno);  //force flush kernel buffer
    fp->size=0;
}
void fclose_(FILE_* fp)
{
    fflush_(fp);
    close(fp->fileno);
    free(fp);
}












