#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>

#include <linux/mmzone.h>
#include <linux/nodemask.h>
#include <linux/numa.h>

#define DEV_NAME "buddy_test"

/*
 * first_online_pgdat
 */
struct pglist_data *first_online_pgdat(void)
{
	return NODE_DATA(first_online_node);
}
/*
 * next_online_pgdat
 */
struct pglist_data *next_online_pgdat(struct pglist_data *pgdat)
{
	int nid = next_online_node(pgdat->node_id);
	if(nid == MAX_NUMNODES)
		return NULL;
	return NODE_DATA(nid);
}
/*
 * next_zone
 */
struct zone *next_zone(struct zone *zone)
{
	pg_data_t *pgdat = zone->zone_pgdat;
	if(zone < pgdat->node_zones + MAX_NR_ZONES - 1)
		zone++;
	else {
		pgdat = next_online_pgdat(pgdat);
		if(pgdat)
			zone = pgdat->node_zones;
		else
			zone = NULL;
	}
	return zone;
}
/*
 * Free are
 */
static void buddy_free_nr(struct free_area *a)
{
	int i;
	for(i = 0; i < 11;i++)
		printk(KERN_INFO "The free number are[%ld]\n",a[i].nr_free);
}
/*
 * Get zone infomation
 */
static void buddy_zone_info(struct zone *z)
{
	printk(KERN_INFO "======================================\n");
	printk(KERN_INFO "The Zone name is[%s]\n",z->name);
	printk(KERN_INFO "The Zone strat pfn is[%08x]\n",z->zone_start_pfn);
	printk(KERN_INFO "The Zone size are[%ld]\n",z->spanned_pages);
	printk(KERN_INFO "The Zone amount of memory are[%ld]",z->present_pages);
	printk(KERN_INFO "The Zone flags are[%ld]\n",z->flags);
	printk(KERN_INFO "Since last reclaim are[%ld]",z->pages_scanned);
#ifdef CONFIG_NUMA
	printk(KERN_INFO "The min unmapped pages are[%ld]",z->min_unmapped_pages);
	printk(KERN_INFO "The min slab pages are[%ld]",z->min_slab_pages);
#endif
	printk(KERN_INFO "The number for can't reclaim[%d]",z->all_unreclaimable);
	printk(KERN_INFO "The wait task are[%ld]\n",z->wait_table_hash_nr_entries);
	printk(KERN_INFO "The wait table bits are[%ld]\n",z->wait_table_bits);
	buddy_free_nr(z->free_area);
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
	struct zone *zone;
	printk(KERN_INFO "read device\n");
	for_each_zone(zone)
	{
		printk(KERN_INFO "For each zone.\n");
		if(is_highmem(zone))
			printk(KERN_INFO ">The zone in high_memory");
		if(is_normal(zone))
			printk(KERN_INFO ">The Zone in normal memory");
		buddy_zone_info(zone);
	}
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
