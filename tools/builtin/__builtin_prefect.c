#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
/*
 * __builtin_prefetch(const void *address,int rw,int locality);
 * @address:the memory address will read or write.
 * @rw:1 write,0 read.
 * locality:temporal locality->
 *      0:hasn't temporal locality.
 *      3:has high temporal locality.
 *      1/2:mid temproal locality.
 */
int main()
{
	int a[10] = {0,0,0,0,0,0,0,0,0,0};
	int b[10] = {0,1,2,3,4,5,6,7,8,9};
	int i;
	for(i = 0; i<10 ; i++)
	{
		a[i] = a[i] + b[i];
		/*
		 * temporal write
		 */
		__builtin_prefetch(&a[i],1,2);
		/*
		 * temporal read
		 */
		__builtin_prefetch(&b[i],0,2);
	}
	for(i = 0; i<10 ; i++)
	{
		__builtin_prefetch(&a[i],0,3);
		printf("the %d data is %d\n",i,a[i]);
	}
	return 0;
}
