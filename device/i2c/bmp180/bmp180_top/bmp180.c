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

#define DEV_NAME "bmp180"
#define DEBUG          0

static struct class *i2c_class;
static struct i2c_client *my_client;
static int i2c_major;

/*
 * bmp180 filp operation
 */
static const struct file_operations bmp180_fops = {
	.owner    = THIS_MODULE,
};

/*
 * device id table
 */
static struct i2c_device_id bmp180_id[] = {
	{"bmp180",0xEE},
	{},
};
/*
 * device probe
 */
static int bmp180_probe(struct i2c_client *client,const struct i2c_device_id *id)
{
	int res;
	struct device *dev;
	my_client = client;
	printk(KERN_INFO ">>>>>bmp180 probe<<<<<\n");
	dev = device_create(i2c_class,&my_client->dev,MKDEV(i2c_major,0),NULL,DEV_NAME);
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
 * device remove
 */
static int bmp180_remove(struct i2c_client *client)
{
	printk(KERN_INFO ">>>>>bmp180 remove<<<<<\n");
	device_destroy(i2c_class,MKDEV(i2c_major,0));
	return 0;
}
/*
 * i2c_driver
 */
static struct i2c_driver bmp180_driver = {
	.driver = {
		.name = "bmp180",
	},
	.probe    = bmp180_probe,
	.id_table = bmp180_id,
	.remove   = bmp180_remove,
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
	strlcpy(i2c_info.type,"bmp180",I2C_NAME_SIZE);
	i2c_info.addr = 0xEE;

	my_client = i2c_new_device(adap,&i2c_info);
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
	i2c_major = register_chrdev(0,DEV_NAME,&bmp180_fops);
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
	ret = i2c_add_driver(&bmp180_driver);
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
	unregister_chrdev(i2c_major,"bmp180");
out:
	return ret;

}
/*
 * exit module
 */
static __exit void buddy_exit(void)
{
	printk(KERN_INFO ">>>>>BMP180 exit<<<<<\n");
	i2c_del_driver(&bmp180_driver);
	class_destroy(i2c_class);
	unregister_chrdev(i2c_major,"bmp180");
}



MODULE_AUTHOR("Buddy <514981221@qq.com>");
MODULE_DESCRIPTION("BMP180 driver");
MODULE_LICENSE("GPL");

module_init(buddy_init);
module_exit(buddy_exit);
