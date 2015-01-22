/*
 * Built-in Function: double __builtin_huge_val(void)
 * Return a positive infinity,if supported by floating-point format,else
 * DBL_MAX.This function is suitable for implementing the ISO C macro
 * HUGE_VAL.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
int main()
{
	/*
	 * Return inf meas positive infinity,and return nan means not a number.
	 */
	printf("The __builtin_huge_val():[%lf]\n",
			__builtin_huge_val());
	printf("The __builtin_huge_valf():[%f]\n",
			__builtin_huge_valf());
	printf("The __builtin_huge_vall():[%Lf]\n",
			__builtin_huge_vall());
	return 0;
}
