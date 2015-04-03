/*
 * Stack operation
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STACK_SIZE 50
static const_data[5] = {1,10,100,1000,10000};
/*
 * Stack struct
 */
typedef struct s {
	int top;
	int array[STACK_SIZE];
}Stack;
/*
 * Init stack
 */
void init_stack(Stack *S)
{
	S->top = STACK_SIZE;
}
/*
 * Push Stack
 */
void push(Stack *S,int data)
{
	S->top--;
	S->array[S->top] = data;
}
/*
 * Pop Stack
 */
int pop(Stack *S)
{
	return S->array[S->top++];
}
/*
 * isempty
 */
int isempty(Stack *S)
{
	if(S->top == STACK_SIZE)
		return 0;
	else 
		return 1;
}
/*
 * spil
 */
void spil(Stack *S,char *s)
{
	char ch;
	int data,tmp;
	int count = 0;
	while(1)
	{
		ch = *s;
		printf("The char is %c\n",*s);
		/*
		 * number
		 */
		if(ch != '+' && ch != '-' && ch != '*' && ch != '/' && ch != '\0')
		{
			printf("pus char is %c\n",ch);
			push(S,ch);
		}
		/*
		 * character
		 */
		else
		{
			while(isempty(S))
			{
				/*
				 * Stack is not empry
				 */
				ch = pop(S);
				printf("pop char is %c\n",ch);
				tmp = ch - '0';
				data += tmp * const_data[count];
				count++;
			}
			/* test data */
			printf("The data is %d\n",data);
			data  = 0;
			count = 0;
		}
		if(*s == '\0')
			break;
		s++;
	}
}
/*
 * calcuate
 */
void calc(Stack *S,int *a,char *string)
{
	int count = 0;
	char ch;
	int data;
	while(*string != '\0')
	{
		ch = *string;
		if(ch == '+' && ch == '-')
		{
			if(isempty(S))
			{
				/*
				 * add or sub
				 */
				push(S)
			}
		}
	}
}
/*
 * main
 */
int main()
{
	char *string = "521+365+210+526+5489+1548+12554+145";
	/*
	 * Stack 
	 */
	Stack stack;
	init_stack(&stack);
	
	spil(&stack,string);
}
