/*
 * The keyword '__alignof__' allows we to inquire about how an object is 
 * alignof,or minimum alignment usually required by a type.Its syntax is 
 * just like 'sizeof'.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
 * For example,if the target machine requires a 'double' value to be 
 * aligned on an 8-byte boundary,then '__alignof__(double)' is 8.This is 
 * true on many RISC machines.On more traditional machine designs,
 * '__alginof__(double)' is 4 or even 2.
 */
struct buddy {
	char ch1;
	char ch2;
	short sh;
	int data;
};
/*
 * In X86. 
 */
int main()
{
	/*
	 * Some machine never actually require alignment.they allow reference
	 * to any data type even at an odd addresses.For these machines,
	 * '__alignof__' reports the _recommended_alignment of a type.
	 */
	struct buddy bd;	
	struct buddy mv;
	printf("=======The alignemnt in X86=======\n");
	printf("The alignment of char is\t[%d]\n",
			__alignof__(char));
	printf("The alignment of short is\t[%d]\n",
			__alignof__(short));
	printf("The alignment of int is\t[%d]\n",
			__alignof__(int));
	printf("The alignment of long is\t[%d]\n",
			__alignof__(long));
	printf("The alignment of float is\t[%d]\n",
			__alignof__(float));
	printf("The alignment of double is\t[%d]\n",
			__alignof__(double));
	printf("The alignment of long long is\t [%d]\n",
			__alignof__(long long));
	printf("The alignment of int * is\t[%d]\n",
			__alignof__(int *));
	printf("The alignement of char * is\t[%d]\n",
			__alignof__(char *));
	printf("The alignemtn of int ** is\t[%d]\n",
			__alignof__(int **));
    printf("The alignment of struct buddy is\t[%d]\n",
			__alignof__(struct buddy));
	printf("The size of the struct is\t[%d]\n",
			sizeof(struct buddy));
	/*
	 * If the operand of '__alignof__' is an lvalue rather than a type,
	 * its value is the required alignment for its type,taking into account
	 * any minimum alignment specified with GCC's '__attribute__' extension
	 * (*note Variable Attribute::).
	 */
    printf("the alignment of the buddy->ch1 is\t[%d]\n",
			__alignof__(bd.ch1));
	return 0;
}
