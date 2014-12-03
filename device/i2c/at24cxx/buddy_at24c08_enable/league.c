/*
 * i2c read for at24c08
 */
static ssize_t buddy_read(struct file *filp,char __user *buf,size_t count,loff_t *offset)
{
	char *tmp;
	char reg_addr;
	int ret;
	struct i2c_msg msg[2];
	struct i2c_client *client = (struct i2c_client *)filp->private_data;
	struct i2c_adapter *adap  = client->adap;
}





