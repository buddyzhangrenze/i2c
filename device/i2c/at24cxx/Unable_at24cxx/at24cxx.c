#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL v2");

const struct i2c_device_id at24cxx_id[] = 
{
	{"at24cxx",0},
	/*结束标志，必须加，match匹配时要使用*/
	{}
};


dev_t dev;
struct at24cxx_dev
{
	struct cdev cdev;
	struct i2c_client *client;
};
struct at24cxx_dev *at24cxx_devp = NULL;

struct class *dev_class = NULL;
struct device *dev_device = NULL;


ssize_t at24cxx_write(struct file *filp,const char __user *buf,size_t size,loff_t *offset)
{
	/*保存用户空间输入的两个值*/
	unsigned char val[2];
	int ret;
	struct i2c_msg msg[1];

	if(size != 2)
	{
		return -EINVAL;
	}

	/*在内核中拿到要写入的值和偏移地址*/
	ret = copy_from_user(val,buf,2);

	/*按照I2C协议的格式发送数据，
	完成START...ADDR+W...ACK...OFFSET...ACK...VAL...ACK 整个发送全过程
	参数一adap:内核中有一个I2C控制器，就有一个adapter与之对应
	参数二msgs:
	参数三num:
	*/
	msg[0].addr = at24cxx_devp->client->addr; //实际上就是0x50
	msg[0].flags  = 0;  // 表示写信号
	msg[0].buf = val; //buf是一个指针,指向了发送的数据和偏移地址
	msg[0].len = 2; //msg[0].buf的数据长度
	ret = i2c_transfer(at24cxx_devp->client->adapter,msg,1);

	if(ret == 1)
	{
		return 2;
	}
	else 
	{
		return -EIO;
	}
}


ssize_t at24cxx_read(struct file *filp, char __user *buf, size_t size, loff_t *offset)
{
	unsigned char address;
	unsigned char data;
	struct i2c_msg msg[2];
	int ret;

	if(size != 1)
	{
		return -EINVAL;
	}

	ret = copy_from_user(&address,buf,1);

	/*有几个start就有几个msg，这里有两个*/
	msg[0].addr = at24cxx_devp->client->addr;
	msg[0].flags = 0;//写
	msg[0].buf = &address;
	msg[0].len = 1; //msg[0].buf数据的长度
	
	msg[1].addr = at24cxx_devp->client->addr;
	msg[1].flags = 1;//读
	msg[1].buf = &data;
	msg[1].len = 1;

	ret = i2c_transfer(at24cxx_devp->client->adapter,msg,2);

	/*将收到的数据拷贝到用户空间去*/
	ret = copy_to_user(buf,&data,1);
	
	return 1;
}


/*定义操作函数集合*/
struct file_operations at24cxx_fops = 
{
	.owner = THIS_MODULE,
	.read = at24cxx_read,
	.write = at24cxx_write,
};



int at24cxx_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	alloc_chrdev_region(&dev,0,1,"at24c02");
	
	printk(KERN_INFO "major is %d\n",dev);
	at24cxx_devp = kzalloc(sizeof(struct at24cxx_dev),GFP_KERNEL);

	/*client是匹配成功之后调用probe函数传递进来的参数，
	这个client表示是那个次设备，这里是at24c02，client这个结构体里面
	有一项是adapter，这个adapter表明了这个此设备是连接到哪个
	I2C控制器上面的，后面的发送函数需要这个参数，
	进行初始化，以便read和wirte函数在后面使用
	at24c02这个client创建成功时就知道它挂载在哪个I2C控制器上面了
	*/
	at24cxx_devp->client = client;
	cdev_init(&at24cxx_devp->cdev,&at24cxx_fops);
	cdev_add(&at24cxx_devp->cdev,dev,1);

	dev_class = class_create(THIS_MODULE,"AT24C02");
	dev_device = device_create(dev_class,NULL,dev,NULL,"at24cxx0");
	
	return 0;
}

int at24cxx_remove(struct i2c_client *client)
{
	device_destroy(dev_class,dev);
	class_destroy(dev_class);
	cdev_del(&at24cxx_devp->cdev);
	kfree(at24cxx_devp);
	unregister_chrdev_region(dev,1);

	return 0;
}



struct i2c_driver at24cxx_driver = 
{
	.driver = 
	{
		.name = "at24c",
		.owner = THIS_MODULE,
	},
	.probe = at24cxx_probe,
	.remove = at24cxx_remove,
	/*match匹配时使用*/
	.id_table = at24cxx_id,
};

int __init at24cxx_init(void)
{
	/*向I2C-CORE添加一个driver*/
	i2c_add_driver(&at24cxx_driver);
	
	return 0;
}

void __exit at24cxx_exit(void)
{
	i2c_del_driver(&at24cxx_driver);
}

module_init(at24cxx_init);
module_exit(at24cxx_exit);
