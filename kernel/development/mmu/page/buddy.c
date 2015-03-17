#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>

#include <linux/mm_types.h>
#include <asm/current.h>
#include <asm/page.h>
#include <linux/sched.h>
#include <asm/pgtable.h>
#include <linux/mm.h>

#define DEV_NAME "buddy_test"

static struct page *my_page;
static unsigned long va;
/*
 * Page transfer
 */
static void buddy_PT(void)
{
	pgd_t *pgd_tmp = NULL;
	pud_t *pud_tmp = NULL;
	pmd_t *pmd_tmp = NULL;
	pte_t *pte_tmp = NULL;

	/*
	 * test virtual address.
	 */
	va = current->mm->start_data;
	/*
	 * chech address whether is safe.
	 */
	if(!find_vma(current->mm,va))
	{
		printk(KERN_INFO "virtual 0x%lx not available.\n",va);
		return;
	}
	/*
	 * Get pgd 
	 * #define pgd_offset mm->pgd + pgd_index(va)
	 * #define pgd_index ((va)>>PGDIR_SHIFT)
	 */
	pgd_tmp = pgd_offset(current->mm,va);
	printk(KERN_INFO "The virtual address are[%lx]\n",va);
	printk(KERN_INFO "The PGD_SHIFT are[%ld]\n",PGDIR_SHIFT);
	printk(KERN_INFO "The mm pgd are[08%lx]\n",current->mm->pgd);
	printk(KERN_INFO "The virtual address's pgd are[08%lx]\n",pgd_tmp);
	printk(KERN_INFO "The virtual address pgd_val[%8lx]\n",pgd_val(*pgd_tmp));
	printk(KERN_INFO "The *address are[%8lx]\n",*pgd_tmp);
	/*
	 * Get pud
	 */
	pud_tmp = pud_offset(pgd_tmp,va);
	printk(KERN_INFO "[%8lx]The pud\n",pud_tmp);
	/*
	 * Get pmd
	 * #define pmd_offset(dir,addr) ((pmd_t *)(dir))
	 */
	pmd_tmp = pmd_offset(pud_tmp,va);
	printk(KERN_INFO "[%8lx]The pmd\n",pmd_tmp);
	/*
	 * Get pte
	 */
	pte_tmp = pte_offset_kernel(pmd_tmp,va);
	printk(KERN_INFO "[%8lx]The pte\n",pte_tmp);
}
/*
 * Show page information
 */
static void buddy_page_info(struct page *p)
{
	printk(KERN_INFO "The page infomation as:\n");
	printk(KERN_INFO "The page flags are[%x]\n",p->flags);
	printk(KERN_INFO "The Use count of page are [%d]\n",p->_count.counter);
}
/*
 * open operation
 */
static int buddy_open(struct inode *inode,struct file *filp)
{
	printk(KERN_INFO "Open device\n");
	return 0;
}
/*
 * release opertion 
 */
static int buddy_release(struct inode *inode,struct file *filp)
{
	unsigned long page_addr =  (unsigned long)filp->private_data;
	printk(KERN_INFO "Close devuce\n");
	/*
	 * free a page
	 */
	__free_page(my_page);
	/*
	 * free virtual address.
	 */
	free_page(page_addr);
	return 0;
}
/*
 * read operation
 */
static ssize_t buddy_read(struct file *filp,char __user *buffer,size_t count,
		loff_t *offset)
{
	unsigned long page_addr = 0;
	filp->private_data = (void *)page_addr;

	printk(KERN_INFO "read device\n");
	/*
	 * Get a struct for page
	 */
	my_page = alloc_page(GFP_KERNEL);
	if(my_page == NULL)
		printk(KERN_INFO "Unable to get page from buddy-system\n");
	else
	{
		printk(KERN_INFO "Succeed to get page from buddy_system\n");
		buddy_page_info(my_page);
	}
	/*
	 * Get address for page
	 */
	page_addr = __get_free_page(GFP_KERNEL);
	if(page_addr == 0)
		printk(KERN_INFO "Can't get free page\n");
	else
		printk(KERN_INFO "Get free page address is[%x]\n",page_addr);
	/*
	 * Page Table
	 */
	buddy_PT();
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
