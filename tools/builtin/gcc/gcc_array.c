/*
 * GCC array.
 * Variable array
 *The length of array is variable,can determine its size  in running.
 * Zero array
 * GCC allow we to declare a zero array,it can implement on end of struct.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
 * Zero array
 */
struct line {
	int length;
	char array[0];
};
/*
 * Variable length array
 */
struct lin {
	int length;
	char array[];
};
int main()
{
	int i,count = 10;
	char ch = 'a';
	struct line *bd;
	static struct lin nb = {5,{'1','2','3','4','5'}};
	/*
	 * == Zero array ==
	 */
	/*
	 * malloc memory to zero array.
	 */
	bd = (struct line *)malloc(sizeof(struct line) + count);
	if(!bd)
	{
		printf("No enough memory\n");
		return -1;
	}
	/*
	 * push data to zero array.
	 */
	bd->length = 10;
	for(i = 0; i < count ; i++)
	{
		bd->array[i] = ch + i;
	}
	/*
	 * get the length of struct line.
	 */
	printf("sizeof(struct line) = %d\n",sizeof(struct line));
	for(i = 0; i < count; i++)
		printf("%2c",bd->array[i]);
	printf("\n");
	/*
	 * == Variable array==
	 */
	/*
	 * Can't get size of variable array
	 */
	printf("sizeof(struct lin) = %d\n",sizeof(struct lin));

	return 0;
}
