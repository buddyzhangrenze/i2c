#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#define PAGE_SIZE 4096

int main()
{
	int fd;
	int i;
	unsigned char *p_map;

	fd = open("/dev/buddy_test",O_RDWR);
	if(fd < 0)
	{
		printf("open fault\n");
		exit(1);
	}
	/*
	 * mmap
	 */
	p_map = (unsigned char *)mmap(0,PAGE_SIZE,PROT_READ | PROT_WRITE,
			MAP_SHARED,fd,0);
	if(p_map == MAP_FAILED)
	{
		printf("mmap fault\n");
		goto here;
	}
	for(i=0; i < 10; i++)
		printf("%d\n",p_map[i]);
here:
	munmap(p_map,PAGE_SIZE);
	return 0;
}
