/*
 * Copyright@2014 Buddy Zhangrenze
 */
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/list.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <linux/jiffies.h>
#include <linux/uaccess.h>
#include <linux/string.h>

#define DEV_NAME "lm3642"
#define DEBUG          1

static struct class *i2c_class;
static struct i2c_client *my_client;
static int i2c_major;

#if DEBUG
static void lm3642_debug(struct i2c_client *client,char *name)
{
	printk(KERN_INFO "======================%s=======================\n=>client->name:%s\n=>client->addr:%08x\n=>adapter->name:%s\n",name,client->name,client->addr,client->adapter->name);
}
#endif 
/*
 * write operation 
 */
static ssize_t lm3642_write(struct file *filp,const char __user *buf,size_t count,loff_t *offset)
{
	int ret;
	char *tmp;
	struct i2c_client *client = filp->private_data;
	struct i2c_msg msg[2];
	char tmp_data1[2] = {0x0a,0x00};
	char tmp_data[2]  = {0x09,0x0F};

	printk(KERN_INFO ">>>>>lm3642 write<<<<<\n");

	tmp = kmalloc(count,GFP_KERNEL);
	if(tmp == NULL)
	{
		printk(KERN_INFO "Unable to get enough memory\n");
		return -ENOMEM;
	}
	printk(KERN_INFO "Succeed to get memory\n");

	ret = copy_from_user(tmp,buf,count);
	if(ret != 0)
		printk(KERN_INFO "Loss %d bytes\n",ret);
	printk(KERN_INFO "Succeed to get %d bytes from Userspace\n",count);

	/*
	 * i2c transfer
	 */
	msg[0].addr     = client->addr;
	msg[0].flags    = client->flags | I2C_M_TEN;
	msg[0].len      = 2;
	msg[0].buf      = tmp_data1;

	msg[1].addr     = client->addr;
	msg[1].flags    = client->flags | I2C_M_TEN;
	msg[1].len      = 2;
	msg[1].buf      = tmp_data;

	ret = i2c_transfer(client->adapter,msg,2);
	if(ret != 2)
	{
		printk(KERN_INFO "Fail to transfer data\n");
		goto out_free;
	}
	printk(KERN_INFO "Succeed to transfer data\n");
	kfree(tmp);
	return 0;
out_free:
	kfree(tmp);
	return ret;
}
/*
 * read operation
 */
static ssize_t lm3642_read(struct file *filp,char __user *buf,size_t count,loff_t *offset)
{
	int ret;
	char *tmp;
	struct i2c_client *client = filp->private_data;
	struct i2c_msg msg[2];
	char reg_addr = 0xf6;
	printk(KERN_INFO ">>>>>lm3642 read<<<<<\n");

	tmp = kmalloc(count,GFP_KERNEL);
	if(tmp == NULL)
	{
		printk(KERN_INFO "Unable to get memory from system\n");
		return -ENOMEM;
	}
	printk(KERN_INFO "Succeed to get memory\n");
#if DEBUG
	lm3642_debug(client,"read");
#endif
	/*
	 * read data
	 */
	msg[0].addr   = client->addr;
	msg[0].flags  = client->flags | I2C_M_TEN;
	msg[0].len    = 1;
	msg[0].buf    = &reg_addr;

	msg[1].addr   = client->addr;
	msg[1].flags  = client->flags | I2C_M_TEN;
	msg[1].flags |= I2C_M_RD;
	msg[1].len    = 2;
	msg[1].buf    = tmp;

	ret = i2c_transfer(client->adapter,msg,2);
	if(ret != 2)
		printk(KERN_INFO "Fail to transfer data\n");
	printk(KERN_INFO "Succeed to transfer datat\n");
	/*
	 * push data to Userspace
	 */	
	ret = copy_to_user(buf,tmp,count);
	if(ret != 0)
	{
		printk(KERN_INFO "loss the %d bytes\n",(int)(count - ret));
		goto out_free;
	} else
		printk(KERN_INFO "Succeed to push %d bytes to Userspace\n",count);
	kfree(tmp);
	return 0;
out_free:
	kfree(tmp);
	return ret;
}
/*
 * open operation
 */
static int lm3642_open(struct inode *inode,struct file *filp)
{
	printk(KERN_INFO ">>>>>lm3642 open<<<<<\n");
	filp->private_data = my_client;
	return 0;
}
/*
 * release operation
 */
