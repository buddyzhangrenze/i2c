#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define offset(type,member) (size_t)&(((type *)0)->member)
#define container_of(ptr,type,member) ({ \
		typeof(((type *)0)->member) *__mptr = (ptr); \
		(type *)((char *)__mptr - offset(type,member)); \
		})

struct test {
	int c;
};
struct buddy_test {
	int a;
	int b;
	struct test c;
};
int main()
{
	struct buddy_test d,*q;
	struct test *p;
	p = &d.c;

	d.a = 10;
	d.b = 20;
	d.c.c = 30;
	
	q = container_of(p,struct buddy_test,c);
	printf("The data is %d\n",q->c.c);
	return 0;
}
