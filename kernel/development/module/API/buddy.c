#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>

#include <linux/list.h>
#define DEV_NAME "buddy_test"
#include <linux/kallsyms.h>

static int address;
















/*
 * Get a module via itself name.
 */
static void buddy_get_module(void)
{
	struct module *md,*tmp;
	md = THIS_MODULE;
	tmp = find_module("buddy");
	printk(KERN_INFO "THIS_MODULE[%s]\n"
			"get module[%s]\n",
			md->name,tmp->name);
}
/*
 * Find a module via symbol name then decare one for counter of module.
 */
static void buddy_symbol_put(void)
{
	struct module *md = THIS_MODULE;
	__symbol_put("buddy_sshow");
	printk(KERN_INFO "refcount[%d]\n",module_refcount(md));
	__symbol_put("buddy_sshow");
	printk(KERN_INFO "refcount[%d]\n",module_refcount(md));
	__symbol_put("buddy_sshow");
	printk(KERN_INFO "refcount[%d]\n",module_refcount(md));
	__symbol_put("buddy_sshow");
	printk(KERN_INFO "refcount[%d]\n",module_refcount(md));
}
/*
 * Get address for kernel symbol via kernel-symbol
 */
static void buddy_sshow(void)
{
	printk(KERN_INFO "Buddy show\n");
}
EXPORT_SYMBOL(buddy_sshow);
static void (*buddy_sh)(void);
static void buddy_symbol_gets(void)
{
	buddy_sh = __symbol_get("buddy_sshow");
	buddy_sh();
}
/*
 * lookup a kernel symbol
 */
static int buddy_a()
{
	return 0;
}
EXPORT_SYMBOL(buddy_a);
static const char *(*kallsyms_lookups)(unsigned long,
		unsigned long *,
		unsigned long *,
		char **,char *) = 0xc01af2d0;
static void buddy_kallsyms_lookup(void)
{
	unsigned long addr = 0xc0514a00;
	unsigned long md_core,size,offset;
	char *mdname;
	struct module *md = THIS_MODULE;
	const char *name;

	dump_stack();
	name = kallsyms_lookups(addr,&size,&offset,&mdname,"%s");
	if(name != NULL)
		printk(KERN_INFO "Succeed \n");
	else
		printk(KERN_INFO "Can't lookup the symbol form kernel\n");
}
EXPORT_SYMBOL(buddy_kallsyms_lookup);
/*
 * Get symbol position
 */
static unsigned long (*get_symbol_pos)(unsigned long,
		unsigned long *,unsigned long *) =0xc01af074;
static void buddy_get_symbol_pos(void)
{
	unsigned long addr  = 0xc0514a00;
	unsigned long addr1 = 0xc0502668;
	unsigned long symsize;
	unsigned long offset;
	unsigned long ret;
	unsigned long ret1;
	ret  = get_symbol_pos(addr,&symsize,&offset);
	ret1 = get_symbol_pos(addr1,&symsize,&offset); 
	printk(KERN_INFO 
			"ret[%x]\n"
			"symsize[%ld]\n"
			"offset[%ld]\n"
			"ret1[%x]\n",
			ret,symsize,offset,ret1);
}
/*
 * Area check. 
 */
static int (*is_ksym_addr)(unsigned long) = 0xc01af048;
static unsigned long (*get_symbol_poss)(unsigned long addr,
		unsigned long *,unsigned long *) = 0xc01af074;
static unsigned int (*get_symbol_offset)(unsigned long)
	= 0xc01aed2c;
static void buddy_pos(unsigned long addr);
static void buddy_is_ksym_addr(unsigned long addr)
{
	printk(KERN_INFO "The addr[%x]",addr);
	if(is_ksym_addr(addr))
	{
		printk(KERN_INFO " is kernel symbol address\n");
		buddy_pos(addr);
	}
	else
		printk(KERN_INFO " isn't kernel symbol address\n");
}
static void buddy_pos(unsigned long addr)
{
	unsigned long symb_size,offset,pos;
	int off;
	pos = get_symbol_poss(addr,&symb_size,&offset);
	off = get_symbol_offset(pos);
	printk(KERN_INFO 
			"addr[%lx]\n"
			"positon[%x]\n"
			"symbol size[%ld]\n"
			"offset[%ld]\n"
			"new off[%d]\n",
			addr,pos,symb_size,offset,off);
}
static void buddy_area(void)
{
	unsigned long u_addr = 0x01111111;
	unsigned long k_addr = buddy_area;
	unsigned long sym_addr = 0xc01af048;
	
	buddy_is_ksym_addr(u_addr);
	buddy_is_ksym_addr(k_addr);
	buddy_is_ksym_addr(sym_addr);
}
EXPORT_SYMBOL(buddy_area);
/*
 * print a symbol via a virtual address.
 */
