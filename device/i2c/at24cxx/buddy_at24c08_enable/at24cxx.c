/*
 * Copyright@2014
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <linux/jiffies.h>
#include <linux/uaccess.h>
#include <linux/string.h>

#define DEV_NAME "at24c"
#define I2C_MINORS	256


static struct class *i2c_dev_class;
static struct i2c_client *my_client;
static int    i2c_major;

static struct i2c_device_id at24xx_ids[]=
{
		{"at24c01",0x50},
		{"at24c02",0x50},
		{"at24c04",0x50},
		{"at24c08",0x50},	
			{},
};


static ssize_t i2cdev_read(struct file *file, char __user *buf, size_t count, loff_t *offset)
{
	char *tmp;
	char reg_addr=0;
	int ret;
	struct i2c_msg msg[2];
	struct i2c_client *client= (struct i2c_client *)file->private_data;
	struct i2c_adapter *adap = client->adapter;
	reg_addr = *offset;
	
	printk(KERN_INFO "i2cdev_read\n");
	if(count > 8192)
		count = 8192;
	tmp = kmalloc(count,GFP_KERNEL);
	if(tmp == NULL)
		return -ENOMEM;

	printk(KERN_ERR"read:count = %d,offset = %d\n",(int)count,(int)*offset);

	msg[0].addr = client->addr;
	msg[0].flags = client->flags & I2C_M_TEN;
	msg[0].len = 1;
	msg[0].buf = &reg_addr;//read addr

	msg[1].addr = client->addr;
	msg[1].flags = client->flags & I2C_M_TEN;
	msg[1].flags |= I2C_M_RD;
	msg[1].len = count;
	msg[1].buf = tmp;//read data

	ret = i2c_transfer(adap, msg, 2);
	printk(KERN_ERR "i2c_transfer:ret=%d\n",ret);

	if (ret != 2)
		goto out;
	ret = copy_to_user(buf, tmp, count);
	printk(KERN_ERR "copy_to_user:ret=%d\n",ret);
	if(ret)
		goto out;
	kfree(tmp);
	return count;
	out:
		kfree(tmp);
		printk(KERN_ERR "i2cdev_read out:\n");
		return -1;
}

static ssize_t i2cdev_write(struct file *file, const char __user *buf,
		size_t count, loff_t *offset)
{
	int ret;
	int i;
	char *tmp;
	char tmp_data[2];
	struct i2c_client *client = file->private_data;
	struct i2c_adapter *adap = client->adapter;
	struct i2c_msg msg;

	printk(KERN_ERR "i2cdev_write\n");

	if (count > 8192)
		count = 8192;

	printk(KERN_ERR"write:count = %d,offset = %d\n",(int )count,(int)*offset);
	tmp = kmalloc(count, GFP_KERNEL);
	if (tmp == NULL)
		return -ENOMEM;
	if (copy_from_user(tmp, buf, count)) {
		kfree(tmp);
		printk(KERN_ERR"copy_from_user falied\n");
		return -EFAULT;
	}
 	msg.addr = client->addr;
	msg.flags = client->flags & I2C_M_TEN;
    for(i=0; i< count; i++)
	{
		tmp_data[0] = *offset +i;
		tmp_data[1] = *(tmp+i);
		msg.len = 2;
		msg.buf = tmp_data;		
		ret = i2c_transfer(adap, &msg, 1);
		if(1 != ret)
		{
			printk(KERN_ERR"i2cdev_write:i2c_transfer !=1 ret=%d\n",ret);
			goto out;
		}
			
	}
	kfree(tmp);
	/* If everything went ok (i.e. 1 msg transmitted), return #bytes
	   transmitted, else error code. */
	return (ret == 1) ? count : ret;	
	out:
		kfree(tmp);
		printk(KERN_ERR"in out ret=%d\n",ret);
		return -1;
}


static long i2cdev_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	struct i2c_client *client = file->private_data;
	printk(KERN_ERR "i2cdev_ioctl, cmd=0x%02x, arg=0x%02lx\n",cmd, arg);

	switch (cmd) {
	case I2C_TENBIT:
		if (arg)
			client->flags |= I2C_M_TEN;
		else
			client->flags &= ~I2C_M_TEN;
		return 0;

	case I2C_RETRIES:
		client->adapter->retries = arg;
		break;
	case I2C_TIMEOUT:
		/* For historical reasons, user-space sets the timeout
		 * value in units of 10 ms.
		 */
		client->adapter->timeout = msecs_to_jiffies(arg * 10);
		break;
	default:
		/* NOTE:  returning a fault code here could cause trouble
		 * in buggy userspace code.  Some old kernel bugs returned
		 * zero in this case, and userspace code might accidentally
		 * have depended on that bug.
		 */
		return -ENOTTY;
	}
	return 0;
}

