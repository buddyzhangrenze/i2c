/*
 * Anonymous Union.
 * We can use union with no name in a struct,and we can use member of union
 * as member of struct.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct buddy {
	char code;
	union {
		char ch;
		int mid;
	};
	char *name;
};
int main()
{
	struct buddy bd;
	bd.ch = 'a';
	printf("The union ch is [%c]\n",bd.ch);
	bd.mid = 10;
	printf("The union int is [%d]\n",bd.mid);
	return 0;
}
