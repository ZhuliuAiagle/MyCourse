#include <stdio.h>
#include <string.h>
#include <linux/unistd.h>
#include <sys/syscall.h>
#define __NR_mysyscall 223


int main(){
	int ch;
	int output=0;
	FILE *fp;
	//打开日志文件
	syscall(__NR_mysyscall);
	fp=fopen("/var/log/kern.log","r");
	if(fp==NULL)
        {//若打不开则输出错误信息
		printf("No Permission!\n");
		return 0;
	}
	
	//读取日志文件
	fseek(fp,0,SEEK_SET);
        //文件指针重定位到文件头 
    unsigned long offset = 0;
	ch=fgetc(fp);
	//找到内核模块输出记录开头
    	FILE *begin = NULL;
    while(ch!=EOF)
    {
        while(ch!=EOF)
        {
            //输出记录开头的特殊标记为"#$#" 
            if(ch=='#')
            {
                ch=fgetc(fp);
                if(ch=='$')
                {
                    ch=fgetc(fp);
                    if(ch=='#')
                    {
                    
                        printf("HEAD\n");
                        break;
                    }
                }
            }
            ch=fgetc(fp);
        }
        //打印出内核模块的输出记录, @是用于识别每一行的特殊标记
        while(ch!=EOF)
        {
            if(ch == '@')
            {
                output = 1;
            }
            else if(ch == '\n')
            {
                printf("\n");
                output = 0;
            }
            else if(ch =='$')
            {//输出记录结尾的特殊标记为"$#$" 
            ch=fgetc(fp);
            if(ch=='#')
                        {
                ch=fgetc(fp);
                if(ch=='$')
                {
                    printf("END\n");
                    ch=fgetc(fp);
                    break;
                }
            }
            }if(output == 1 && ch != '@')
            {
                printf("%c",ch);
            }
            ch=fgetc(fp);
        }
    }
	//关闭日志文件
	fclose(fp); 
	return 0;
}


extern unsigned long pfcount;
asmlinkage int sys_mysyscall(void)
{
    printk("#$#\n");
    printk("@all page fault: %lu\n", pfcount);
    printk("@current page fault: %lu\n",current->pf);
    struct task_struct *p = &init_task;
    for(;(p=next_task(p))!=&init_task;)
    {
        printk("@The dirty page of process %d: %d\n",p->pid,p->nr_dirtied);
    }
    rintk("$#$\n");
    return 0;
}