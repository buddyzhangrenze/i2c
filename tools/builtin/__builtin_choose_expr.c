/*
 * -Built-in Function:type __builtin_choose_expr(const_exp,exp1,exp2)
 * We can use the __builtin_choose_expr to evaluate code depending on the
 * value of a constant expression.This built-in function return exp1 if
 * const_exp,which is a constant expression that must be able to determined
 * at compile time,is nonzero.Otherwise it return 0.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
 * This built-in function is analogus to the "?:" operation in C,except 
 * that the expression returned has its type unaltered by promotion rules.
 * Also,the built-in function does not evaluate the expression that was not
 * chosen.For example,if const_exp evaluates to true,exp2 is not evaluated
 * even if it has side-effects.
 */
int main()
{
    int a = 10;
	int *b = NULL;
	*b = 10;
	/*
	 * This built-in function can return an lvalue if the chosen argument
	 * is a lvalue.
	 * If exp1 is returned,the return type is the same as exp1's type.
	 * Similarly,if exp2 is return,its return types is the same as exp2.
	 */
	if(__builtin_choose_expr(__builtin_types_compatible_p(
					typeof(a),typeof(b[0])),1,0))
		printf("The a is same with *b\n");
	else
		printf("The a isnot same with *b\n");
	return 0;
	/*
	 * Note:This construct is only available for C.Furthermore,the unused
	 * expression(exp1 or exp2 depending on the value of const_exp) may
	 * still generate syntax errors.This may change in future revisions.
	 */
}
