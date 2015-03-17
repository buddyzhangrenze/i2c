#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>

#include <asm/current.h>
#include <linux/mm_types.h>
#include <linux/sched.h>
#include <asm/page.h>
#define DEV_NAME "buddy_test"
/*
 * Vm info
 */
static void buddy_all_vma(struct vm_area_struct *vm)
{
	struct vm_area_struct *p = vm;
	for(;p != NULL;p = p->vm_next)
	{
		printk(KERN_INFO "vm_start[%8u]\n",p->vm_start);
		printk(KERN_INFO "vm_end  [%8u]\n",p->vm_end);
	}
}
/*
 * PGD PMD PTE
 */
static void buddy_P(pgd_t p)
{
	printk(KERN_INFO "The Task's PGD[%08x]\n",p);
}
/*
 * VM infomation
 */
static void buddy_vm_info(struct vm_area_struct *vm)
{
	printk(KERN_INFO "Vm start are[%u]\n",vm->vm_start);
	printk(KERN_INFO "Vm end are[%u]\n",vm->vm_end);
	printk(KERN_INFO "Vm flags are[%u]\n",vm->vm_flags);
	printk(KERN_INFO "Vm vm page offset are[%u]\n",vm->vm_pgoff);
	printk(KERN_INFO "Vm trrncate count are[%u]\n",vm->vm_truncate_count);
}
/*
 * Get mm_struct information
 */
static void mm_info(struct mm_struct *m)
{
	printk(KERN_INFO "Base of mmap area are[%u]\n",m->mmap_base);
	printk(KERN_INFO "Size of task vm space are[%u]\n",m->task_size);
	printk(KERN_INFO "free are cache are[%u]\n",m->cached_hole_size);
	printk(KERN_INFO "larger are [%u]\n",m->free_area_cache);
	printk(KERN_INFO "The number of VMAs are[%d]\n",m->map_count);
	printk(KERN_INFO "Total_vm are[%u]\n",m->total_vm);
	printk(KERN_INFO "Stack vm are[%u]\n",m->stack_vm);
	printk(KERN_INFO "Task code segment area[%u - %u]\n",
			m->start_code,m->end_code);
	printk(KERN_INFO "Task data segment area[%u - %u]\n",
			m->start_data,m->end_data);
	printk(KERN_INFO "Task brk segment area[%u - %u]\n",
			m->start_brk,m->brk);
	printk(KERN_INFO "Task stack segment area[%u - %u]\n",
			m->start_stack,m->brk);
	printk(KERN_INFO "Task arg segment area[%u - %u]\n",
			m->arg_start,m->arg_end);
	printk(KERN_INFO "Task env segment area[%u - %u]\n",
			m->env_start,m->env_end);
	printk(KERN_INFO "The flags are[%u]\n",m->flags);
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
	printk(KERN_INFO "Close device\n");
	return 0;
}
/*
 * read operation
 */
static ssize_t buddy_read(struct file *filp,char __user *buffer,size_t count,
		loff_t *offset)
{
	struct mm_struct *m = current->mm;
	mm_info(m);
	buddy_vm_info(m->mmap);
	buddy_P(*(m->pgd));
	buddy_all_vma(m->mmap);
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
