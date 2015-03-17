#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/slab.h>
#include <linux/compiler.h>
#include <trace/events/kmem.h>
#include <asm/page.h>
#include <asm/cache.h>

#define DEV_NAME "buddy_test"

#define TEST                              1
#define ALLOC_PAGE                        0
#define ALLOC_PAGES                       0
#define __GET_FREE_PAGE                   0
#define __GET_FREE_PAGES                  0
#define __GET_DMA_PAGES                   0
#define __KMALLOC                         0
#define _KMALLOC                          0
#define __BUILTIN                         0
#define CACHE_TEST                        0
#define CACHE_USE                         1


#if CACHE_USE

#endif
#if __BUILTIN
static void _builtin_test(void)
{
	int b = 5;
	int *a = &b;
#define ZERO_OR_NULL(x) (unsigned long)(x) <= \
	(unsigned long)(ZERO_SIZE_PTR)
	if(__builtin_expect(ZERO_OR_NULL(a),1))
		printk(KERN_INFO "This is a null point\n");
	else
		printk(KERN_DEBUG "This is not a null point\n");
}
#endif
#if CACHE_TEST
struct buddy_cache {
	size_t cs_size;
	struct kmem_cache *cs_cache;
};
struct buddy_cache buddy_sizes[] = {
	#define CACHE(x) {.cs_size = x},
	#include "cache.h"
		CACHE(ULONG_MAX)
	#undef CACHE
};
#endif

#if __KMALLOC
static inline struct kmem_cache *buddy___find_general_cachep(size_t size,
		gfp_t gfpflags)
{
	struct buddy_cache *cachep = buddy_sizes;
	printk(KERN_INFO "[__find_general_cachep]\n");
	if(!size)
		return ZERO_SIZE_PTR;
	while(size > cachep->cs_size)
		cachep++;
	printk(KERN_INFO "The buddy cache size is[%d]\n",cachep->cs_size);
	return cachep->cs_cache;
}
static int obj_size(struct kmem_cache *cachep)
{
	return cachep->buffer_size;
}
static bool buddy_slab_should_failslab(struct kmem_cache *cachep,
		gfp_t flags)
{
	printk(KERN_INFO "[buddy_slab_should_failslab]\n");
	return 0;
}
static inline void buddy_cache_alloc_debugcheck_before(
		struct kmem_cache *cachep,gfp_t flags)
{
	printk(KERN_INFO "[buddy_cache_alloc_debugcheck_before]\n");
}
static void *buddy_cache_alloc_debugcheck_after(
		struct kmem_cache *cachep,gfp_t flags,void *obj,void *caller)
{
	printk(KERN_INFO "[buddy_cache_alloc_debugcheck_after]\n");
	return NULL;
}
static inline void buddy_kmemleak_alloc_recursive(const void *ptr,
		size_t size,int min_count,unsigned long flags,gfp_t gfp)
{

}
static __always_inline void *buddy___cache_alloc(struct kmem_cache *cachep,
		gfp_t flags,void *caller)
{
	unsigned long save_flgas;
	void *objp;

	flags &= gfp_allowed_mask;
	lockdep_trace_alloc(flags);
	if(buddy_slab_should_failslab(cachep,flags))
		return NULL;
	buddy_cache_alloc_debugcheck_before(cachep,flags);
//	local_irq_save(save_flags);
//	objp = __do_cache_alloc(cachep,flags);
//	loacl_irq_restore(save_flags);
	objp = buddy_cache_alloc_debugcheck_after(cachep,flags,objp,caller);
	buddy_kmemleak_alloc_recursive(
			objp,obj_size(cachep),1,cachep->flags,flags);
	prefetchw(objp);
/*
	if(likely(objp))
		kmemcheck_slab_alloc(cachep,flags,objp,obj_size(cachep));
	if(unlikely((flags & __GFP_ZERO) && objp))
		memset(objp,0,obj_size(cachep));
*/
	return objp;
}
static __always_inline void *buddy___do_kmalloc(size_t size,
		gfp_t flags,void *caller)
{
	struct kmem_cache *cachep;
	void *ret;
	/*
	 * If you want to save a few bytes.
	 */
	cachep = buddy___find_general_cachep(size,flags);
	if(unlikely(ZERO_OR_NULL_PTR(cachep)))
		return cachep;
	ret = buddy___cache_alloc(cachep,flags,caller);
	if(ret != NULL)
	{
		printk(KERN_INFO "[buddy__do_kmalloc]get cache\n");
		return ret;
	} else
	{
		printk(KERN_INFO "[buddy__do_kmalloc]Unable to get memory\n");
		return NULL;
	}
}
void *buddy___kmalloc(size_t size,gfp_t flags)
{
	return buddy___do_kmalloc(size,flags,__builtin_return_address(0));
}
#endif
#if CACHE_TEST
static char *buddy_name[] = {"buddy_2","buddy_4","buddy_8","buddy_16",
	"buddy_32","buddy_64","buddy_128","buddy_256",
	"buddy_512","buddy_1024"};
static void buddy_create_cache(struct buddy_cache *p)
{
	int i;
	for(i = 0;i < 9; i++)
		p[i].cs_cache = kmem_cache_create(buddy_name[i],p[i].cs_size,
				SLAB_HWCACHE_ALIGN,0,NULL);
}
#endif


#if _KMALLOC
/*
 * __builtin_constant_p
 */
static void *buddy_kmalloc(size_t size,gfp_t flags)
{
	void *ret = NULL;
	/*
	 * check size is constant value,it will return 1 if it's true,if not
	 * return 0.
	 */
	if(__builtin_constant_p(size))
	{
		int i = 0;
		if(!size)
			return ZERO_SIZE_PTR;/* ((void *)16) */
		/*
		 * check the size of memory is can get.
		 */
	#define CACHE(x) \
		if(size <= x) \
			goto found; \
		else \
			i++; 
	#include <linux/kmalloc_sizes.h>
	#undef CACHE
		return NULL;/* over ths slab size */
	found:
			printk(KERN_INFO "Unable to get memory from trace\n");
			return NULL;
	}
	ret = buddy___kmalloc(size,flags);
	if(ret != 0)
	{
		printk(KERN_INFO "get memory from __kmalloc()\n");
		kfree(ret);
		return NULL;
	}else
	{
		printk(KERN_INFO "Unable to get memory from __kmalloc\n");
		return NULL;
	}
}
#endif
#if TEST
static void _buddy_test(void)
{
#if CACHE_TEST
	buddy_create_cache(buddy_sizes);
#endif
#if _KMALLOC
	buddy_kmalloc(2,GFP_KERNEL);
	buddy_kmalloc(6,GFP_KERNEL);
	buddy_kmalloc(43,GFP_KERNEL);
#endif
}
#endif
/*
 * open operation
 */
static int buddy_open(struct inode *inode,struct file *filp)
{
#if TEST
	_buddy_test();
#endif
	printk(KERN_INFO "Open device\n");
	return 0;
}
/*
 * read operation
 */
static ssize_t buddy_read(struct file *filp,char __user *buf,size_t count,loff_t *offset)
{
	return 0;
}
/*
 * release operation
 */
static ssize_t buddy_write(struct file *filp,const char __user *buf,size_t count,loff_t *offset)
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
