#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#define DEBUG      1

char tostring(int n);

int main()
{
	int length = 0;
	int *a ;
	int n;
	int i = 0;
	char *s ;
	int j;
	int m;

	printf("input a number:");
	scanf("%d",&n);
	i = n;
#if DEBUG
	printf("1. The input number is:%d\n",n);
#endif
	/*
	 * get length for n array
	 */
	while(n)
	{
		n /= 10;
		length += 1;
	}
#if DEBUG
	printf("2. The length of number is:%d\n",length);
#endif
	/*
	 * alloc resource
	 */
	n = i;
	i = 0;
	a = (int *)malloc(length*4);
	memset(a,0,length);
	s = (char *)malloc(length + 1);
	memset(s,0,length+1);
	/*
	 * get every bit number
	 */
#if DEBUG
	printf("=========================================\n");
#endif
	while(n)
	{
		a[i] = n % 10;
#if DEBUG
		printf("4. data check next:n=%d.a[%d]=%d\n",n,i,a[i]);
#endif
		i++;
		n /= 10;
	}
#if DEBUG
	/*
	 * check data for a array
	 */
	i = length;
	while(length--)
		printf("5. output:[%d]=%d\n",length,a[length]);
	length = i;
	printf("6. check length in bit = %d\n",length);
#endif
	/*
	 * transfer int to char
	 */
	i=0;
	s[length] = '\0';
	j = length;
	while(length--)
	{
#if DEBUG
		for(m = 0;m < j;m++)
			printf("a[%d] = %d ",m,a[m]);
		printf("\n");
		printf("=================================================\n");
		printf("7. check a array:-----------a[%d]=%d\n",length,a[length]);
		n = a[length];
		
#endif
//		s[i] = tostring(a[length]);
		s[i] = (char)((a[length]) + '0');
#if DEBUG
		for(m = 0;m< j;m++)
			printf("a[%d] = %d ",m,a[m]);
		printf("\n");
		printf("8. check s and a arrays:----s[%d]=%c,a[%d]=%d\n",i,s[i],length,a[length]);
#endif
		i++;
	}
#if DEBUG
	/*
	 * check output
	 */
	printf("===========================================\n");
	printf("9.  the s's length are :%d\n",strlen(s));
	printf("10. the string are [%s]\n",s);
#endif
	if(s == NULL)
		printf("sddf\n");
	free((void *)s);
	free((void *)a);
	return 0;
}

char tostring(int n)
{
	int num = n - 0;
#if DEBUG
	printf("11.check in tostring()------num=%d,n=%d\n",num,n);
	printf("12.check char in tostring():c=%c\n",(num + '0'));
#endif
	return num + '0';
}
