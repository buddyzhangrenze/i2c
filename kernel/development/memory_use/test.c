#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	printf("%08x\n",__builtin_return_address(0));
	return 0;
}