static int i2cdev_open(struct inode *inode, struct file *file)
{

	printk(KERN_ERR "i2cdev_open\n");

	file->private_data = my_client;

	return 0;
}

static int i2cdev_release(struct inode *inode, struct file *file)
{
	//struct i2c_client *client = file->private_data;
	printk(KERN_ERR "i2cdev_release\n");
	file->private_data = NULL;
	return 0;
}

static const struct file_operations i2cdev_fops = {
	.owner		= THIS_MODULE,
	.read		= i2cdev_read,
	.write		= i2cdev_write,
	.unlocked_ioctl	= i2cdev_ioctl,
	.open		= i2cdev_open,
	.release	= i2cdev_release,
};

static int at24cxx_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
		int res;
		struct device *dev;
        printk(KERN_ERR "probe:name = %s,flag =%d,addr = %x,adapter = %d,driver = %s\n",client->name,  
        client->flags,client->addr,client->adapter->nr,client->driver->driver.name );	

		my_client = client;
	
		/* register this i2c device with the driver core */
		dev = device_create(i2c_dev_class, &my_client->dev,
						 MKDEV(i2c_major, 0), NULL,
						 DEV_NAME);
		if (IS_ERR(dev)) {
			res = PTR_ERR(dev);
			goto error;
		}
	
		return 0;
	error:
		return res;

}

static int at24xx_remove(struct i2c_client *client)
{
	printk(KERN_ERR "at24xx_remove\n");
	device_destroy(i2c_dev_class, MKDEV(I2C_MAJOR, 0));	
	return 0;

}
static struct i2c_driver at24cxx_driver = {
	.driver = {
		.name	= "at24c01",
	},
	.probe = at24cxx_probe,
	.id_table = at24xx_ids,
	.remove = at24xx_remove,
};

/*
 *at24cxx init
 */

static int __init at24cxx_init(void)
{
	int res = 0;
	/*
	 * add infomation for i2c board
	 */
	struct i2c_adapter *adap;
	struct i2c_board_info i2c_info;

	adap = i2c_get_adapter(0);
	if(adap == NULL)
	{
		printk(KERN_INFO "Unable to get i2c adapter\n");
		return -EFAULT;
	}
	printk(KERN_INFO "Succeed to get i2c adapter[%s]\n",adap->name);

	memset(&i2c_info,0,sizeof(struct i2c_board_info));
	strlcpy(i2c_info.type,"at24c01",I2C_NAME_SIZE);
	i2c_info.addr = 0x50;

	my_client = i2c_new_device(adap,&i2c_info);
	if(my_client) 
	{
		printk(KERN_INFO "Unable to create i2c client\n");
		return -EFAULT;
	}
	i2c_put_adapter(adap);
	/*
	 * register char driver 
	 */
	printk(KERN_ERR "at24cxx_init\n");

	i2c_major = register_chrdev(0, DEV_NAME, &i2cdev_fops);
	if (i2c_major == 0)
		goto out;
	printk(KERN_INFO "The driver major is %d\n",i2c_major);
	/*
	 * insearch driver into class
	 */
	i2c_dev_class = class_create(THIS_MODULE, DEV_NAME);
	if (IS_ERR(i2c_dev_class)) {
		res = PTR_ERR(i2c_dev_class);
		goto out_unreg_chrdev;
	}
	/*
	 * add i2c driver into system
	 */
	res = i2c_add_driver(&at24cxx_driver);
	if (res)
		goto out_unreg_class;

	return 0;

out_unreg_class:
	class_destroy(i2c_dev_class);
out_unreg_chrdev:
	unregister_chrdev(I2C_MAJOR, "at24c");
out:
	printk(KERN_ERR "%s: Driver Initialisation failed\n", __FILE__);
	return res;
}

static void __exit at24cxx_exit(void)
{
	printk(KERN_DEBUG "at24cxx_exit\n");
//	i2c_del_driver(&i2cdev_driver);
	class_destroy(i2c_dev_class);
	unregister_chrdev(I2C_MAJOR, "i2c");
}

MODULE_AUTHOR("yiheng <yihengzuji@163.com>");
MODULE_DESCRIPTION("I2C at24cxx entries driver");
MODULE_LICENSE("GPL");

module_init(at24cxx_init);
module_exit(at24cxx_exit);



