#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/sched.h>
#include <asm/current.h>
#include <linux/delay.h>
#include <linux/hardirq.h>

#define DEV_NAME "buddy_test"

#define NICE_TO_PRIO(nice) (MAX_RT_PRIO + (nice) + 20)
#define PRIO_TO_NICE(prio) ((prio) - MAX_RT_PRIO - 20)

struct task_struct *task;
/*
 * get current task
 */
struct task_struct *buddy_get_current(void)
{
	return current;
}
/*
 * get task state
 */
void buddy_task_state(struct task_struct *ts)
{
	printk(KERN_INFO "The task state list:\n \
			TASK_RUNNONG : 0\n \
			TASK_INTERRUPTIBLE : 1\n \
			TASK_UNINTERRUPTIBLE : 2\n \
			__TASK_STOPPED : 4\n \
			__TASK_TRACED  : 8\n \
			EXIT_ZOMBIE  : 16\n \
			EXIT_DEAD  : 32\n \
			TASK_DEAD  : 64\n \
			TASK_WAKEKILL : 128\n \
			TASK_WAKING : 256\n \
			TASK_STATE_MAX  : 512\n");
	printk(KERN_INFO "The current state is %ld\n",ts->state);
}
/*
 * get prio
 */
void buddy_task_prio(struct task_struct *ts)
{
	int nice;
	printk(KERN_INFO "The current prio[%d],static_prio[%d],normal_prio[%d]\n",
			ts->prio,ts->static_prio,ts->normal_prio);
	printk(KERN_INFO "The current real timing priority[%d]\n",ts->rt_priority);
	nice = PRIO_TO_NICE(ts->prio);
	printk(KERN_INFO "The current nice is[%d]\n",nice);
	printk(KERN_INFO "The current prio is[%d]\n",NICE_TO_PRIO(nice));

}
/*
 * Get pid
 */
void buddy_task_pid(struct task_struct *ts)
{
	preempt_disable();
	printk(KERN_INFO "The pid of current is[%d]\n",ts->pid);
	printk(KERN_INFO "The tgid of current is[%d]\n",ts->tgid);
	preempt_enable();
}
/*
 * Get current flags
 */
void buddy_task_flags(struct task_struct *ts)
{
	cpumask_t *tmp = &ts->cpus_allowed;
	printk(KERN_INFO "The flags of current is [%u]\n",ts->flags);
	printk(KERN_INFO "The policy is[%u]\n",ts->policy);
	printk(KERN_INFO "The cups_allowed is [%ld]\n",ts->cpus_allowed);
	/*
	 * check task whether is in interrupt
	 */
	if(in_interrupt())
		printk("The task in interrupt and can't sleep\n");
	else
		printk("The task not in interrupt and can sleep\n");
	/*
	 * check task is init task
	 */
	if(ts->pid == 1)
		printk(KERN_INFO "The task is init\n");
	else 
		printk(KERN_INFO "The task is not init\n");
	/*
	 * check task is 0 task
	 */
	if(ts->pid == 0)
		printk(KERN_INFO "The task is 0\n");
	else
		printk(KERN_INFO "The task isn't 0 task\n");
}
/*
 * Get exit information
 */
void buddy_exit_info(struct task_struct *ts)
{
	/*
	 * return to father process.
	 */
	printk(KERN_INFO "The exit state is[%d]\n \
			The exit code is[%d]\n \
			The exit signal is[%d]\n",ts->exit_state,ts->exit_code,
			ts->exit_signal);
	/*
	 * Father information.
	 */
	printk(KERN_INFO "The pdeath_signal is[%d]\n",ts->pdeath_signal);
}
/*
 * Comm string
 */
void buddy_comm_name(struct task_struct *ts)
{
	printk(KERN_INFO "The comm name is[%s]\n",ts->comm);
}
/*
 * Sched information.
 */
void buddy_sched(struct task_struct *ts)
{
	printk(KERN_INFO "The slice time is[%u]\n",ts->rt.time_slice);
	printk(KERN_INFO "The timeout is[%lu]\n",ts->rt.timeout);
	printk(KERN_INFO "The nivcsw is[%lu]\n",ts->nivcsw);
	printk(KERN_INFO "The nvcsw is[%lu]\n",ts->nvcsw);
}
/*
 * parent
 */
void buddy_parent(struct task_struct *ts)
{
	struct task_struct *parent = ts->parent;
	struct task_struct *real_parent = ts->real_parent;
	printk(KERN_INFO "Parent is [%s]\n",parent->comm);
	printk(KERN_INFO "The real parent is [%s]\n",real_parent->comm);
	printk(KERN_INFO "The group leader is [%s]\n",ts->group_leader->comm);
}
/*
 * open operation
 */
static int buddy_open(struct inode *inode,struct file *filp)
{
	printk(KERN_INFO "Open device\n");
	task = buddy_get_current();
	filp->private_data = task;
	return 0;
}
/*
 * read operation
 */
static ssize_t buddy_read(struct file *filp,char __user *buf,size_t count,
		loff_t *offset)
{
	struct task_struct *task;
	printk(KERN_INFO "Read device\n");
	task = filp->private_data;
	buddy_task_state(task);
	ndelay(5000);
	buddy_task_state(task);
	buddy_task_prio(task);
	buddy_task_pid(task);
	buddy_task_flags(task);
	buddy_comm_name(task);
	buddy_sched(task);
	buddy_parent(task);
	return 0;
}
/*
 * write operation
 */
static ssize_t buddy_write(struct file *filp,const char __user *buf,size_t count,loff_t *offset)
{
	printk(KERN_INFO "Write device\n");
	return 0;
}
static int buddy_release(struct inode *inode,struct file *filp)
{
	
	struct task_struct *task = filp->private_data;
	printk(KERN_INFO "Release\n");
	buddy_exit_info(task);
	return 0;
}
/*
 * file_operations
 */
static struct file_operations buddy_fops = {
	.owner     = THIS_MODULE,
	.read      = buddy_read,
	.write     = buddy_write,
	.open      = buddy_open,
	.release   = buddy_release,
};
/*
 * misc struct 
 */

static struct miscdevice buddy_misc = {
	.minor    = MISC_DYNAMIC_MINOR,
	.name     = DEV_NAME,
	.fops     = &buddy_fops,
};
/*
 * Init module
 */
static __init int buddy_init(void)
{
	misc_register(&buddy_misc);
	printk("buddy_test\n");
	return 0;
}
/*
 * Exit module
 */
static __exit void buddy_exit(void)
{
	misc_deregister(&buddy_misc);
}
/*
 * module information
 */
module_init(buddy_init);
module_exit(buddy_exit);

MODULE_LICENSE("GPL");
