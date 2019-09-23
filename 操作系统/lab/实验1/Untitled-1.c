#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/init_task.h>
// 初始化函数
int count = 0;
static int count[50];
int init_module(void)
{
    struct task_struct *p;
    p = NULL;
    p = &init_task;
    printk("名称\t进程号\t状态\t优先级\t父进程号\t");
    for_each_process(p)
    {
        if(p->mm == NULL){ //内核线程的mm成员为空
            printk(KERN_EMERG"%s\t%d\t%ld\t%d\t%d\n",p->comm,p->pid, p->state,p->normal_prio,p->parent->pid);
	    count++;
        if(p->state == 0x0000) count[0]++;
        else if(p->state == 0x0001) count[1]++;
	    else if(p->state == 0x0002) count[2]++;
	    else if(p->state == 0x0004) count[3]++;
	    else if(p->state == 0x0008) count[4]++;
        else if(p->state == 0x0010) count[5]++;
        else if(p->state == 0x0020) count[6]++;
        else if(p->state == 0x0020 | 0x0010) count[7]++;
        else if(p->state == 0x0040) count[8]++;
        else if(p->state == 0x0080) count[9]++;
        else if(p->state == 0x0100) count[10]++;
        else if(p->state == 0x0200) count[11]++;
        else if(p->state == 0x0400) count[12]++;
        else if(p->state == 0x0800) count[13]++;
        else if(p->state == 0x1000) count[14]++;
        else if(p->state == 0x0100 | 0x0002) count[15]++;
        else if(p->state == 0x0100 | 0x0004) count[16]++;
        else if(p->state == 0x0100 | 0x0008) count[17]++;
        else if(p->state == 0x0002 | 0x0400) count[18]++;
        else if(p->state == 0x0001 | 0x0002) count[19]++;
        }
    }
    printk("TOTAL_NUMBER:\t\t%d\n",count);
    printk("TASK_RUNNING:\t\t%d\n",count[0]);
    printk("TASK_INTERRUPTIBLE:\t%d\n",count[1]);
    printk("TASK_UNINTER:\t\t%d\n",count[2]);
    printk("TASK_STOPPED:\t\t%d\n",count[3]);
    printk("TASK_TRACED:\t\t%d\n",count[4]);
    printk("EXIT_DEAD:\t\t%d\n",count[5]);
    printk("EXIT_ZOMBIE:\t\t%d\n",count[6]);
    printk("EXIT_TRACE:\t\t%d\n",count[7]);
    printk("TASK_PARKED:\t\t%d\n",count[8]);
    printk("TASK_DEAD:\t\t%d\n",count[9]);
    printk("TASK_WAKEKILL:\t\t%d\n",count[10]);
    printk("TASK_WAKING:\t\t%d\n",count[11]);
    printk("TASK_NOLOAD:\t\t%d\n",count[12]);
    printk("TASK_NEW:\t\t%d\n",count[13]);
    printk("TASK_STATE_MAX:\t\t%d\n",count[14]);
    printk("TASK_KILLABLE:\t\t%d\n",count[15]);
    printk("TASK_STOPPED:\t\t%d\n",count[16]);
    printk("TASK_TRACED:\t\t%d\n",count[17]);
    printk("TASK_IDLE:\t\t%d\n",count[18]);
    printk("TASK_NORMAL:\t\t%d\n",count[19]);
    
    return 0;
}
// 清理函数
void cleanup_module(void)
{
    printk("goodbye!\n");
}
// 模块许可申明
MODULE_LICENSE("GPL");