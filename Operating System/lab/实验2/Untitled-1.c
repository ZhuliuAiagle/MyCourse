extern unsigned long pfcount;
asmlinkage int sys_mysyscall(void)
{
    struct task_struct *p = &init_task;
	printk("## The overall page fault: %lu\n", pfcount);
    printk("## The page fault of current process:  %lu\n", current->pf);
    while((p = next_task(p)) != &init_task)
    {
        printk("## Dirtied pages of process %d: %d\n", p->pid, p->nr_dirtied);
    }
    return 0;
}