static int lm3642_release(struct inode *inode,struct file *filp)
{
	printk(KERN_INFO ">>>>>lm3642 release<<<<<\n");
	filp->private_data = NULL;
	return 0;
}
/*
 * lm3642 filp operation
 */
static const struct file_operations lm3642_fops = {
	.owner    = THIS_MODULE,
	.read     = lm3642_read,
	.write    = lm3642_write,
	.open     = lm3642_open,
	.release  = lm3642_release,
};

/*
 * device id table
 */
static struct i2c_device_id lm3642_id[] = {
	{"lm3642",0x63},
	{},
};
/*
 * device probe
 */
static int lm3642_probe(struct i2c_client *client,const struct i2c_device_id *id)
{
	int res;
	struct device *dev;
	my_client = client;
	printk(KERN_INFO ">>>>>lm3642 probe<<<<<\n");
	dev = device_create(i2c_class,&my_client->dev,MKDEV(i2c_major,0),NULL,DEV_NAME);
	if(IS_ERR(dev))
	{
		res = PTR_ERR(dev);
		goto error;
	}
#if DEBUG
	lm3642_debug(client,"probe");
#endif
	return 0;
error:
	return res;
}
/*
 * device remove
 */
static int lm3642_remove(struct i2c_client *client)
{
	printk(KERN_INFO ">>>>>lm3642 remove<<<<<\n");
	device_destroy(i2c_class,MKDEV(i2c_major,0));
	return 0;
}
/*
 * i2c_driver
 */
static struct i2c_driver lm3642_driver = {
	.driver = {
		.name = "lm3642",
	},
	.probe    = lm3642_probe,
	.id_table = lm3642_id,
	.remove   = lm3642_remove,
};
/*
 * init module
 */
static __init int buddy_init(void)
{
	int ret;
	/*
	 * add i2c-board information
	 */
	struct i2c_adapter *adap;
	struct i2c_board_info i2c_info;

	adap = i2c_get_adapter(0);
	if(adap == NULL)
	{
		printk(KERN_INFO "Unable to get i2c adapter\n");
		ret = -EFAULT;
		goto out;
	}
	printk(KERN_INFO "Succeed to get i2c adapter[%s]\n",adap->name);
	
	memset(&i2c_info,0,sizeof(struct i2c_board_info));
	strlcpy(i2c_info.type,"lm3642",I2C_NAME_SIZE);
	i2c_info.addr = 0x63;

	my_client = i2c_new_device(adap,&i2c_info);
#if DEBUG
	lm3642_debug(my_client,"init");
#endif
	if(my_client == NULL)
	{
		printk(KERN_INFO "Unable to get new i2c device\n");
		ret = -ENODEV;
		i2c_put_adapter(0);
		goto out;
	}
	i2c_put_adapter(adap);

	printk(KERN_INFO ">>>>>module init<<<<<<\n");
	/*
	 * Register char device
	 */
	i2c_major = register_chrdev(0,DEV_NAME,&lm3642_fops);
	if(i2c_major == 0)
	{
		printk(KERN_INFO "Unable to register driver into char-driver\n");
		ret = -EFAULT;
		goto out;
	}	
	printk(KERN_INFO "Succeed to register driver,char-driver is %d\n",i2c_major);
	/*
	 * create device class
	 */
	i2c_class = class_create(THIS_MODULE,DEV_NAME);
	if(IS_ERR(i2c_class))
	{
		printk(KERN_INFO "Unable to create class file\n");
		ret = PTR_ERR(i2c_class);
		goto out_unregister;
	}
	printk(KERN_INFO "Succeed to create class file\n");
	/*
	 * create i2c driver
	 */
	ret = i2c_add_driver(&lm3642_driver);
	if(ret)
	{
		printk(KERN_INFO "Unable to add i2c driver\n");
		ret = -EFAULT;
		goto out_class;
	}
	printk(KERN_INFO "Succeed to add i2c driver\n");
	return 0;
out_class:
	class_destroy(i2c_class);
out_unregister:
	unregister_chrdev(i2c_major,"lm3642");
out:
	return ret;

}
/*
 * exit module
 */
static __exit void buddy_exit(void)
{
	printk(KERN_INFO ">>>>>lm3642 exit<<<<<\n");
	i2c_del_driver(&lm3642_driver);
	class_destroy(i2c_class);
	unregister_chrdev(i2c_major,"lm3642");
}



MODULE_AUTHOR("Buddy <514981221@qq.com>");
MODULE_DESCRIPTION("lm3642 driver");
MODULE_LICENSE("GPL");

module_init(buddy_init);
module_exit(buddy_exit);
