/*
 * Built-in Function: double __builtin_nan(const char *str)
 * This is an implementation of the ISO C99 function nan.
 * Since ISO C99 defines this function in terms of strod,which we do not 
 * implement,a description of the parsing is in order.The string is parsed
 * as by strtol;that is,the base is recognized by leading "0" or "0x" 
 * prefixes.The number parsed is placed in the significand such that
 * the least significant bit of the number is at the least significant bit
 * of the significand.The number is truncated to fit the significand field
 * privated.The significand is forced to be a quiet NAN.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	/*
	 * This function,if given a string literal,is evaluated early enough 
	 * that it is considered a compile-time constant.
	 */
	printf("The __builtin_nan():[%lf]\n",
			__builtin_nan("Buddy"));
	printf("The __builtin_nanf():[%f]\n",
			__builtin_nanf("Buddy"));
	printf("The __builtin_nanl():[%Lf]\n",
			__builtin_nanl("Buddy"));
	printf("The __builtin_nans():[%f]\n",
			__builtin_nans("Buddy"));
	printf("The __builtin_nansf():[%f]\n",
			__builtin_nansf("Buddy"));
	printf("The __builtin_nansl():[%Lf]\n",
			__builtin_nansl("Buddy"));
	return 0;
}
