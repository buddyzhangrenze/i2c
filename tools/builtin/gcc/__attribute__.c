/*
 * GCC the most powerful feature is '__attribute__'.It can set function 
 * attribute,variable attribute and type attribute.
 * The keyword '__attribute__' use as:__attribute__((sttribute-list)).the
 * position is between end of declare and before ';'.
 * attribute-list has 6 member,include 'aligned','packed','transparent_u-
 * nion','unused','deprecated' and 'may_alias'.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
 * __attribte__((format))
 */
void buddy_format(const char *s,...)
	__attribute__((format(printf,1,2)));
/*
 * __attribute__((noreturn))
 */
int buddy_exit(int i) 
	__attribute__((noreturn));
/*
 * __attribute__((const))
 */
int __attribute__((const)) seq(int n);
/*
 * __attribute__((constructor))
 */
void __attribute__((constructor)) init_entry(void)
{
	printf("The __init__function[%s]\n",__FUNCTION__);
}
/*
 * __attribute__((destructor))
 */
void __attribute__((destructor)) eixt_entry(void)
{
	printf("The __exit__function[%s]\n",__FUNCTION__);
}
/*
 * -finstrument-function
 * It will generate __cyg_profile_func_exit() and __cyg_profile_func_entry
 * in entry and exit of function for use definine.
 */
void __attribute__((no_instrument_function)) 
	__cyg_profile_func_entry(void *callee,void *callsite);
void __attribute__((no_instrument_function))
	__cyg_profile_func_exit(void *callee,void *callsite);
void __cyg_profile_func_entry(void *callee,void *callsite)
{
	printf("Entry %p in %p\n",callee,callsite);
}
void __cyg_profile_func_exit(void *callee,void *callsite)
{
	printf("Exite %p in %p\n",callee,callsite);
}

int main() 
{
/*
 * Variable attribute
 */
	{
	/*
	 * aligned: __attribute__((aligned(int n)))
	 * This attribute can set size for alignment(1 = 1 byte). 
	 */
	struct s {
		short b[3];
	}__attribute__((aligned(4)));

//	typedef int int32_t __attribute__((aligned(4)));

	struct b {
		short b[5];
	}__attribute__((aligned));
	
	struct s bd,mp;
	struct b bd1,mp1;

	printf("== aligned(8) ==\n");
	printf("The address of bd is:[%d]\n",(int)&bd);
	printf("The address of mp is:[%d]\n",(int)&mp);
	printf("== aligned() ==\n");
	printf("The address of bd1 is:[%d]\n",(int)&bd1);
	printf("The address of mp1 is:[%d]\n",(int)&mp1);
	}
	{
	/*
	 * Packed: __attribute__((__packed__))  
	 * packed make compiler not alignment and aligned by current bytes.
	 */
	struct my {
		char ch;
		int  a;
	};
	struct my_packed {
		char ch;
		int a;
	}__attribute__((__packed__));
	struct my n;
	struct my_packed p;
	printf("=== no packed ===\n");
	printf("the size of my->a is:[%d]\n",sizeof(n.a));
	printf("the size of my is:[%d]\n",sizeof(n));
	printf("=== packed ===\n");
	printf("the size of my_packed->a is:[%d]\n",sizeof(p.a));
	printf("the size of my_packed is:[%d]\n",sizeof(p));
	}
/*
 * Function attribute
 * Must use '-Wall' in compile-time,like:
 * gcc xxx.c -Wall -o a
 */	
	{
	/*
	 * format:format(archetype,string-index,first-to-check)
	 * format told compiler check arguement format as printf and scanf and
	 * so on.
	 * 'archetype' determine the style.like 'printf' or 'scanf'
	 * 'string-index' determine the number of arguemnt is format string.
	 * 'first-to-check' determine check from this by this rule.
	 */
	void buddy_format(const char *s,...)
	{
		const char *tmp = s;
		printf("%s\n",tmp);
	}

	buddy_format("buddy%d",2);
	}
	{
	/*
	 * noreturn:
	 * It told compiler no return anything.If function need return 
	 * something but can't research to position of return.The attribute 
	 * will aviod generate error information.The declare of abort() and 
	 * exit() use this format.
	 */
	void buddy_exit(int i)
	{
		printf("exit\n");
	}
	buddy_exit(1);
	}
	{
	/*
	 * const:
	 * It must use in function that has some arguement.It will have same 
	 * reutrn value When call these functions which has some arguemet 
	 * repeatly,and compiler use usefull way.Expect first perfect,they will
	 * return the same value,it is effective.
	 */
	int seq(int n)
	{
		printf("%s[%d]\n",__FUNCTION__,n);
	}
	seq(10);
	seq(10);
	seq(10);
	seq(10);
	}
	{
	/*
	 *
	 */
	}
	return 0;
}











