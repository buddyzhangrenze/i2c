#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

#define DEV_NAME "/dev/at24c01"

int main()
{
	int ret;
	int fd;
	char *tmp;
	/*
	 * open device
	 */
	fd = open(DEV_NAME,O_RDWR);
	if(fd < 0)
	{
		printf("Unable to open device\n");
		exit(1);
	}
	printf("Succeed to open device\n");
	/*
	 * read file
	 */
	if(read(fd,tmp,1) == 0)
		printf("Fail to read data from device\n");
	printf("Succeed to read data from device\n");

	close(fd);
}
