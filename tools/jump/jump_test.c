#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>


jmp_buf jmpbuffer;
void fun2();
void fun1();
int main()
{
	printf("in main\n");
	if(setjmp(jmpbuffer) != 0)
	{
		printf("get the error from jump return !\n");
	}
	fun1();
	printf("main function end !\n");
	return 0;
}
void fun1()
{
	printf("in func1 !\n");

	fun2();
}
void fun2()
{
	printf("in fun2 !\n");
	longjmp(jmpbuffer,1);
}
