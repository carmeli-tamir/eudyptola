#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <errno.h>

#if __x86_64__
#define EU_SYSCALL 335
#else
#define EU_SYSCALL 387
#endif 

int main(int argc, char *argv[])
{
    long res = syscall( EU_SYSCALL, 0x0102, 0x03040506);
    printf("System call returned %ld with errno %d\n", res, errno);

    res = syscall( EU_SYSCALL, 0x010203, 0x040506);
    printf("System call returned %ld with errno %d\n", res, errno);
  
    return 0;

}