int a_symbol()
{
	return 1;
}
EXPORT_SYMBOL(a_symbol);
static void buddy_print_symbol(void)
{
	unsigned long addr = 0xc0514a00;
	char *fmt;
	char *name;
	struct module *md = NULL;
	struct module *tmp = THIS_MODULE;
	int count = 50,i = 0;
	fmt = "it's the first part\n%s";
	__print_symbol(fmt,addr);
	printk("\n");
	name = tmp->name;
	md = find_module(name);
	if(md != NULL)
	{
		printk(KERN_INFO "module name are[%s]\n",md->name);
		addr = (unsigned long)md->module_core;
		fmt  = "it's the second part\n%s\n";
		__print_symbol(fmt,addr);
	}
	printk("\n");
	addr = (unsigned long)a_symbol + 5;
	fmt  = "it's the third part\n%s";
	__print_symbol(fmt,addr);
	printk("\n");
	__print_symbol("%s",md->module_core + 8);
	printk("\n");
	__print_symbol("%s",md->module_core + 25);
	printk("\n");
	__print_symbol("%s",md->module_core + 31);
	printk("\n");
	__print_symbol("%s",md->module_core + 32);
	printk("\n");
	__print_symbol("%s",md->module_core + 40);
	printk("\n");
	__print_symbol("%s",md->module_core + 48);
	printk("\n");
	__print_symbol("%s",md->module_core + 49);
	printk("\n");
	__print_symbol("%s",md->module_core + 55);
	while(count--)
	{
		__print_symbol("%s",md->module_core + i);
		i++;
		printk("\n");
	}
	return 0;
}
EXPORT_SYMBOL(buddy_print_symbol);
/*
 * Get a symbol via a string.
 */
static void buddy_symbol_get(void)
{
	char *name = "printk";
	unsigned long *addr = (unsigned long *)__symbol_get(name);
	if(addr == NULL)
		printk(KERN_INFO "Can't get address for printk\n");
	else
		printk(KERN_INFO "Success to get address for printk are[%x]\n",
				addr);
}
/*
 * Get a module via virtual address.
 */
static void buddy_module(void)
{
	unsigned long addr = (unsigned long)0xc0514a00;
	struct module *ma,*mb;
	preempt_disable();
	ma = __module_address(addr);
	mb = __module_text_address(addr);
	preempt_enable();
	printk(KERN_INFO "ma[%s]\n"
			"mb[%s]\n",
			ma->name,mb->name);
}
/*
 * Judging the address whether is text segment in module.
 */
static bool (*is_module_text_addr)(unsigned long) = 0xc01aeba4;
static void buddy_is_module_text_address(void)
{
	unsigned long test_addr = (unsigned long)&address;
	unsigned long addr = (unsigned long)buddy_is_module_text_address;
	struct module *md = THIS_MODULE;
	if(is_module_text_addr(addr))
		printk(KERN_INFO "address in module text\n");
	else
		printk(KERN_INFO "address not in module text\n");
	printk(KERN_INFO "test[%d][%d]\n",
			within_module_init(test_addr,md),
			within_module_core(test_addr,md));
	printk(KERN_INFO "addr[%x]core[%x~%x]init[%x~%x]\n",
			test_addr,
			md->module_core,md->module_core + md->core_size,
			md->module_init,md->module_init + md->init_size);
}
/*
 * Show information for module
 */
static void buddy_show_info(void)
{
	struct module *md = THIS_MODULE;
	printk(KERN_INFO 
			"module name are[%s]\n"
			"module start virtual address[%x]\n"
			"module end virtual address[%x]\n"
			"module size [%d]\n"
			"module init size[%d]core size[%d]\n"
			"module size[%d]\n"
			"module init text size[%d]\n"
			"module core text size[%d]\n"
			"module text size [%d]\n"
			"module init ro size[%d]\n"
			"module core ro size[%d]\n"
			"module ro size[%d]\n"
			"total size are[%d]\n"
			,md->name,
			md->module_init,md->module_core,
			md->module_core - md->module_init,
			md->init_size,md->core_size,
			md->core_size - md->init_size,
			md->init_text_size,md->core_text_size,
			md->core_text_size - md->init_text_size,
			md->init_ro_size,md->core_ro_size,
			md->core_ro_size - md->init_ro_size,
			(md->core_text_size - md->init_text_size) +
			(md->core_ro_size - md->init_ro_size));
	printk(KERN_INFO "The text size in init[%x~%x]\n"
			"The text size in core[%x~%x]\n",
			md->module_init,md->module_init + md->init_text_size,
			md->module_core,md->module_core + md->core_text_size);
}
/*
 * Judge address whether within rate
 */
