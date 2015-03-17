/*
 * Copyright@2014 Buddy Zhang
 * This content about alloc memory in Linux kernel.
 */
===========================================================================
1. alloc_page 
	struct page *alloc_page(unsigned int gfp_mask)
	alloc a physic memory and return a point for this page.
	[definition]============================================
	#define alloc_page(gfp_mask) alloc_pages(gfp_mask,0)
	static inline struct page *alloc_pages(gfp_t gfp_mask,
			unsigned int order)
	{
		return alloc_pages_current(gfp_mask,order);
	}
	struct page *alloc_page_current(gfp_t gfp,unsigned order)
	{
		struct mempolicy *pol = current->mempolicy;
		struct page *page;

		if(!pol || in_interrupt() || (gfp & __GFP_THISNODE))
			pol = &default_policy;
		get_mems_allowed();
		/*
		 * No reference counting needed for current->mempolicy
		 * nor system default_policy
		 */
		if(pol->mode == MPOL_INTERLEAVE)
			page = alloc_page_interleave(gfp,order,interleave_nodes(pol));
		else
			page = __alloc_pages_nodemask(gfp,order,
					policy_zonelist(gfp,pol,numa_node_id()),
					policy_nodemask(gfp,pol));
		put_mems_allowed();
		return page;
	}
	[Explain]==================================================
	The alloc_page() is sub-function of alloc_pages().Use same flags and
	requirs one page from alloc_pags().The alloc_pages() will entry into 
	alloc_pages_current().In function of alloc_pages_current(),first,it
	will get current task's paga pool.When not in interrupt context and 
	apply the current process NUMA policy.it will return Null when no page
	can be alloced.
	When no reference counting need for current->mempolicy and nor system 
	default_policy,it will call get_mems_allowed() and put_mems_allowed() to
	get a new page.If the mode of page is MPOL_INTERLEAVE,the kernel wil call
	alloc_page_interleave() to get a new page,otherwise it will call
	__alloc_pages_nodemask() to get a new page.
	The definition of gfp:
	GFP_USER:        user allocation
	GFP_KERNEL:      kernel allocation
	GFP_HIGHMEM:     highmem allocation
	GFP_FS:          don't call back into a file system.
	GFP_ATOMIC:      don't sleep
	[example]===================================================
	#include <linux/slab.h>
	
	struct page *page;
	page = alloc_page(GFP_KERNEL);
	__free_page(page);
==========================================================================
2. alloc_pages
	struct page *alloc_pages(unsigned int gfg_mask,unsigned int order);
	This function alloc countinuous physic memory and return a point for
	fist page address.
	[explain]=============================================================
	The element of "order" is not a randam number,such as order is 4,kernel
	will alloc 16 pages.
	[example]=============================================================
	#include <linux/slab.h>

	struct page *page;
	page = alloc_pages(GFP_KERNEL,2);
	__free_pages(page,2);
==========================================================================
3.__get_free_page
	unsigned long __get_free_page(unsigned int gfp_mask);
	This function will alloc one physic page and return a virtual address
	[definition]==========================================================
	#define __get_free_page(gfp_mask) __get_free_pages((gfp_mask),0)
	unsigned long __get_free_pages(gfp_t gfp_mask,unsigned int order)
	{
		struct page *page;
		/*
		 * __get_free_pages() return a 32-bit address,which cannot represet
		 * a higmen page
		 */
		VM_BUG_ON((gfp_mask & __GFP_HIGHMEM) != 0);
		page = alloc_pages(gfp_mask,order);
		if(!page)
			return 0;
		return (unsigned long)page_address(page);
	}
	void *page_address(struct page *page)
	{
		unsigned long flags;
		void *ret;
		struct page_address_slot *pas;

		if(!PageHighMem(page))
			return lowmem_page_address(page);
		pas = page_slot(page);
		ret = NULL;
		spin_lock_irqsave(&pas->lock,flags);
		if(!list_empty(&pas->lh)) 
		{
			struct page_address_map *pam;
			list_for_each_entry(pam,&pas->lh,list)
			{
				if(pam->page == page)
				{
					ret = pam->virtual;
					goto done;
				}
			}
		}
	done:
		spin_unlock_irqrestore(&pas->lock,flags);
		return ret;
	}
	static __always_inline void *lowmem_page_address(struct page *page)
	{
		return __va(PFN_PHYS(page_to_pfn(page)));
	}
	#define __va(x) ((void *)((unsigned long)(x) | 0xc0000000))
	[explain]=============================================================
	__get_free_page() will entry __get_free_pages().In __get_free_pages(),
	It won't get free page from HIGHMEM.It use page_address() to get page
	virtual address.If the address is lowmem in page_address(),it will call
	lowmem_page_address(),otherwise kernel will alloc new virtual address
	from lowmem to highmem physic page.
	It's simple to get virtual address for lowmem,it add 0xc0000000 to 
	generate a virtual address(In 4G virtual adrress,the first 3G for use
	and the last 1G for kernel).Use __va() to get virtual address.
	[example]=============================================================
	#include <linux/slab.h>

	unsigned long *address;
	address = (unsigned long *)__get_free_page(GFP_KERNEL);
	free_page((unsigned long)address);
