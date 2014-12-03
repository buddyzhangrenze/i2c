/*
 * This file is test at24c EEPROM,it can check at24c enable to work 
 */
#include <stdio.h>
#include <linux/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
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
	int i;
	char state[1] = "a";
	char rbuf[10] = {0,0,0,0,0,0,0,0,0,0};
	char wbuf[10] = {0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa};
	/*
	 * open
	 */
	fd = open(DEV_NAME,O_RDWR);
	if(fd < 0)
	{
		perror("open");
		exit(-1);
	}
	/*
	 * test slave
	 */
	if(read(fd,state,1) == 0)
		printf("Succeed to read,the stata is %s\n",state);
	/*
	 * write
	 */
	ret = write(fd,wbuf,10);
	if(ret < 0)
	{
		perror("write");
		exit(-1);
	}
	printf("write data to i2c-dev\n");
	for(i = 0;i < sizeof(wbuf);i++)
		printf("wbuf[%d] = %x\n",i,wbuf[i]);
	printf("write finish\n");
	/*
	 * read
	 */
	printf("start read.....\n");
	lseek(fd,0,SEEK_SET);
	ret = read(fd,rbuf,10);
	if(ret < 0)
	{
		perror("read");
		exit(-1);
	}
	for(i = 0;i<sizeof(rbuf);i++)
	{
		printf("rbuf[%d] = %x\n",i,rbuf[i]);
	}
	printf("read finish\n");
	close(fd);
	return 0;
}
