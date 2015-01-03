#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/*
 * This function is show wait process for transfering
 */
void show_print(int percent,int barlen)
{
	int i;
	putchar('[');
	for(i=1;i<= barlen;++i)
		putchar(i*100 <= percent * barlen ? '>' : ' ');
	putchar(']');
	printf("%3d%3d",percent,percent);
	for(i=0;i != barlen + 12;++i)
		putchar('\b');
}
	
int main()
{
	int i;
	for(i = 0;i<101;++i)
	{
		show_print(i,50);
		fflush(stdout);
		sleep(1);
	}
	return 0;
}
