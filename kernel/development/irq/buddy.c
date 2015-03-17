/*
 * This undering driver uses a GPIO as source of irq.
 * When we push key,it will generate a irq signal.The CPU will deal with this
 * irq.
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>

#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/irq.h>

#define DEV_NAME "buddy_test"
static struct work_struct buddy_wq;
static int sum = 0;
/*
 * tasklet handle
 */
static void buddy_tasklet_handle(unsigned long par)
{
	printk(KERN_INFO "This is tasklet\n");
}
static DECLARE_TASKLET(buddy_tasklet,buddy_tasklet_handle,0);
/*
 * work_queue
 */
void buddy_do_work(unsigned long arg)
{
	printk(KERN_INFO "This is work queue\n");
}
/*
 * IRQ handle -- tasklet
 */
static irqreturn_t buddy_handle_tasklet(int irq,void *dev_id)
{
	printk(KERN_INFO "The irq number[%3d]\n",irq);
	printk(KERN_INFO "This is buddy handler,sum[%d]\n",sum);
	sum++;
	/*
	 * Call tasklet
	 */
	tasklet_schedule(&buddy_tasklet);
	/*
	 * Get desc for irq.
	 */
	return IRQ_HANDLED;
}
/*
 * IRQ handle -- work_queue
 */
static irqreturn_t buddy_handle_wq(int irq,void *dev_id)
{
	printk(KERN_INFO "This irq number[%3d]\n",irq);
	printk(KERN_INFO "This is buddy handler,sum[%d]\n",sum);
	sum++;
	/*
	 * Call wq
	 */
	schedule_work(&buddy_wq);
	return IRQ_HANDLED;
}
/*
 * open operation
 */
static int buddy_open(struct inode *inode,struct file *filp)
{
	int ret;
	/*
	 * init wq
	 */
	INIT_WORK(&buddy_wq,(void (*)(void *))buddy_do_work);
	printk(KERN_INFO "Open device\n");
	/*
	 * request a irq
	 * Flags of generate way:
	 *		IRQF_TRIGGER_FALLING.
	 *		IRQF_TRIGGER_RISING.
	 *		IRQF_TRIGGER_HIGH.
	 *		IRQF_TRIGGER_LOW.
	 * Flags of deal way:
	 *      IRQF_DISABLED.
	 *      IRQF_SHARED.
	 */
	ret = request_irq(IRQ_EINT(0),buddy_handle_tasklet,IRQF_TRIGGER_FALLING,
			"buddy_irq",NULL);
	ret = request_irq(IRQ_EINT(1),buddy_handle_tasklet,IRQF_TRIGGER_RISING,
			"buddy_irq1",NULL);
	/*
	 * Disable irq
	 */
	disable_irq(IRQ_EINT(1));
	return 0;
}
/*
 * release opertion 
 */
static int buddy_release(struct inode *inode,struct file *filp)
{
	printk(KERN_INFO "Close devuce\n");
	/*
	 * free irq
	 */
	free_irq(IRQ_EINT(0),NULL);
	free_irq(IRQ_EINT(1),NULL);
	return 0;
}
/*
 * read operation
 */
static ssize_t buddy_read(struct file *filp,char __user *buffer,size_t count,
		loff_t *offset)
{
	printk(KERN_INFO "read device\n");
	/*
	 * Do nothing wait irq.
	 */
	while(1)
	{
		printk(KERN_INFO "...[%3d]\n",sum);
		if(sum == 5)
		{
			printk(KERN_INFO "The sum is [%d]\n",sum);
			/*
			 * Enable an irq.
			 */
			enable_irq(IRQ_EINT(1));
		}
		mdelay(1000);
	}
	return 0;
}
/*
 * write operation
 */
static ssize_t buddy_write(struct file *filp,const char __user *buf,
		size_t count,loff_t *offset)
{
	printk(KERN_INFO "Write device\n");
	return 0;
}
/*
 * file_operations
 */
static struct file_operations buddy_fops = {
	.owner     = THIS_MODULE,
	.open      = buddy_open,
	.release   = buddy_release,
	.write     = buddy_write,
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