==========================================================================
4.__get_free_pages
	unsigned long __get_free_pages(unsigned int gfp_mask,
			unsigned int order)
	Allocates 2^order number of pages and return a virtual address.
	[example]=============================================================
	#include <linux/slab.h>

	unsigned long *address;
	address = (unsigned long *)__get_free_pages(GFP_KERNEL,2);
	free_pages((unsigned long)address,2);
==========================================================================
5.__get_dma_pages
	struct page *__get_dma_pages(unsigned int gfp_mask,unsigned int order);
	The function will allocate 2^order number of page from DMA zone and
	return a point to page address.
	[definiation]=========================================================
	#define __get_dma_pages(gfp_mask,order) \
		__get_free_pages((gfp_mask) | GFP_DMA,(order))
	[example]=============================================================
	#include <linux/slab.h>

	unsigned long *address;
	address = (unsigned long *)__get_dma_pages(GFP_KERNEL,2);
	free_pages((unsigned long)address,2);
==========================================================================
	These functions allocate memory by Buddy system.They are basic function
	of allocation memory in Linux kernel.The maxminu of allocation are
	MAX_ORDER.
==========================================================================
6.__free_pages
	void __free_pages(struct page *page,unsigned int order);
	This function is free order number of pages from the given page.
	[definiation]=========================================================
	void __free_pages(struct page *page,unsigned int order)
	{
		if(put_page_testzero(page)) 
		{
			if(order == 0)
				free_hot_cold_page(page,0);
			else
				__free_pages_ok(page,order);
		}
	}
	static inlint int put_page_testzero(struct page *page)
	{
		VM_BUG_ON(atomic_read(&page->_count) == 0);
		return atomic_dec_and_test(&page->_count);
	}
==========================================================================
7.__free_page
	void __free_page(struct page *page);
	Free single page.
==========================================================================
8.free_page(void *addr)
	void free_page(void *addr);
	Free a page by a virtual address.
	[definiation]=========================================================
	#define free_page(addr) free_pages((addr),0)
	void free_pages(unsigned long addr,unsigned int order)
	{
		if(addr != 0)
		{
			VM_BUG_ON(!virt_addr_valid((void *)addr));
			__free_pages(virt_to_page((void *)addr),order);
		}
	}
	#define virt_to_page(p) ((struct page *)((virt_to_phys(p)/4096) * \
				sizeof(struct page)))
	#define virt_to_phys(a) ((unsigned long)__pa(a))
	#define __pa(x) ((unsigned long)(x) & 0x3fffffff)
	[explain]=============================================================
	This function will free a virtual address,the virtual address will 
	transfer to physic address in underlying system.
	Both use __free_pages(),it need a point for struct of page,so we need
	a page from virtual addree by virt_to_page().
