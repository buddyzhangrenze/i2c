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
	printf("%3d%%",percent);
	for(i=0;i != barlen + 6;i++)
		putchar('\b');
}
/*
 * This function is show wait process for "...."
 */
void show_print2(void)
{
	char buf = '.';
	int i;
	for(i = 0;i < 100;i++)
	{
		putchar(i);
		putchar('\b');
		fflush(stdout);
		sleep(1);
	}
}
int main()
{
	int i;
	show_print2();
#if 0
	for(i = 0;i<101;i++)
	{
		show_print(i,50);
		fflush(stdout);
		sleep(1);
	}
#endif
	return 0;
}
