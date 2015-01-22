/*
 * Built-in Function: float __builtin_huge_valf(void)
 * Return a positive infinity,if supported by the floating-point,else
 * DBL_MAX.This function is suitable for implementing the ISO C macro
 * HUGE_VAL_
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	/*
	 * Return inf means infinity,and return NAN means not a number.
	 */
	printf("The __builtin_huge_valf:[%f]\n",
			__builtin_huge_valf());
	return 0;
}
