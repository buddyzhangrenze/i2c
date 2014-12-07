#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <linux/types.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <assert.h>
#include <string.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>


#define DEV_NAME "/dev/at24c"

int main()
{
	int ret;
	int fd;
	char *tmp;
	int i;
	char rbuf[10] = {0,0,0,0,0,0,0,0,0,0};
	char wbuf[10] = {'a','b','c','d','e','f','g','h','i','j'};
	char wbuf2[10]= {0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa};
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
#if 1
	/*
	 * check conjunction
	 */
	if(read(fd,tmp,1) == 0)
		printf("Fail to conjunction to device\n");
	printf("Succeed to connect data from device\n");
#endif
	/*
	 * write data
	 */
	ret = write(fd,wbuf2,10);
	if(ret != 0)
		printf("Write %d bytes to at24c02\n",ret);
	/*
	 * read data
	 */
	lseek(fd,0,SEEK_SET);
	memset(rbuf,0,10);
	for(i = 0;i<10;i++)
	{
		printf("befor=>data[%d]:%x\n",i,rbuf[i]);
	}
	ret = read(fd,rbuf,10);
	if(ret != 0)
		printf("Read %d butes from at24c02\n",ret);
	/*
	 * show data
	 */
	printf("======================show data============================\n");
	for(i = 0;i < 10; i++)
	{
		printf("==>data[%d]:%x\n",i,rbuf[i]);
	}
	
	close(fd);
}
