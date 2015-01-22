/*
 * Stantard output arguement list
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	/* signed char */
	printf("The signed char\t[%c],size is [%d]\n",
			(char)'a',sizeof(char));
	/* signed short */
	printf("The signed short\t[%d],size is [%d]\n",
			(short)250,sizeof(short));
	/* signed int */
	printf("The int   \t[%d],size is [%d]\n",
			(int)250,sizeof(int));
	/* signed long */
	printf("The long  \t[%ld],size is[%d]\n",
			(long)250,sizeof(long));
	/* long long */
	printf("The long long \t[%Ld],size is[%d]\n",
			(long long)250,sizeof(long long));
	/* signed float */
	printf("The float \t[%f],size is [%d]\n",
			(float)250,sizeof(float));
	/* signed double */
	printf("The double \t[%lf],size is [%d]\n",
			(double)250,sizeof(double));
	/* long double */
	printf("The long double \t[%Lf],size is [%d]\n",
			(long double)250,sizeof(long double));
	return 0;
}