==========================================================================
9.struct page;
	struct page {
		/*
		 * Atomic flags,some possible updated asynchronously
		 */
		unsigned long flags;
		/*
		 * Usage count
		 */
		atomic_t _count;
		union {
			/*
			 * Count of ptes mapped in mms,to show when page is 
			 * mapped & limit reverse map searches.
			 */
			atomic_t _mapcount;
			struct {
				/*
				 * SLUB
				 */
				u16 inuse;
				u16 objects;
			};
		};
		union {
			struct {
				/*
				 * Mapping-private opaque data,usually used for buffer_head
				 * if PagePrivate set.used for swp_entry_t if PageSwapCache
				 * indicates order in the buddy system if PG_buddy is set.
				 */
				unsigned long private;
				/*
				 * If low bit clear,points to inode address_space,or NULL
				 * If page mapped as anonymous memory,low bit is set,and
				 * it points to anon_vma object.
				 */
				struct address_space *mapping;
			};
			/*
			 * SLUB:point to slab
			 */
			struct kmem_cache *slab;
			/*
			 * Compound tail pages
			 */
			struct page *first_page;
		}
		union {
			/*
			 * Our offset within mapping
			 */
			pgoff_t index;
			/*
			 * SLUB:freelist req,slab lock
			 */
			void *freelist;
		};
		struct list_head lru;
		/*
		 * On machines where all RAM is mapped into kernel address space,
		 * we can simply calculate the virtual address.On machines with
		 * highmem some memory is mapped into kernel virtual memory
		 * dynamically,so we need a place to store that address.Note that
		 * this field could be 16 bits on x86.
		 */
		defined(WANT_PAGE_VIRTUAL)
		/*
		 * Kernel virtual address
		 */
			void *virtual;
	}
==========================================================================
10.kmalloc,kzalloc,kcalloc
	void *kmalloc(size_t size,gfp_t flags);
	void *kzalloc(size_t size,gfp_t flags);
	void *kcalloc(size_t n,size_t size,gfp_t flags);
	[definiation]=========================================================
	static __always_inline void *kmalloc(size_t size,gfp_t flags)
	{
		struct kmem_cache *cachep;
		void *ret;

		if(__builtin_constant_p(size))
		{
			int i = 0;
			if(!size)
				return ZERO_SIZE_PTR;
	#define CACHE(x) \
			if(size < = x) \
				goto found; \
			else \
				i++;
	#include <linux/kmalloc_sizes.h>
	#undef CACHE
			return NULL;
	found:
			ret = kmem_cache_alloc_trace(size,cachep,flags);
			return ret;
		}
		return __kmalloc(size,flags);
	}
	void *__kmalloc(size_t size,gfp_t flags)
	{
		return __do_kmalloc(size,flags,__builtin_return_address(0));
	}
	static __always_inline void *__do_kmalloc(size_t size,gfp_t flags,
			void *caller)
	{
		struct kmem_cache *cachep;
		void *ret;
		/*
		 * If you want to save a few bytes.text space:replace __with kmem_
		 * Then kmalloc uses the uninlined functions instead of the inline
		 * functions.
		 */
		cachep = __find_general_cachep(size,flags);
		if(unlikely(ZERO_OR_NULL_PTR(cachep)))
			return cachep;
		ret = __cache_alloc(acachep,flags,caller);
		trace_kmalloc((unsigned long)caller,ret,
				size,cachep->buffer_size,flags);
		return ret;
	}
	static __always_inline void *__cache_alloc(struct kmem_cache *cache,
			gfp_t flags,void *caller)
	{
		unsigned long save_flags;
		void *objp;
		flags &= gfp_allowed_mask;
		lockdep_trace_alloc(flags);
		if(slab_should_failslab(cache,flags))
			return NULL;
		cache_alloc_debugcheck_before(cachep,flags);
		local_irq_save(save_flags);
		objp = __do_cache_alloc(cachep,flags);
		local_irq_restore(save_flags);
		objp = cache_alloc_debugcheck_after(cacahe,flags,objp,caller);
		kmemleak_alloc_recursive(objp,obj_size(cachep),1,cachep->flags,
				flags);
		prefetchw(objp);
		if(likely(objp))
			kmemcheck_slab_alloc(cachep,flags,objp,obj_size(cachep));
		if(unlickely((flags & __GFP_ZERO) && objp))
			memset(objp,0,obj_size(cachep));
		return objp;
	}
	[explain]============================================================
	1.__builtin_contant_p(x)
	I belive that this asks the compiler whether its argument evaluates
	to a constant value.For instance,__builtin_constant_p(1234)has a value
	of 1,whereas __builtin_constant_p(x),where 'x' is a variable,has a
	value of 0.This is sometimes useful when writing highly-optimized code.
	2. #define CACHE(x)
	   #include <linux/kmalloc_sizes.h>
	   #undef
	This methods in order to check the size of alloc otherwise over size.
	3.__builtin_return_address(0)
	It will return a address of current address for call-function,in this
	system,it must be 0.

























