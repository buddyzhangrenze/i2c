/*
 * Buit-in funtion: void __builtin_prefetch(const void *addr,....)
 * This function is used to minimize cache-miss latency by moving data into
 * a cache before it is accessed.We can insert calls to __builtin_prefetch
 * into code for target supports them,data prefetch instruction will be 
 * generated.If the prefetch is done early enough before the access then 
 * the data will be in the cache by the time it is accessed.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
/*
 * The value of addr is the address of the memory to prefetch.There are
 * two optional arguments,rw and locality.the value of rw is a compile-time
 * constant one or zero;one means that the prefetch is preparing for a 
 * write to the memory address and zero,the default,means that the prefetch
 * for a read.The value loaclity must be a compile-time constant integer 
 * between zero and three.A value of zero means that the data has no 
 * temporal locality,so it need not be left in the cache after the access.
 * A value of the three means that the data has a high degree of temporal
 * locality and should be left in all levels of cache possible.Values of 
 * one and two means,respecitively,a low or moderate degree of temporal
 * locality.The default is three.
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
	/*
	 * Data prefetch does not generate faults if addr is invalid,but the 
	 * address expression itself must be valid.For example,a prefetch of 
	 * p->next will not fault if p->next is not a valid address,but 
	 * evaluation will fault if p is not a valid address.
	 */
	for(i = 0; i<10 ; i++)
	{
		__builtin_prefetch(&a[i],0,3);
		printf("the %d data is %d\n",i,a[i]);
	}
	/*
	 * If the target does not support data prefetch,the address expression
	 * is evaluated if it include side effects but no other code is 
	 * generated and GCC does not issue a warning.
	 */
	return 0;
}
