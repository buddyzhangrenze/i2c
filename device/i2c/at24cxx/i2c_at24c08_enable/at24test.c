#include <stdio.h>
#include <linux/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <assert.h>
#include <string.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

#define DEV_NAME "/dev/at24c"

int main()
{

  int ret;
  int fd;
  int i;
  char rbuf[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0,0};//0 where to read ;1 read data
  char wbuf[10] = {0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa};//0 where to write; 1 write data

  fd = open(DEV_NAME,O_RDWR);
  if(0>fd)
  {
	perror("open");
	exit(-1);
  }
/*
  ret = ioctl(fd, I2C_RETRIES, 2);
  if(0>ret)
  {
	perror("ioctl:I2C_RETRIES");
	exit(-1);
  }

  ret = ioctl(fd, I2C_TIMEOUT , 5);
  if(0>ret)
  {
	perror("ioctl:I2C_TIMEOUT");
	exit(-1);
  }
*/  
  ret = write(fd, wbuf, 10);
   if(0>ret)
  {
  	perror("write");
    exit(-1);
  }
   for(i=0; i<sizeof(wbuf); i++)
      printf("wbuf[%d]=%x \n",i,wbuf[i]);
   for(i=0; i<sizeof(rbuf); i++)
      printf("rbuf[%d]=%x\n",i,rbuf[i]);
  ret = lseek(fd,2,SEEK_SET);
  if(0>ret)
  {
  	perror("lseek");
    exit(-1);
  }
  ret = read(fd, rbuf, 8);
  if(0>ret)
  {
  	perror("read");
    exit(-1);
  }
  for(i=0; i<sizeof(rbuf); i++)
     printf("after read:rbuf[%d]=%x\n",i,rbuf[i]);

  close(fd);
  return 0;
}
