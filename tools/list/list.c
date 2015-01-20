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
		printf("The data is *%d\n",count);
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
		printf("free data is %d\n",tmp->data);
		free(tmp);
	}
}
static void buddy_show(struct buddy_list *head)
{
	struct buddy_list *list = head->next;
	while(list->data != STOP)
	{
		printf("The list data is[%d]\n",list->data);
		list = list->next;
	}
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
 * main
 */
int main()
{
	/*
	 * create head
	 */
	struct buddy_list head,list;
	struct buddy_list *entry;
	struct buddy_dev dev,*tmp;
	/*
	 * init head
	 */
	dev.list.data = STOP;
	head.data  = STOP;
	list.data  = STOP;
	
	buddy_list_init(&dev.list);
	buddy_list_init(&head);
	buddy_list_init(&list);
    /*
	 * add member for list
	 */
	buddy_create(&head,10);
	buddy_create(&list,5);
	buddy_create(&dev.list,25);
	/*
	 * other operation for list
	 */
	buddy_list_splice_init(&list,&head);
	buddy_show(&dev.list);
	tmp = buddy_list_first_entry(&dev.list,struct buddy_dev,list);
	printf("The data buddy is %d\n",&tmp->list.data);
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
