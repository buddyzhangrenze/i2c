#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#define STOP   9999
static int count = 20;

/*
 * Basic struct - list
 */
struct buddy_list {
	struct buddy_list *next,*prev;
	int data;
};
struct buddy_dev {
	int data;
	struct buddy_list *head;
	struct buddy_list list;
};

/*
 * init list
 */
static void buddy_list_init(struct buddy_list *head)
{
	head->next = head;
	head->prev = head;
}
/*
 * list add
 */
static void _buddy_list_add(struct buddy_list *new,
		struct buddy_list *prev,struct buddy_list *next)
{
	new->prev = prev;
	new->next = next;
	next->prev = new;
	prev->next = new;
}
static void buddy_list_add(struct buddy_list *new,struct buddy_list *head)
{
	_buddy_list_add(new,head,head->next);
}
static void buddy_list_add_tail(struct buddy_list *new,struct buddy_list *head)
{
	_buddy_list_add(new,head->prev,head);
}
/*
 * list del
 */
static void _buddy_list_del(struct buddy_list *prev,struct buddy_list *next)
{
	prev->next = next;
	next->prev = prev;
}
static void buddy_list_del(struct buddy_list *head)
{
	_buddy_list_del(head->prev,head->next);
}
/*
 * create list
 */
static void buddy_create(struct buddy_list *head,int i)
{
	struct buddy_list *alin[i];
	while(i--)
	{
		alin[i] = (struct buddy_list *)malloc(sizeof(struct buddy_list));
		if(alin[i] == NULL)
		{
			printf("Unable to get memory from user\n");
			continue;
		}
		alin[i]->data = count++;
		buddy_list_add(alin[i],head);
	}
}
static void buddy_del(struct buddy_list *head)
{
	struct buddy_list *tmp,*list = head->next;
	while(list->data != STOP)
	{
		buddy_list_del(list);
		tmp = list;
		list = list->next;
		free(tmp);
	}
}
static void buddy_show(struct buddy_list *head)
{
	struct buddy_list *list = head->next;
	printf("||->");
	while(list->data != STOP)
	{
		printf("%d->",list->data);
		list = list->next;
	}
	printf("\n");
}
static void buddy_list_del_init(struct buddy_list *head)
{
	buddy_list_del(head);
	buddy_list_init(head);
}
/*
 * list head replace
 */
static void buddy_list_replace(struct buddy_list *old,
		struct buddy_list *new)
{
	new->prev = old->prev;
	new->next = old->next;
	new->prev->next = new;
	new->next->prev = new;
}
static void buddy_list_replace_init(struct buddy_list *old,
		struct buddy_list *new)
{
	buddy_list_replace(old,new);
	buddy_list_init(old);
}
/*
 * move
 */
static void buddy_list_move(struct buddy_list *list,
		struct buddy_list *head)
{
	buddy_list_del(list);
	printf("The list data is %d\n",list->data);
	buddy_list_add(list,head);
}
static void buddy_list_move_tail(struct buddy_list *list,
		struct buddy_list *head)
{
	buddy_list_del(list);
	buddy_list_add_tail(list,head);
}
/*
 * get a element
 */
static struct buddy_list *buddy_list_get(int data,
		struct buddy_list *head)
{
	struct buddy_list *list = head->next;
	while(list->data != data)
		list = list->next;
	printf("The data find is %d\n",list->data);
	return list;
}
/*
 * list is last
 */
static int buddy_list_is_last(struct buddy_list *list,
		const struct buddy_list *head)
{
	return list->next == head;
}
/*
 * list empty
 */
static int buddy_list_empty(struct buddy_list *head)
{
	return head->next == head;
}
/*
 * rotate left
 */
static void buddy_rotate_left(struct buddy_list *head)
{
	struct buddy_list *first;
	if(!buddy_list_empty(head))
	{
		first = head->next;
		buddy_list_move_tail(first,head);
	}
}
/*
 * list cut position
 */
static void _buddy_list_cut_position(struct buddy_list *list,
		struct buddy_list *head,struct buddy_list *entry)
{
	struct buddy_list *first = entry->next;

	list->next = head->next;
	list->next->prev = list;

	list->prev  = entry;
	entry->next = list;

	head->next  = first;
	first->prev = head; 

}
/*
 * list singular
 */
static int buddy_list_singular(struct buddy_list *head)
{
	return !buddy_list_empty(head) && (head->next == head);
}
/*
 * cut position - cut a list into two
 * @list:a new list to add all remove entries
 * @head:a list with entries
 * @entry:an entry within head,could be the head itself
 * and if so we won't cut the list.
 */
static void buddy_list_cut_position(struct buddy_list *list,
		struct buddy_list *head,struct buddy_list *entry)
{
	/*
	 * empyt list can't cut into two list.
	 */
	if(buddy_list_empty(head))
		return;
	/*
	 * List has one element and entry not in list can't cut into two list.
	 */
	if(buddy_list_singular(head) && (head->next != entry && head != entry))
		return;
	/*
	 * Entry is head can't cut into two list.
	 */
	if(head == entry)
		buddy_list_init(head);
	else
		_buddy_list_cut_position(list,head,entry);
}
/*
 * splice two list
 */
static void _buddy_list_splice(const struct buddy_list *list,
		struct buddy_list *prev,
		struct buddy_list *next)
{
	struct buddy_list *first = list->next;
	struct buddy_list *last  = list->prev;

	prev->next  = first;
	first->prev = prev;

