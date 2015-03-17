#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>



int main()
{	
	int ret;
	struct rlimit rlimt;
	ret = getrlimit(RLIMIT_NOFILE,&rlimt);
	printf("the soft[%lu]\nthe hard[%lu]\n",rlimt.rlim_cur,rlimt.rlim_max);
	rlimt.rlim_cur = 10;
	setrlimit(RLIMIT_NOFILE,&rlimt);
	printf("The soft[%lu]\n",rlimt.rlim_cur);
	return 0;
}
