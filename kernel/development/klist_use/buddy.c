#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/slab.h>

#define DEV_NAME "buddy_test"

#define TEST         1
#define LIST         1

void debug(char *string)
{
	printk("[%s]\n",string);
}
#if LIST
struct buddy_list_head {
	struct buddy_list_head *next,*prev;
	int test;
};
static inline void buddy_init_list_head(struct buddy_list_head *list)
{
	list->next = list;
	list->prev = list;
	debug("buddy_init_list_head");
}
static inline void _buddy_list_add(struct buddy_list_head *list,
		struct buddy_list_head *prev,
		struct buddy_list_head *next)
{
	next->prev = list;
	prev->next = list;
	list->prev = prev;
	list->next = next;
}
static inline void buddy_list_add(struct buddy_list_head *list,
		struct buddy_list_head *head)
{
	_buddy_list_add(list,head,head->next);
}
static inline void buddy_list_add_tail(struct buddy_list_head *list,
		struct buddy_list_head *head)
{
	_buddy_list_add(list,head->prev,head);
}
static inline void _buddy_list_del(struct buddy_list_head *prev,
		struct buddy_list_head *next)
{
	next->prev = prev;
	prev->next = next;

}
static inline void _buddy_list_del_entry(struct buddy_list_head *entry)
{
	_buddy_list_del(entry->prev,entry->next);
}
static inline void buddy_list_del(struct buddy_list_head *entry)
{
	_buddy_list_del(entry->prev,entry->next);
	entry->next = NULL;
	entry->prev = NULL;
}
static inline void buddy_list_replace(struct buddy_list_head *old,
		struct buddy_list_head *new)
{
	new->next = old->next;
	new->prev = old->prev;
	new->next->prev = new;
	new->prev->next = new;
}
static inline void buddy_list_replace_init(struct buddy_list_head *old,
		struct buddy_list_head *new)
{
	buddy_list_replace(old,new);
	buddy_init_list_head(old);
}
static inline void buddy_list_del_init(struct buddy_list_head *old)
{
	_buddy_list_del(old->prev,old->next);
	buddy_init_list_head(old);
}
static inline void buddy_list_move(struct buddy_list_head *entry,
		struct buddy_list_head *new)
{
	_buddy_list_del_entry(entry);
	buddy_list_add(entry,new);
}
static inline void buddy_list_move_tail(struct buddy_list_head *list,
		struct buddy_list_head *new)
{
	_buddy_list_del_entry(list);
	buddy_list_add_tail(list,new);
}
static inline int buddy_list_is_last(struct buddy_list_head *list,
		struct buddy_list_head *head)
{
	return list == head->prev;
}
static inline int buddy_list_empty(struct buddy_list_head *head)
{
	return head->next == head;
}
static inline int buddy_list_empty_careful(
		const struct buddy_list_head *head)
{
	struct buddy_list_head *next = head->next;
	return (next == head) && (next == head->prev);
}
static inline void buddy_list_rotate_left(struct buddy_list_head *head)
{
	struct buddy_list_head *first;
	if(!buddy_list_empty(head))
	{
		first = head->next;
		buddy_list_move_tail(first,head);
	}
}
#endif

#if TEST
static void buddy_create(struct buddy_list_head *head,int i)
{
	struct buddy_list_head *new[i];
	while(i--)
	{
		new[i] = (struct buddy_list_head *)kmalloc(
				sizeof(struct buddy_list_head),GFP_KERNEL);
		if(new[i] == NULL)
		{
			printk(KERN_INFO "No memory to create value\n");
		}
		printk(KERN_INFO "Succeed to create a new value\n");
		new[i]->test = i + 10;
		buddy_list_add(new[i],head);
	}
}
static void buddy_test_show(struct buddy_list_head *head)
{
	struct buddy_list_head *list = head->next;
	while(list->test != 9999)
	{
		printk(KERN_INFO "The list number is[%d]\n",list->test);
		list = list->next;
	}
}
/*
 * Test API
 */
static void test(void)
{
#if LIST
	struct buddy_list_head head;
	head.test = 9999;
	buddy_init_list_head(&head);
	printk(KERN_INFO "list start head->test:%4d\n",head.test);
	buddy_create(&head,12);
	buddy_test_show(&head);
#endif
}
#endif
/*
 * open operation
 */
static int buddy_open(struct inode *inode,struct file *filp)
{
	/*
	 * Create a dentry
	 */
#if TEST
	test();
#endif 
	
	printk(KERN_INFO "Open device\n");
	return 0;
}
/*
 * release operation
 */
static ssize_t buddy_write(struct file *filp,char __user *buf,size_t count,loff_t *offset)
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
