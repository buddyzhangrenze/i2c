/*
 * Built-in Function:double __builtin_inf(void)
 * Return a positive infinity,expect a warning is generated if the target
 * floating-point format does not support infinities.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	/*
	 * Return inf means infinity,and return NAN means not a number.
	 */
	printf("The __builtin_inf:[%lf]\n",
			__builtin_inf());
	return 0;
}
