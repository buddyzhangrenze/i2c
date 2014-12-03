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
	/*������־������ӣ�matchƥ��ʱҪʹ��*/
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
	/*�����û��ռ����������ֵ*/
	unsigned char val[2];
	int ret;
	struct i2c_msg msg[1];

	if(size != 2)
	{
		return -EINVAL;
	}

	/*���ں����õ�Ҫд���ֵ��ƫ�Ƶ�ַ*/
	ret = copy_from_user(val,buf,2);

	/*����I2CЭ��ĸ�ʽ�������ݣ�
	���START...ADDR+W...ACK...OFFSET...ACK...VAL...ACK ��������ȫ����
	����һadap:�ں�����һ��I2C������������һ��adapter��֮��Ӧ
	������msgs:
	������num:
	*/
	msg[0].addr = at24cxx_devp->client->addr; //ʵ���Ͼ���0x50
	msg[0].flags  = 0;  // ��ʾд�ź�
	msg[0].buf = val; //buf��һ��ָ��,ָ���˷��͵����ݺ�ƫ�Ƶ�ַ
	msg[0].len = 2; //msg[0].buf�����ݳ���
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

	/*�м���start���м���msg������������*/
	msg[0].addr = at24cxx_devp->client->addr;
	msg[0].flags = 0;//д
	msg[0].buf = &address;
	msg[0].len = 1; //msg[0].buf���ݵĳ���
	
	msg[1].addr = at24cxx_devp->client->addr;
	msg[1].flags = 1;//��
	msg[1].buf = &data;
	msg[1].len = 1;

	ret = i2c_transfer(at24cxx_devp->client->adapter,msg,2);

	/*���յ������ݿ������û��ռ�ȥ*/
	ret = copy_to_user(buf,&data,1);
	
	return 1;
}


/*���������������*/
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

	/*client��ƥ��ɹ�֮�����probe�������ݽ����Ĳ�����
	���client��ʾ���Ǹ����豸��������at24c02��client����ṹ������
	��һ����adapter�����adapter������������豸�����ӵ��ĸ�
	I2C����������ģ�����ķ��ͺ�����Ҫ���������
	���г�ʼ�����Ա�read��wirte�����ں���ʹ��
	at24c02���client�����ɹ�ʱ��֪�����������ĸ�I2C������������
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
	/*matchƥ��ʱʹ��*/
	.id_table = at24cxx_id,
};

int __init at24cxx_init(void)
{
	/*��I2C-CORE���һ��driver*/
	i2c_add_driver(&at24cxx_driver);
	
	return 0;
}

void __exit at24cxx_exit(void)
{
	i2c_del_driver(&at24cxx_driver);
}

module_init(at24cxx_init);
module_exit(at24cxx_exit);
