#include <linux/unistd.h>
#include <sys/syscall.h>
#define __NR_mysyscall	223
int main()
{
    syscall(__NR_mysyscall);
}
