#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>

#define DEV_NAME "buddy_test"
#define NO_DEBUG  1

















static void bud()
{
	int i;
	int m;
}
/*
 * Elf32
 */
static void buddy_ELF(void)
{
	struct module *md = THIS_MODULE;
	int i;
	int type;
	int bind;
	char info;
	char *tmp = md->strtab;
	unsigned long addr;
	/*
	 * print all name
	 */
	for(i = 0;i<md->num_symtab;i++)
	{
		if(md->strtab[i] == '\0')
		{
			tmp = md->strtab + i +1;
			printk(KERN_INFO "[%s]\n",tmp);
		}
	}
#if NO_DEBUG
	/*
	 * print all character
	 */
	printk(KERN_INFO "test\n");
	for(i = 0;i<md->num_symtab;i++)
	{
		printk(KERN_INFO "[%s]\n",md->strtab + md->symtab[i].st_name);
	}
	/*
	 * print all ch
	 */
	printk(KERN_INFO "Scan all\n");
	for(i = 0;i< md->num_symtab;i++)
		printk(KERN_INFO "[%c]\n",md->strtab[md->symtab[i].st_name]);
#endif
	/*
	 * print all function name
	 */
	printk(KERN_INFO "show\n");
	for(i = 0;i < md->num_symtab ;i++)
	{
		info = md->symtab[i].st_info;		
		type = ELF_ST_TYPE(md->symtab[i].st_info);
		bind = ELF_ST_BIND(md->symtab[i].st_info);
		tmp  = md->strtab + md->symtab[i].st_name;
		addr = md->symtab[i].st_value;
		printk(KERN_INFO "info[%d]addr[%lx]",info,addr);
		if(type == STT_FUNC)
			printk(KERN_INFO "FUNC[%s][%d]\n",tmp,bind);
		else if(type == STT_NOTYPE)
			printk(KERN_INFO "NO_TYPE[%s]\n",tmp);
		else if(type == STT_OBJECT)
			printk(KERN_INFO "OBJECT[%s]\n",tmp);
		else if(type == STT_SECTION)
			printk(KERN_INFO "SECTION[%s]\n",tmp);
		else if(type == STT_FILE)
			printk(KERN_INFO "FILE[%s][%d]\n",tmp,bind);
		else if(tmp == STT_COMMON)
			printk(KERN_INFO "COMMON[%s]\n",tmp);
		else if(tmp == STT_TLS)
			printk(KERN_INFO "TLS[%s]\n",tmp);
	}
	printk(KERN_INFO "The core symtab\n");
	for(i = 0 ; i < md->core_num_syms; i++)
	{
		printk(KERN_INFO "[%s][%d]\n",
			md->core_strtab + md->core_strtab[md->core_symtab[i].st_name],
				md->core_symtab[i].st_info);
	}
}
/*
 * Symbol section for module
 */
static void buddy_ssm(void)
{
	struct module *md = THIS_MODULE;
	int i,j;
	char tmp[100];
	if(md->num_symtab > 0)
	{
		for(i = 0; i< md->num_symtab ; i++)
		{
			printk(KERN_INFO "\n\n");
			printk(KERN_INFO "symtab[%d].st_value:%x\n"
					"strtab[%c]\n",
					i,md->symtab[i].st_value,
					*(md->strtab + md->symtab[i].st_name));
			for(j = 0;j<md->symtab[i].st_name;j++)
				tmp[j] = *(md->strtab + j);
			printk(KERN_INFO "%s",tmp);
		}
	}
}
EXPORT_SYMBOL(buddy_ssm);
/*
 * open operation
 */
static int buddy_open(struct inode *inode,struct file *filp)
{
	printk(KERN_INFO "Open device\n");
//	buddy_ssm();
	buddy_ELF();
	return 0;
}
/*
 * release opertion 
 */
static int buddy_release(struct inode *inode,struct file *filp)
{
	printk(KERN_INFO "Close device\n");
	return 0;
}
/*
 * read operation
 */
static ssize_t buddy_read(struct file *filp,char __user *buffer,size_t count,
		loff_t *offset)
{
	printk(KERN_INFO "read device\n");
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
