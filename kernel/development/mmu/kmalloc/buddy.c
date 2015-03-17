#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>

#define DEV_NAME "buddy_test"
/*
 * underlying alloc API
 */
void *buddy_kmem_cache_alloc(struct kmem_cache *s,gfp_t gfpflags)
{
	void *ret = slab_alloc(s,gfpflags,NUMA_NO_NODE,_RET_IP_);
	trace_kmem_cache_alloc(_RET_IP_,ret,s->objsize,s->size,gfpflags);
	return ret;
}
static __always_inline void *
buddy_kmem_cache_alloc_trace(struct kmem_cache *s,gfp_t gfpflags,
		size_t size)
{
	return buddy_kmem_cache_alloc(s,gfpflags);
}
/*
 * Underlying kmalloc
 */
void *__buddy_kmalloc(size_t size,gfp_t flags)
{
	struct kmem_cache *s;
	void *ret;
	
	if(unlikely(size > SLUB_MAX_SIZE))
		return NULL;
	/*
	 * Get kmem_cache for slab.
	 */
	s = get_slab(size,flags);
	if(unlikely(ZERO_OR_NULL_PTR(s)))
		return s;
	ret = slab_alloc(s,flags,NUMA_NO_NODE,_RET_IP_);
	trace_kmalloc(_RET_IP_,ret,size,s->size,flags);
	return ret;
}
/*
 * buddy kmalloc API
 */
static __always_inline void *buddy_kalloc(size_t size,gfp_t flags)
{
	if(__builtin_constant_p(size))
	{
		/*
		 * If size bigger than SLUB_MAX_SIZE,return NULL.
		 */
		if(size > SLUB_MAX_SIZE)
			return NULL;
		if(!(flags & SLUB_DMA))
		{
			/*
			 * Get kmem_cache for SLAB object.
			 */
			struct kmem_cache *s = kmalloc_slab(size);
			if(!s)
				return ZERO_SIZE_PTR;
			return buddy_kmem_cache_alloc_trace(s,flags,size);
		}
	}
	return __buddy_kmalloc(size,flags);
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
