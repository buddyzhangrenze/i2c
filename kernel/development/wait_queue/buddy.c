#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/sched.h>
#include <asm/current.h>
#include <linux/wait.h>
#include <linux/types.h>
#include <linux/unistd.h>
#include <linux/delay.h>

#define DEV_NAME "buddy_test"

/*
 * data segment
 */
static wait_queue_head_t my_queue;
static int buddy_state;
/*
 * th0
 */
int bth0()
{
	/*
	 * Declare and init a wait queue.
	 */
	DECLARE_WAITQUEUE(wait,current);
	/*
	 * Add this item into wait queue.
	 */
	add_wait_queue(&my_queue,&wait);
	/*
	 * start sleep.
	 */
	if(buddy_state < 0)
	{
		do{
			/*
			 * Give CPU.
			 */
			set_current_state(TASK_INTERRUPTIBLE);
			/*
			 * select a new task.
			 */
			printk(KERN_INFO "Start schedule.\n");
			schedule();
			/*
			 * End of schedule.
			 */
			if(signal_pending(current))
				printk(KERN_INFO "This task need signal wake up!\n");
			printk(KERN_INFO "End of scheduld.\n");
		} while(buddy_state < 0);
	}
	/*
	 * Stop sleep.
	 */
	remove_wait_queue(&my_queue,&wait);
	/*
	 * Wait run
	 */
	set_current_state(TASK_RUNNING);
    printk(KERN_INFO "Finish all\n");
	return 0;
}
/*
 * th1
 */
int bth1()
{
	/*
	 * wake up wait queue.
	 */
	wake_up(&my_queue);
	mdelay(1000);
	buddy_state = 39;
	mdelay(1000);
	buddy_state = 50;
	mdelay(1000);
	wake_up(&my_queue);
	return 0;
}
/*
 * th2
 */
int bth2()
{
	/*
	 * Dclare a wait and init it.
	 */
	DEFINE_WAIT(wait);
	if(buddy_state < 40)
	{
		do{
			/*
			 * Add it into wait queue and set current task as TASK_INTERRUPTIBLE.
			 */
			prepare_to_wait(&my_queue,&wait,TASK_INTERRUPTIBLE);
			/*
			 * schedule
			 */
			printk(KERN_INFO "Start new schedule.\n");
			schedule();
			printk(KERN_INFO "Stop new schedule.\n");
		}while(buddy_state < 40);
	}
	/*
	 * finish wait
	 */
	finish_wait(&my_queue,&wait);
	printk(KERN_INFO "Finish new.\n");
	return 0;
}
/*
 * open operation
 */
static int buddy_open(struct inode *inode,struct file *filp)
{
	/*
	 * init wait queue head
	 */
	init_waitqueue_head(&my_queue);
	printk(KERN_INFO "Open device\n");
	return 0;
}
/*
 * release operation
 */
static ssize_t buddy_read(struct file *filp,char __user *buf,size_t count,loff_t *offset)
{
	buddy_state = -1;
	/*
	 * Virture wait event.
	 */
//	kernel_thread(bth0,NULL,CLONE_FS | CLONE_FILES | CLONE_SIGHAND | SIGCHLD);
//	kernel_thread(bth2,NULL,CLONE_FS | CLONE_FILES | CLONE_SIGHAND | SIGCHLD);
	/*
	 * wake up the wait queue
	 */
	kernel_thread(bth1,NULL,CLONE_FS | CLONE_FILES | CLONE_SIGHAND | SIGCHLD);
	
	/*
	 * Wait event is true.
	 */
	printk(KERN_INFO "Start wait event\n");
	wait_event(my_queue,NULL);
	printk(KERN_INFO "Finish all\n");
	return 0;
}
/*
 * file_operations
 */
static struct file_operations buddy_fops = {
	.owner     = THIS_MODULE,
	.open      = buddy_open,
	.read      = buddy_read,
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
