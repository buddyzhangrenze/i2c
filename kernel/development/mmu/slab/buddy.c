#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>

#include <linux/slab.h>
#include <linux/mm_types.h>
#define DEV_NAME "buddy_test"

struct buddy_st {
	int data;
	char ch;
};
struct buddy_ti {
	long long data;
	long long da;
	char a;
};
/*
 * Cache
 */
static struct kmem_cache *my_cache;
static struct kmem_cache *b_cache;
/*
 * slab for page
 */
static void buddy_slab_page(struct kmem_cache_cpu *k)
{
	struct page *page = k->page;
	printk(KERN_INFO "The current node are[%d]\n",k->node);
	printk(KERN_INFO "freelist address are[%x]\n",(unsigned long)k->freelist);
	printk(KERN_INFO "The page are[%s]\n",page->slab->name);
#ifdef CONFIG_SLUB_STATS
	printk(KERN_INFO "The state are[%d]\n",k->stat[0]);
#endif
}
/*
 * Cache slab list
 */
static void buddy_node_list(struct kmem_cache *c)
{
	int i;
	struct kmem_cache_node *n;
	for(i = 0;i< MAX_NUMNODES;i++)
	{
		n = c->node[i];
		printk(KERN_INFO "The[%d] nr partial are [%ld]\n",
				i,n->nr_partial);
		if(n->partial.next == n->partial.prev)
			printk(KERN_INFO "No member\n");
		else
			printk(KERN_INFO "Have member\n");
#ifdef CONFIG_SLUB_DEBUG
		printk(KERN_INFO "The[%d] nr full are[%d]\n",
				n->nr_slabs.counter);
		if(n->full.next == n->full.prev)
			printk(KERN_INFO "No Full\n");
		else
			printk(KERN_INFO "Have full\n");
#endif
	}
}
/*
 * Get Cache information
 */
static void buddy_cache_info(struct kmem_cache *c)
{
	printk(KERN_INFO "Cache name are [%s]\n",c->name);
	printk(KERN_INFO "Cache a object size include meta data are[%d]\n",c->size);
	printk(KERN_INFO "Cache a object size are[%d]\n",c->objsize);
	printk(KERN_INFO "Cache free point offset[%d]\n",c->offset);
	printk(KERN_INFO "CaChe gfp flags[%d]\n",c->allocflags);
	printk(KERN_INFO "Cache refcount are[%d]\n",c->refcount);
	printk(KERN_INFO "Cache number of inuse[%d]\n",c->inuse);
	printk(KERN_INFO "Cache align are[%d]\n",c->align);
	printk(KERN_INFO "Cache min partial are[%ld]\n",c->min_partial);
	printk(KERN_INFO "Cache allocation are[%ld]\n",c->max.x);
	printk(KERN_INFO "Cache freeing are[%ld]\n",c->min.x);
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
	/*
	 * Free cache
	 */
	kmem_cache_destroy(b_cache);
	kmem_cache_destroy(my_cache);
	return 0;
}
/*
 * read operation
 */
static ssize_t buddy_read(struct file *filp,char __user *buffer,size_t count,
		loff_t *offset)
{
	struct buddy_st *my_st;
	printk(KERN_INFO "read device\n");
	/*
	 * Create a cache for struct buddy.
	 */
	my_cache = kmem_cache_create("Buddy_cache",
			sizeof(struct buddy_st),0,
			SLAB_HWCACHE_ALIGN,NULL);
	if(my_cache == NULL)
	{
		printk(KERN_INFO "Can't create Cache\n");
		return -1;
	} else 
		printk(KERN_INFO "Succeed to create Cache\n");
	
	b_cache = kmem_cache_create("By_cache",
			sizeof(struct buddy_ti),0,
			SLAB_HWCACHE_ALIGN,NULL);
	/*
	 * show cache
	 */
	buddy_cache_info(my_cache);
	buddy_cache_info(b_cache);
	/*
	 * alloc from cache
	 */
	my_st = (struct buddy_st *)kmem_cache_alloc(my_cache,GFP_KERNEL);
	my_st->ch = 'A';
	printk(KERN_INFO "The struct from slub are[%c]\n",my_st->ch);
	printk(KERN_INFO "The cache size are[%d]\n",kmem_cache_size(my_cache));
	/*
	 *
	 */
	buddy_node_list(my_cache);
	kmem_cache_free(my_cache,my_st);
	buddy_slab_page(my_cache->cpu_slab);
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
