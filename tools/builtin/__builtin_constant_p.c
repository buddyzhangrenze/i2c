/*
 * Built-in Function: int __builtin_constant_p(exp)
 * We can use the built-in function __builtin_constant_p to detemine if a
 * value is known to be constant at compile-time and  hence that GCC can
 * perform constantfolding on expressions involving that value.The argument
 * of the function is the value to test.The function returns the integer 1
 * if the argument is known to be a compile-time constant and 0 if it is 
 * not known to be a compile-time constant.A return of 0 does not indicat
 * that the value is not a contant,but merely that GCC cannot prove it is
 * a constant with the specified value of the -O option.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
 * We may use this built-in function in either a macro or an inline 
 * function.However,if we use it in an inlined fucntion and pass an 
 * argument of the funtion as the argument to the built in,GCC will 
 * never return 1 when we call the inline function with a string 
 * constant or compound literal and will not return 1 when we pass a
 * constant numeric value to he inline function unless we specify the
 * -O option.
 */
static inline void buddy_constant_inline(const int a)
{
	if(__builtin_constant_p(a))
		printf("The arguement is a constant\n");
}
static void buddy_constant_no_inline(const int a)
{
	if(__builtin_constant_p(a))
		printf("The arguement is a constant not inline \n");
}
int main()
{
	/*
 	* We would typically use this function in an embedded appliction where 
 	* memory was a critical resource.If we have some complex calculation,we
 	* may want it to be folded if it involves constants,but need to call a
 	* function if it does not.
 	*/
	int a = 10;
	int const b = 10;
	if(__builtin_constant_p(a))
		printf("The a is constant\n");
	if(__builtin_constant_p(10))
		printf("The 10 is constant\n");
	if(__builtin_constant_p(b))
		printf("The const value is constant\n");
	/*
	 * This is an acceptable initializer even if EXPRESSION is not a 
	 * constant expression.GCC must be more conservative about evaluating
	 * the built-in int this case,because it has no opportunity to perform
	 * optimization.
	 */
	buddy_constant_inline(10);
	buddy_constant_no_inline(10);
	return 0;
}
