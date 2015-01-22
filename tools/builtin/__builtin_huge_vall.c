/*
 * Built-in Function: long double __builtin_huge_vall(void)
 * Return a positive infinity,if support by the floating-point format,else
 * DBL_MAX.This function is suitable for implementing the ISO C macro 
 * HUGE_VAL.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	/*
	 * Return a inf mean infinity,and return a NAN means not a number.
	 * In printf format arguement,"long double" use %Lf.
	 */
	printf("The __builtin_huge_vall is [%Lf]\n",
			__builtin_huge_vall());
	return 0;
}
