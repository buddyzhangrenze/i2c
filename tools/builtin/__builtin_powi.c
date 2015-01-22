/*
 * Built-in Function: int __builtin_powi(double,int)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	double a = 5.0;
	/*
	 * Return the first argument raised to the power of the second.Unlike
	 * the pow function no guarantees about precision and rounding are made
	 * eg: (5,2) means 5*5.return 25.
	 */
	printf("The __builtin_powi(5.0,2):[%f]\n",
			__builtin_powi(a,2));
	/*
	 * Similar to __builtin_powi(),has more function for return,such as
	 * float __builtin_powif(float,int)
	 * long double __builtin_powil(long double,int)
	 */
	return 0;
}
