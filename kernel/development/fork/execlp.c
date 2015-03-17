#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	printf("Running ps with execlp\n");
	execlp("ls","ls",0);
	printf("Done.\n");
	return 0;
}
