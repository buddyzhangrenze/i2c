#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>

#include <linux/slab.h>
#include <linux/mm_types.h>
#include <linux/mm.h>

#define DEV_NAME "buddy_test"
static char *buffer;
static char  array[10] = {0,1,2,3,4,5,6,7,8,9};
/*
 * open operation
 */
static int buddy_open(struct inode *inode,struct file *filp)
{
	printk(KERN_INFO "Open device\n");
	/*
	 * Get a page
	 */
	buffer = (char *)kmalloc(PAGE_SIZE,GFP_KERNEL);
	/*
	 * Let page not reclaim.
	 */
	SetPageReserved(virt_to_page(buffer));
	return 0;
}
/*
 * release opertion 
 */
static int buddy_release(struct inode *inode,struct file *filp)
{
	printk(KERN_INFO "Close device\n");
	/*
	 * Clear reclaim
	 */
	ClearPageReserved(virt_to_page(buffer));
	kfree(buffer);
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
 * mmap
 */
static int buddy_mmap(struct file *filp,
		struct vm_area_struct *vma)
{
	unsigned long page;
	unsigned char i;
	/*
	 * fill the struct
	 */
	unsigned long start = (unsigned long)vma->vm_start;
	unsigned long end   = (unsigned long)vma->vm_end;
	unsigned long size  = (unsigned long)(vma->vm_end - vma->vm_start);
	/*
	 * Get phy address.
	 */
	page = virt_to_phys(buffer);
	/*
	 * Let use's vma mmap into page
	 */
	if(remap_pfn_range(vma,start,page >> PAGE_SHIFT,size,PAGE_SHARED))
	{
		printk(KERN_INFO "Unable to mmap\n");
		return -1;
	}
	/*
	 * test operation
	 */
	for(i = 0; i < 10; i++)
	{
		buffer[i] = array[i];
	}
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
	.mmap      = buddy_mmap,
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
