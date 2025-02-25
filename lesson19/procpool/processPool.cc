#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <functional>
#include <unistd.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define PROCESS_NUM 10
#define MAKESEED() srand(time(nullptr) ^ 1798923 ^ rand()^ getpid())

typedef void (*func)();

/////task///
void downLoadTask()
{
    std::cout << getpid() << " : running DownLoad task....\n";
    sleep(1);
}
void ioTask()
{
    std::cout << getpid() << " : running IO task....\n";
    sleep(1);
}
void flushTask()
{
    std::cout << getpid() << " : running flush task....\n";
    sleep(1);
}

void loadTask(std::vector<std::function<void()>> *taskMap)
{
    assert(taskMap);
    taskMap->push_back(downLoadTask);
    taskMap->push_back(ioTask);
    taskMap->push_back(flushTask);
}

class subEp // sub end point 一端
{
public:
    subEp(pid_t id, int rfd)
        : subId_(id), writeFd_(rfd)
    {
        char buffer[1024];
        snprintf(buffer, sizeof buffer, "process-%d[pid(%d)-fd(%d)]", num++, subId_, writeFd_);
        name_ = buffer;
    }

public:
    static int num;
    std::string name_;
    pid_t subId_;
    int writeFd_;
};
int subEp::num = 0;

int recevTask(int readfd)
{
    int code = 0;
    ssize_t n = read(readfd, &code, sizeof(int));
    if (n <= 0)
        return -1;
    else if (n == sizeof(int))
        return code;
    else
        return -1;
}
void sendTask(const subEp &process, int taskNum)
{
    std::cout << "send task num: " << taskNum << " send to -> " << process.name_ << std::endl;
    ssize_t n = write(process.writeFd_, &taskNum, sizeof(taskNum));
    assert(n == sizeof(int));
    (void)n;
}

void createprocess(std::vector<subEp> *subs, const std::vector<std::function<void()>> &taskMap)
{
    std::vector<int> deletefd;
    for (int i = 0; i < PROCESS_NUM; ++i)
    {
        int fds[2];
        int ret = pipe(fds);
        assert(ret == 0);
        (void)ret;
        pid_t id = fork();
        if (id == 0)
        {
            for (auto fd : deletefd)
                close(fd); // close the write end of other pipes
            close(fds[1]);
            // child process , process task
            while (true)
            {
                int commandcode = recevTask(fds[0]);
                if (commandcode >= 0 && commandcode < taskMap.size())
                    taskMap[commandcode]();
                else if (commandcode == -1)
                    break;
            }
            exit(0);
        }
        close(fds[0]);
        subEp sub(id, fds[1]);
        deletefd.push_back(fds[1]);
        subs->push_back(std::move(sub));
    }
}

void loadBalanceControl(std::vector<subEp> &subs,
                        std::vector<std::function<void()>> &taskMap, int taskCnt)
{
    int processnum = subs.size();
    int tasknum = taskMap.size();
    bool forever = taskCnt == 0 ? true : false;

    while (true)
    {
        int subIdx = rand() % processnum;
        int taskIdx = rand() % tasknum;
        sendTask(subs[subIdx], taskIdx);
        sleep(1);
        if (!forever && taskCnt-- == 0)
            break;
    }
    // for(int i=0;i<processnum;++i) close(subs[i].writeFd_);
}

void waitProcess(std::vector<subEp> &subs)
{
    int processnum = subs.size();
    for (int i = 0; i < processnum; ++i)
    {
        close(subs[i].writeFd_);
        int ret = waitpid(subs[i].subId_, nullptr, 0);
        assert(ret == subs[i].subId_);
        std::cout << "wait sub process success .... " << subs[i].name_ << std::endl;
    }
}

int main()
{
    MAKESEED();
    std::vector<subEp> subs; // sub processes
    std::vector<std::function<void()>> taskMap;
    // load task table
    loadTask(&taskMap);

    // create process
    createprocess(&subs, taskMap);

    // loadBalanceControl send message
    int taskCnt = 3;
    loadBalanceControl(subs, taskMap, taskCnt);

    // recycle resources
    waitProcess(subs);
    return 0;
}