#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>


typedef struct 
{
	char a[20][30];
	int size;
}buddy_dir;


void *trave_dir(void *b,int depth)
{
	buddy_dir    *data = (buddy_dir *)b;
	DIR *dir;
	struct dirent *file;
	struct stat sb;
	char *path = "./";
	/*
	 * open dir
	 */
	dir = opendir(path);
	if(!dir)
	{
		printf("Error opendir %s\n",path);
		exit(1);
	}
	while((file = readdir(dir)) != NULL)
	{
		if(strncmp(file->d_name,".",1) == 0)
			continue;
		strcpy(data->a[data->size++],file->d_name);
		printf("size :%d\n",data->size);
		if(stat(file->d_name,&sb) >= 0 && S_ISDIR(sb.st_mode) && depth <= 3)
		{
			trave_dir(data,depth + 1);
		}
	}
	closedir(dir);
	return data;
}



int main()
{
	buddy_dir *data = malloc(sizeof(buddy_dir));
	data->size = 0;
	data = (buddy_dir *)trave_dir(data,0);
	
	while(data->size-- >0)
		printf("aaa%s\n",data->a[data->size]);
	return 0;
}
