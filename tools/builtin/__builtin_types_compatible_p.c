/*
 * -Buit-in Function:int __builtin_types_compatible_p(type1,type2)
 * We can use the built-in function __builtin_type_compatible_p to determi-
 * ne whether two types are the same.
 * This built-in function return :
 *   1 if the unqulified versions of the type1 and type2(which are types,
 *     not expressions)are compatible.
 *   0 otherwise.The result of this built-in function can be used in integ-
 *     er constant expressions.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
 * type define in Linux kernel 2.6.38
 * typedef unsigned char       u8
 * typedef unsigned short      u16
 * typedef unsigned int        u32
 * typedef unsigned long long  u64
 * typedef signed char         s8
 * typedef short               s16
 * typedef int                 s32
 * typedef long long           s64
 */
int main()
{
	int a = 5;
	int array[2],brray[2];
	int *p = array;
	char *b = NULL;
	int **q = &p;
	enum TYPE {
		NOE,
		TWO,
	}bn,pn;
	enum BACK {
		GP,
		BN,
	}bk;
	/*
	 * The type int[] and int[5] are compatible.On the other hand,int and
	 * char * are not compatible,even if the size of their types.On the 
	 * particular architecture are the same.Also,the amount of pointer in-
	 * direction is token into account when determining similarity,short *
	 * is not similar to short **.Furthermore,two types that are typedefed
	 * are considered compatible if their underlying types are compatible.
	 */ 
	printf("The type is compatible %d\n",
			__builtin_types_compatible_p(typeof(a),double));
	printf("The int[] and int * is compatible %d\n",
			__builtin_types_compatible_p(typeof(array),typeof(array[0])));
	printf("The array[] and point is compatible %d\n",
			__builtin_types_compatible_p(typeof(array),typeof(p)));
	printf("The int[1] and int is compatible %d\n",
			__builtin_types_compatible_p(typeof(array[1]),int));
	printf("The different point is not compatible %d\n",
			__builtin_types_compatible_p(typeof(int *),typeof(char *)));
	printf("The a[] and b[] is compatible %d\n",
			__builtin_types_compatible_p(typeof(array),typeof(brray)));
	/*
	 * An enum type is not considered to be compatible with another enum
	 * type even if both are compatible with the same integer type;This is
	 * what the C standard specifies.
	 */
	printf("The TYPE one and TYPE two is compatible %d\n",
			__builtin_types_compatible_p(typeof(bn=NOE),typeof(bn=TWO)));
	printf("The TYPE one and BACK bn is compatible %d\n",
			__builtin_types_compatible_p(typeof(bn=NOE),typeof(bk=BN)));
	return 0;
}

