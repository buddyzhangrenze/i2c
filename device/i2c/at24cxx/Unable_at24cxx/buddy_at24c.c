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
#define DEBUG     1

static struct class *i2c_dev_class;
static struct i2c_client *my_client;
static int i2c_major;

#if DEBUG
static void buddy_debug(struct i2c_client *client,char *s)
{
	printk(KERN_INFO "======clinet %s======\n=>client->addr:%08x\n=>client->name:%s\n=>client->adapter:%s\n=>client->flags:%08x\n========================\n",s,client->addr,client->name,client->adapter->name,client->flags);
}
#endif
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
 * read - file operations
 */
static ssize_t buddy_read(struct file *filp,char __user *buf,size_t count,loff_t *offset)
{
	struct i2c_client *client = filp->private_data;
#if DEBUG
	buddy_debug(client,"read");
#endif
	printk(KERN_INFO "Buddy read file\n");
	return 0;
}
/*
 * struct for file_operations
 */
static const struct file_operations buddy_ops = 
{
	.owner    = THIS_MODULE,
	.read     = buddy_read,
	.open     = buddy_open,  
	.release  = buddy_release,
};
/*
 *	i2c id table
 */
static const struct i2c_device_id id_table[] = {
	{"at24c01",0x50},
	{"at24c02",0x50},
};
/*
 * probe client
 */
static int buddy_probe(struct i2c_client *client,const struct i2c_device_id *id)
{
	int res;
	struct device *dev;
#if DEBUG
	buddy_debug(my_client,"probe0");
#endif
	my_client = client;
#if DEBUG
	buddy_debug(my_client,"probe1");
#endif
	/*
	 * register this i2c driver into driver core
	 */
	dev = device_create(i2c_dev_class,&my_client->dev,MKDEV(i2c_major,0),NULL,DEV_NAME);
	if(IS_ERR(dev))
	{
		res = PTR_ERR(dev);
		goto error;
	}
	return 0;
error:
	return res;

}
/*
 * remove i2c-driver
 */
static int buddy_remove(struct i2c_client *client)
{
#if DEBUG
	buddy_debug(client,"remove");
#endif
	printk(KERN_INFO "at24c_remove\n");
	device_destroy(i2c_dev_class,MKDEV(i2c_major,0));
	return 0;
}
/*
 * i2c driver
 */
static struct i2c_driver buddy_i2c_driver = {
	.driver = {
		.name = "at24c01",
	},
	.probe   = buddy_probe,
	.remove  = buddy_remove,
	.id_table= id_table,
};
/*
 * init module
 */
static __init  int buddy_init(void)
{
	int ret = 0;
	/*
	 * create i2c board information
	 */
	struct i2c_adapter *adap;
	struct i2c_board_info i2c_info;

	adap = i2c_get_adapter(0);
	if(adap == NULL)
	{
		printk(KERN_INFO "Unable to get i2c bus adapter\n");
		return -EFAULT;
	}
	printk(KERN_INFO "Succeed to get i2c adapter\n");
	memset(&i2c_info,0,sizeof(struct i2c_board_info));
	strlcpy(i2c_info.type,"at24c02",I2C_NAME_SIZE);
	i2c_info.addr = 0x50;

	my_client = i2c_new_device(adap,&i2c_info);
	if(my_client == NULL)
	{
		printk(KERN_INFO "Unable to get at24c client\n");
		i2c_put_adapter(adap);
		return -EFAULT;
	}
	printk(KERN_INFO "Succeed to get client\n");
	i2c_put_adapter(adap);
#if DEBUG
	buddy_debug(my_client,"init");
#endif
	/*
	 * register chardev
	 */
	i2c_major = register_chrdev(0,DEV_NAME,&buddy_ops);
	if(i2c_major == 0)
	{
		printk(KERN_INFO "Unable register chrdev into kernel\n");
		goto out;
	}
	printk(KERN_INFO "Succeed to register chrdev into kernel\n");
#if DEBUG
	printk(KERN_INFO "The char major is %d\n",i2c_major);
#endif
	/*
	 * create class
	 */
	i2c_dev_class = class_create(THIS_MODULE,DEV_NAME);
	if(i2c_dev_class == NULL)
	{
		printk(KERN_INFO "Unable to create class\n");
		goto out_register;
	}
	printk(KERN_INFO "Succeed to create class\n");
	/*
	 * add i2c-driver into i2c core
	 */
	ret = i2c_add_driver(&buddy_i2c_driver);
	if(ret)
	{
		printk(KERN_INFO "Unable to add i2c driver to i2c core\n");
		goto out_class;
	}
	return 0;
	/*
	 * deal with error
	 */
out_class:
	class_destroy(i2c_dev_class);
out_register:
	unregister_chrdev(i2c_major,DEV_NAME);
out:
	return -EFAULT;
}
/*
 * exit module
 */
static __exit void buddy_exit(void)
{
	printk(KERN_INFO "at24c01 exit\n");
	i2c_del_driver(&buddy_i2c_driver);
	class_destroy(i2c_dev_class);
	unregister_chrdev(i2c_major,DEV_NAME);
}
module_init(buddy_init);
module_exit(buddy_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Buddy Zhangrenze");