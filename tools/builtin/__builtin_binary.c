/*
 * Built-in: Deal with binary 
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	/*
	 * Return one plus the index of the least significant 1-bit of x,or if
	 * x is zero,return zero.
	 * eg 16: 1 0000. return 5.
	 */
	printf("The __builtin_ffs(16):[%d]\n",
			__builtin_ffs(16));
	/*
	 * Retrun the number of leading 0-bits in x,starting at the most 
	 * significant bit position.If x is 0,the result is undefined.
	 * eg 16: 0000 0000 0000 0000 0000 0000 0001 000. return 27.
	 */
	printf("The __builtin_clz(16):[%d]\n",
			__builtin_clz(16));
	/*
	 * Retrun number of trailing 0-bits in x,starting at the least 
	 * significant bit position.If x is 0,the result is undefined.
	 * eg 16: 1 0000. return 4.
	 */
	printf("The __builtin_ctz(16):[%d]\n",
			__builtin_ctz(16));
	/*
	 * Return the number of 1-bits in x
	 * eg 16: 1 0000. return 1.
	 */
	printf("The __builtin_popcount:[%d]\n",
			__builtin_popcount(16));
	/*
	 * Return the parity of x, i.e the number of 1-bits in x modulo 2.
	 * eg:16 1 0000. return 1.
	 * eg:17 1 0001. return 0.
	 */
	printf("The __builtin_parity(16):[%d]\n",
			__builtin_parity(16));
	printf("The __builtin_parity(17):[%d]\n",
			__builtin_parity(17));
	/*
	 * Have move builtin function,such as __builtin_ffsl.....
	 * expect arguement type is unsigned long.Other is same.
	 */
	return 0;
}