static inline int within(void *addr,void *start,unsigned long size)
{
	return ((addr >= start) && (addr < start + size));
}
static void buddy_within(void)
{
	struct module *md = THIS_MODULE;
	unsigned long addr = (unsigned long)buddy_within;
	int ret;
	printk(KERN_INFO "The init addr[%x][%x][%d][%d]\n",
			addr,md->module_init,md->init_text_size,md->core_text_size);
	ret = within(addr,md->module_init,md->init_text_size);
	if(ret)
		printk(KERN_INFO "The address within text segment\n");
	else
		printk(KERN_INFO "The address not within text segment\n");
}
/*
 * get a module via a address
 */
static void buddy_module_text_address(void)
{
	struct module *tmp;
	unsigned long paddr = (unsigned long)&address;
	unsigned long addr = (unsigned long)buddy_module_text_address;
	struct module *md = __module_text_address(paddr);
	if(md == NULL)
		printk(KERN_INFO "NULL\n");
	if(!IS_ERR(md))
		printk(KERN_INFO "The module name are[%s]\n",md->name);
	tmp = __module_text_address(THIS_MODULE);
	if(!IS_ERR(tmp))
		printk(KERN_INFO "The new module are [%s]\n",tmp->name);
	tmp = __module_address(addr);
	if(!IS_ERR(tmp))
		printk(KERN_INFO "The new name [%s]\n",tmp->name);
	tmp = __module_address(THIS_MODULE);
	if(!IS_ERR(tmp))
		printk(KERN_INFO "The j name are[%s]\n",tmp->name);
}
/*
 * get cpu id
 */
static void buddy_get_cpu(void)
{
	int cpu = get_cpu();
	printk(KERN_INFO "The cpu are[%d]\n",cpu);
	put_cpu();
	BUG_ON(1);
}
/*
 * THIS_MODULE
 */
static void buddy_THIS_MODULE(void)
{
	struct module *md = THIS_MODULE;
	struct module *mm = &__this_module;

	printk(KERN_INFO "The name are[%s][%s]\n",
			md->name,mm->name);
}
/*
 * scan module list
 */
static struct module *modules = 0xc0722c04;
static void buddy_list_for_module(void)
{
	struct module *md;
	list_for_each_entry_rcu(md,&modules->list,list)
	{
		printk(KERN_INFO "Module name are[%s]\n",
				md->name);
	}
}
/*
 * Get a module via address.
 */
static unsigned long module_addr_min = -1UL,module_addr_max = 0;
static void buddy_module_address(void)
{
	struct module *md;
	unsigned long addr = (unsigned long)&address;
	preempt_enable();
	printk(KERN_INFO "module_addr_min[%ld,%x]\n"
			"module_addr_max[%ld,%x]\n"
			"1UL[%ld,%x]"
			"module address are[%x]\n",
			module_addr_min,module_addr_min,
			module_addr_max,module_addr_max,
			1UL,1UL,addr);
	if(addr < module_addr_min)
		printk(KERN_INFO "NULL\n");
	if(addr > module_addr_max)
		printk(KERN_INFO "NO\n");
	preempt_enable();
	preempt_disable();
	md = __module_address(addr);
	preempt_enable();
	if(!IS_ERR(md))
		printk(KERN_INFO 
				"module name are[%s]\n"
				"module state are[%d]\n"
				"module core_size[%d]\n"
				"module ref %d\n",
				md->name,md->state,md->core_size,module_refcount(md));
}
/*
 * open operation
 */
static int buddy_open(struct inode *inode,struct file *filp)
{
	unsigned long addr = (unsigned long)buddy_open;
	printk(KERN_INFO "Open device[%x]\n",addr);
//	buddy_module_address();
//	buddy_list_for_module();
//	buddy_THIS_MODULE();
//	buddy_get_cpu();
//	buddy_module_text_address();
//	buddy_within();
//	buddy_show_info();
//	buddy_is_module_text_address();
//	buddy_module();
//	buddy_symbol_get();
//	buddy_print_symbol();
//	buddy_area();
//	buddy_get_symbol_pos();
//	buddy_kallsyms_lookup();
//	buddy_symbol_gets();
//	buddy_symbol_put();
	buddy_get_module();
	return 0;
}
/*
 * read operation
 */
static ssize_t buddy_read(struct file *filp,const char __user *buf,
		size_t count,loff_t *offset)
{
	printk(KERN_INFO "read\n");
	return 0;
}
/*
 * release operation
 */
static int buddy_release(struct inode *inode,struct file *filp)
{
	printk(KERN_INFO "release\n");
	return 0;
}
/*
 * write operation
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
	.read      = buddy_read,
	.release   = buddy_release,
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
	int a = 10;
	struct module *md = THIS_MODULE;
	unsigned long addr = (unsigned long)&a;
	printk(KERN_INFO "test[%d][%d]\n",
			within_module_init(addr,md),
			within_module_core(addr,md));
	printk(KERN_INFO "addr[%x]init[%x~%x]core[%x~%x]\n",
			addr,
			md->module_init,md->module_init + md->init_size,
			md->module_core,md->module_core + md->core_size);

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
