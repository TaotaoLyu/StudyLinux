#include"my_add.h"
#include"my_sub.h"

int main()
{
    int x=10;
    int y=20;
    int result=Sub(x,y);
    printf("result: %d\n",result);
    result=Add(x,y);
    printf("result: %d\n",result);
    return 0;
}
