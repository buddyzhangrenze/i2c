/*
 * Copyright @2014 Buddy Zhangrenze
 */
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <linux/jiffies.h>
#include <linux/uaccess.h>
#include <linux/string.h>

#define DEV_NAME "at24c02"

static struct class *i2c_dev_class;
static struct i2c_client *my_client;
static int i2c_major;

/*
 * open - file operations
 */
static int buddy_open(struct inode *inode,struct file *filp)
{
	printk(KERN_INFO "Buddy open file\n");
	filp->private_data = my_client;
	return 0;
}
/*
 * close - file operations
 */
static int buddy_release(struct inode *inode,struct file *filp)
{
	printk(KERN_INFO "close file\n");
	return 0;
}
/*
 * struct for file_operations
 */
static struct file_operations buddy_ops = 
{
	.owner    = THIS_MODULE,
	.open     = buddy_open,  
	.release  = buddy_release,
};

/*
 * init module
 */
static __init  int buddy_init(void)
{
	int ret = 0;

	/*
	 * register chardev
	 */
	i2c_major = register_chrdev(0,DEV_NAME,&buddy_ops);
	if(i2c_major == 0)
	{
		printk(KERN_INFO "Unable register chrdev into kernel\n");
		return -EFAULT;
	}
	printk(KERN_INFO "Succeed to register chrdev into kernel\n");
	/*
	 * create class
	 */
	i2c_dev_class = class_create(THIS_MODULE,DEV_NAME);
	if(i2c_dev_class == NULL)
	{
		printk(KERN_INFO "Unable to create class\n");
		return -EFAULT;
	}
	printk(KERN_INFO "Succeed to create class\n");
	return 0;
}
/*
 * exit module
 */
static __init void buddy_exit(void)
{
	class_destroy(i2c_dev_class);
	unregister_chrdev(i2c_major,DEV_NAME);
}
module_init(buddy_init);
module_exit(buddy_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Buddy Zhangrenze");