	next->prev  = last;
	last->next  = next;
}
/*
 * splice - join two lists,this is designed for stacks
 */
static void buddy_list_splice(struct buddy_list *list,
		struct buddy_list *head)
{
	if(!buddy_list_empty(head))
		_buddy_list_splice(list,head,head->next);
}
static void buddy_list_splice_tail(struct buddy_list *list,
		struct buddy_list *head)
{
	if(!buddy_list_empty(head))
		_buddy_list_splice(list,head->prev,head);
}
static void buddy_list_splice_init(struct buddy_list *list,
		struct buddy_list *head)
{
	if(!buddy_list_empty(head))
	{
		_buddy_list_splice(list,head,head->next);
		buddy_list_init(list);
	}
}
static void buddy_list_splice_tail_init(struct buddy_list *list,
		struct buddy_list *head)
{
	if(!buddy_list_empty(head))
	{
		_buddy_list_splice(list,head->prev,head);
		buddy_list_init(list);
	}
}
#define offset(type,member) \
	(size_t)(&(((type *)0)->member))
#define container_of(ptr,type,member) ({ \
		const typeof(((type *)0)->member) *__mptr = (ptr); \
		(type *)((char *)__mptr - offset(type,member)); \
		})
#define buddy_list_entry(ptr,type,member) \
	container_of(ptr,type,member)
#define buddy_list_first_entry(ptr,type,member) \
	buddy_list_entry((ptr)->next,type,member)
/*
 * list for each elememt
 */
#define buddy_list_for_each(pos,head) \
	for(pos = (head)->next; \
			__builtin_prefetch(pos->next,0,2),(pos) != (head); \
			(pos) = (pos)->next)
#define _buddy_list_for_each(pos,head) \
	for(pos = (head)->next;pos != (head);pos = pos->next)
#define buddy_list_for_each_prev(pos,head) \
	for(pos = (head)->prev; \
			__builtin_prefetch(pos->prev,0,2),pos != head; \
			pos = pos->prev)
#define buddy_list_for_safe(pos,n,head) \
	for(pos = (head)->next,n = pos->next; pos != (head); \
			pos = n,n = pos->next)
#define buddy_list_for_prev_safe(pos,n,head) \
	for(pos = (head)->prev,n = pos->prev; pos != (head) ; \
			pos = n,n = pos->prev)
#define buddy_list_for_each_entry(pos,head,member) \
	for(pos = buddy_list_entry(((head)->next),typeof(*pos),member); \
			__builtin_prefetch(pos->member.next,0,3), \
			&(pos->member) != (head); \
			pos = buddy_list_entry(pos->member.next,typeof(*pos),member))
#define buddy_list_for_each_entry_reverse(pos,head,member) \
	for(pos = buddy_list_entry((head)->prev,typeof(*pos),member); \
			__builtin_prefetch(pos->member.next,0,3), \
			&(pos->member) != (head); \
			pos = buddy_list_entry(pos->member.prev,typeof(*pos),member))
#define buddy_list_prepare_entry(pos,head,member) \
	((pos) ? : buddy_list_entry(head,typeof(*pos),member))
/*
 * main
 */
int main()
{
	/*
	 * create head
	 */
	struct buddy_list head,list,basic;
	struct buddy_list *entry;
	struct buddy_dev dev,*tmp = NULL,dev0,dev1,dev2,dev3;
	/*
	 * init head
	 */
	dev.list.data = STOP;
	head.data  = STOP;
	list.data  = STOP;
	basic.data = STOP;
	
	buddy_list_init(&dev.list);
	buddy_list_init(&head);
	buddy_list_init(&list);
	buddy_list_init(&basic);
    /*
	 * add member for list
	 */
	buddy_create(&head,10);
	buddy_create(&list,5);
	buddy_create(&dev.list,25);
	dev0.data = 123;
	dev1.data = 456;
	dev2.data = 789;
	dev3.data = 147;

	buddy_list_add(&dev0.list,&basic);
	buddy_list_add(&dev1.list,&basic);
	buddy_list_add(&dev2.list,&basic);
	buddy_list_add(&dev3.list,&basic);
	tmp = buddy_list_prepare_entry(tmp,basic.next,list);
	if(tmp == NULL)
		printf("hahah\n");
	else
		printf("the ----%d-\n",tmp->data);
	printf("||->");
	buddy_list_for_each_entry(tmp,&basic,list)
	{
		printf("%d->",tmp->data);
		if(tmp->data == 123)
			break;
	}	
	printf("\n");
	
	/*
	 * other operation for list
	 */
	printf("||->");
	buddy_list_for_each_entry_reverse(tmp,&basic,list)
	{
		printf("%d->",tmp->data);
		if(tmp->data == 147)
			break;
	}
	printf("\n||->");
	buddy_list_for_each(entry,&head)
	{
		printf("%d->",entry->data);
	}
	printf("\n||->");
	buddy_list_for_each_prev(entry,&head)
	{
		printf("%d->",entry->data);
	}
	printf("\n");
	buddy_list_splice_init(&list,&head);
	//buddy_show(&dev.list);
//	buddy_show(&list);
    /*
	 * delete the list
	 */
	buddy_del(&head);
	buddy_del(&dev.list);
//	buddy_del(&list);
    /*
	 * deal with head
	 */
	buddy_list_del_init(&dev.list);
	buddy_list_del_init(&list);
	buddy_list_del_init(&head);
	return 0;
}
