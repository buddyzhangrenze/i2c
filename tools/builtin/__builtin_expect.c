/*
 * Built-in Function: long __builtin_expect(long exp,long c)
 * We may use __builtin_expect to provide the compiler with branch 
 * prediction information.In general,we should prefer to use actual profile
 * feedback for this(--fprofile-arcs),as programmers are notoriously bad 
 * at predicting how their program actually perform.However,there are 
 * applications in which this data is hard to collect.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	int *a = NULL;
	int x  = 1;
	/*
	 * The return value is the value of exp,which should be an integral
	 * expression.The value of c must be a compile-time constant.The 
	 * semantics of the built-in are that it is expected that exp == c.
	 */
	if(__builtin_expect((a == NULL),1))
		printf("The a is NULL\n");
	/*
	 * would indication that we don't expect to call,since we expect x to
	 * be zero.Since we are limited to integral expressions for exp,we 
	 * should use constructions such.
	 */
	if(__builtin_expect(!!(a),0))
		printf("The builtin is over\n");
	return 0;
}
