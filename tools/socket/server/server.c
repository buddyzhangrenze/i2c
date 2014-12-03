/*
 * Copyright @2014 Buddy Zhang - ATU
 */

/*
 * This program is server for socket
 */

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
/*
 *  data segment
 */
#define PORT           8888
#define BUFFER_SIZE    1024
#define MAX            5
#define SOCKET_COUNT   500
typedef struct 
{
	FILE *input;
	int size;
	int count;
	char *name;
	char *path;
}buddy_file_attr;
typedef struct 
{
	int size;
	int count;
	char *cmd;
	int file_num;
}buddy_exchange_data;
typedef struct 
{
	int client_fd;
	int server_fd;
	char *state;
	int file_depth;
	buddy_file_attr file;
	buddy_exchange_data data;
	char *buf;
}buddy_socket;
typedef struct 
{
	char a[20][30];
	int size;
}buddy_dir;

char *list0 = "==1. LIST_SERVER\n==2. LIST_CLIENT";
char *list1 = "==1. download\n==2. update\n";
/*
 * function head
 */
void buddy_socket_function(void *data);
void buddy_update(void *master);
void buddy_download(void *master);
void buddy_exchange_message(void *client,void *data,void *file_attr,int state);
void buddy_start_server(void *master);
inline void buddy_stop_server(void *master);
void list_file_server(void *master);
void list_file_client(void *master);
void input_file_name(void *master);
void input_func(void *master);
void buddy_send_menu(void *master);
void buddy_send_sub_menu(void *master);
void socket_delay(void);
void *trave_dir(void *master,void *b,int depth);
/*
 * main()
 */
int main()
{
	struct sockaddr_in  server_addr;
	buddy_socket buddy_fd;
	int i,ret;
	/*
	 * create socket connect
	 */
	buddy_fd.server_fd = socket(AF_INET,SOCK_STREAM,0);
	if(buddy_fd.server_fd == -1)
	{
		perror("Server_buddy: can't create socket\n");
		exit(1);
	}
	printf("Server_buddy:Succeed to create socket,id = %d\n",buddy_fd.server_fd);
	/*
	 * set struct sockaddr_in 
	 */
	bzero(&server_addr,sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port   = htons(PORT);
	server_addr.sin_addr.s_addr  = INADDR_ANY;
	/*
	 * allocate local address bind to socket repeatly
	 */
	setsockopt(buddy_fd.server_fd,SOL_SOCKET,SO_REUSEADDR,&i,sizeof(i));
	/*
	 * bind socket
	 */
	ret = bind(buddy_fd.server_fd,(struct sockaddr *)&server_addr,
			sizeof(server_addr));
	if(ret == -1)
	{
		perror("Server_buddy:faid to bind socket\n");
		exit(1);
	}
	printf("Server_buddy:Succeed to bind socket\n");
	/*
	 * Call listen(),create request queue
	 */
	ret = listen(buddy_fd.server_fd,MAX);
	if(ret == -1)
	{
		perror("Server_buddy:faid to listen\n");
		exit(1);
	}
	while(1)
	{
		printf("Listening ..........\n");
		/*
     	 * call accept(),wait request from user
		 */
		buddy_fd.client_fd = accept(buddy_fd.server_fd,NULL,NULL);
		if(buddy_fd.server_fd == -1)
		{
			perror("Server_buddy:Can't accept message from client\n");
			exit(1);
		}
		/*
		 * Signal of start server
		 */
		buddy_start_server(&buddy_fd);
		/*
		 * User operatioon
		 */
		buddy_socket_function(&buddy_fd);
		/*
		 * ending
		 */
//		buddy_stop_server(&buddy_fd);
	}	
	close(buddy_fd.server_fd);
	exit(0);
}

/*
 * function select
 */
void buddy_socket_function(void *data)
{
	buddy_socket *socket = (buddy_socket *)data;
	/*
	 * Send menu
	 */
	buddy_send_menu(socket);
}
/*
 * buddy_download server
 */
void buddy_update(void *master)
{
	buddy_socket *dev = (buddy_socket *)master;
	buddy_file_attr *file = malloc(sizeof(buddy_file_attr));
	/*
	 * open file
	 */
	dev->file.input = fopen(dev->file.name,"rb");
	if(dev->file.input == NULL)
	{
		printf("Client_buddy:Cannot open file\n");
		exit(1);
	}
	/*
	 * get file size
	 */
	fseek(dev->file.input,0,SEEK_END);
	dev->file.size = ftell(dev->file.input);
	fseek(dev->file.input,0,SEEK_SET);
	/*
	 * sent file size
	 */
	file->size = dev->file.size;
	send(dev->client_fd,(void *)&dev->file,sizeof(buddy_file_attr),0);
	/*
	 * read file
	 */
	dev->buf = malloc(dev->file.size);
	memset(dev->buf,0,dev->file.size);
	dev->file.count = fread((void *)dev->buf,1,dev->file.size,dev->file.input);
	if(!dev->file.count)
		printf("Cannot read file\n");
	printf("|| Client send %d bytes\n",dev->file.count);
	printf("================================================\n");
	/*
	 * send file
	 */
	buddy_exchange_message((void *)dev,(void *)dev->buf,(void *)file,0);
	/*
	 * state check
	 */
	fclose(dev->file.input);
	free(dev->buf);
	free(file);
}
/*
 * buddy_update server
 */
void buddy_download(void *master)
{
	buddy_socket *dev    = (buddy_socket *)master;
	buddy_file_attr *file = malloc(sizeof(buddy_file_attr));
	/*
	 * Create file
	 */
	dev->file.input = fopen(dev->file.name,"wb+");
	/*
	 * open check
	 */
	if(dev->file.input == NULL)
	{
		printf("Client_buddy:Cannot open file\n");
		exit(1);
	}
	/*
	 * Receive file
	 */
	recv(dev->client_fd,file,sizeof(buddy_file_attr),0);
	dev->buf = malloc(file->size);
	if(!dev->buf)
	{
		printf("No memory\n");
		exit(1);
	}
	memset(dev->buf,0,file->size);
	buddy_exchange_message((void *)dev,(void *)dev->buf,(void *)file,0);
	/*
	 * write file
	 */
	file->count = fwrite((void *)dev->buf,1,file->size,dev->file.input);
	if(!file->count)
		printf("Cannot write file\n");
	printf("|| Recevice %d bytes \n",file->count);
	printf("=======================================\n");
	/*
	 * server return
	 */
	free(dev->buf);
	fclose(dev->file.input);
	free(file);
}
/*
 * exchange message
 */
void buddy_exchange_message(void *client,void *data,void *file_attr,int state)
{
	buddy_socket *dev = (buddy_socket *)client;
	buddy_file_attr *file = (buddy_file_attr *)file_attr;
	int size = file->size;
	int current_pos = 0;
	int count = SOCKET_COUNT;
	char *tmp = (char *)data;
	while(size)
	{
		if(count > size)
			count = size;
		if(strncmp(dev->state,"update",6) == 0)
		{
			recv(dev->client_fd,tmp + current_pos,count,0);
			//printf("recv %d bytes\n",count);
		} else if(strncmp(dev->state,"download",8) == 0)
		{
			send(dev->client_fd,tmp + current_pos,count,0);
			//printf("send %d bytes\n",count);
		}
		current_pos += count;
		size -= count;
	}
	printf("|| exchange message complete\n");
	printf("========================================\n");
}
/*
 * List DIR
 */
void *trave_dir(void *master,void *b,int depth)
{
	buddy_socket *dev = (buddy_socket *)master;
	buddy_dir    *data = (buddy_dir *)b;
	DIR *dir;
	struct dirent *file;
	struct stat sb;
	dev->file.path = "./";
	/*
	 * open dir
	 */
	dir = opendir(dev->file.path);
	if(!dir)
	{
		printf("Error opendir %s\n",dev->file.path);
		exit(1);
	}
	while((file = readdir(dir)) != NULL)
	{
		if(strncmp(file->d_name,".",1) == 0)
			continue;
		strcpy(data->a[data->size++],file->d_name);
		if(stat(file->d_name,&sb) >= 0 && S_ISDIR(sb.st_mode) && dev->file_depth <= 3)
		{
			trave_dir(dev,data,dev->file_depth + 1);
		}
	}
	closedir(dir);
	return data;
}
/*
 * start server
 */
void buddy_start_server(void *master)
{
	buddy_socket *dev = (buddy_socket *)master;
	dev->state = malloc(5);
	recv(dev->client_fd,(void *)dev->state,5,0);
	if(strncmp(dev->state,"start",5) == 0)
	{
		printf("======================================\n");
		printf("==========Server start server=========\n");
		send(dev->client_fd,"YES",3,0);
	} else
	{
		printf("======================================\n");
		printf("============Fail to server============\n");
		send(dev->client_fd,"NO",2,0);
	}
	free(dev->state);
}
/*
 * stop server
 */
inline void buddy_stop_server(void *master)
{
	
}
/*
 * input file name
 */
void input_file_name(void *master)
{
	int ret;
	char *tmp;
	buddy_socket *dev = (buddy_socket *)master;
	buddy_file_attr *file = malloc(sizeof(buddy_file_attr));
	/*
	 * get file attr from client
	 */
	ret = recv(dev->client_fd,(void *)file,sizeof(buddy_file_attr),0);
	dev->file.name = malloc(file->size + 1);
	dev->file.name[file->size] = '\0';

	ret = recv(dev->client_fd,(void *)dev->file.name,file->size,0);
	printf("|| filename: %s\n",dev->file.name);
	printf("======================================\n");
	free(file);
	/*
	 * file operation
	 */
	input_func(dev);
}
/*
 * input function 
 */
void input_func(void *master)
{
	buddy_socket *dev = (buddy_socket *)master;
	buddy_exchange_data *data = malloc(sizeof(buddy_exchange_data));
	/*
	 * recv user select
	 */
	recv(dev->client_fd,data,sizeof(buddy_exchange_data),0);
	data->cmd = malloc(data->size);
	data->cmd[data->size] = '\0';
	recv(dev->client_fd,data->cmd,data->size,0);
	printf("|| state: %s\n",data->cmd);
	printf("=====================================\n");
	if(strncmp(data->cmd,"update",data->size) == 0)
	{
		free(data);
		dev->state = "update";
		free(data->cmd);
		buddy_download(dev);
	}
	else if(strncmp(data->cmd,"download",data->size) == 0)
	{
		free(data);
		dev->state = "download";
		free(data->cmd);
		buddy_update(dev);
	}
}
/*
 * Send menu
 */
void buddy_send_menu(void *master)
{
	buddy_socket *dev = (buddy_socket *)master;
	buddy_exchange_data *data = malloc(sizeof(buddy_exchange_data));
	/*
	 * send meum size
	 */
	data->size = strlen(list1);
	send(dev->client_fd,(void *)data,sizeof(buddy_exchange_data),0);
	send(dev->client_fd,list1,data->size,0);
	/*
	 * User select
	 */
	recv(dev->client_fd,(void *)data,sizeof(buddy_exchange_data),0);
	data->cmd = malloc(data->size+1);
	memset(data->cmd,0,data->size);
	data->cmd[data->size] = '\0';
	recv(dev->client_fd,(void *)data->cmd,data->size,0);
	/*
	 * function entry
	 */
	printf("state---------------%s\n",data->cmd);
	if(strncmp(data->cmd,"update",data->size) == 0)
	{
		free(data->cmd);
		free(data);
		dev->state = "update";
	} else if(strncmp(data->cmd,"download",data->size) == 0)
	{
		free(data->cmd);
		free(data);
		dev->state = "download";
	}
	buddy_send_sub_menu(dev);
}
/*
 * Send sub-menu
 */
void buddy_send_sub_menu(void *master)
{
	buddy_socket *dev = (buddy_socket *)master;
	buddy_exchange_data *data = malloc(sizeof(buddy_exchange_data));
	/*
	 * send meum size
	 */
	data->size = strlen(list0);
	send(dev->client_fd,(void *)data,sizeof(buddy_exchange_data),0);
	send(dev->client_fd,list0,data->size,0);
	/*
	 * User select
	 */
	recv(dev->client_fd,(void *)data,sizeof(buddy_exchange_data),0);
	data->cmd = malloc(data->size+1);
	memset(data->cmd,0,data->size);
	data->cmd[data->size] = '\0';
	recv(dev->client_fd,(void *)data->cmd,data->size,0);
	printf("data->size--------------- %d\n",data->size);
	/*
	 * function entry
	 */
	if(strncmp(data->cmd,"list_server",data->size) == 0)
	{
		printf("=============Server file list=============\n");
		free(data->cmd);
		free(data);
		list_file_server(dev);
	} else if(strncmp(data->cmd,"list_client",data->size) == 0)
	{
		printf("=============Client file list==============\n");
		free(data->cmd);
		free(data);
		list_file_client(dev);
	}
}
/*
 * recv list_server
 */
void list_file_server(void *master)
{
	buddy_socket *dev = (buddy_socket *)master;
	buddy_dir *data = malloc(sizeof(buddy_dir));
	data->size = 0;
	data = (buddy_dir *)trave_dir(dev,data,0);
	send(dev->client_fd,data,sizeof(buddy_dir),0);
	while(data->size-- >0)
		printf("%s\n",data->a[data->size]);
	/*
	 * get file number
	 */
	
	//send(dev->client_fd,"haha",(strlen("haha") + 1),0);
	/*
	 * input file name
	 */
	free(data);
	printf("=======================================\n");
	input_file_name(dev);
}
/*
 * recv list
 */
void list_file_client(void *master)
{
	buddy_socket *dev = (buddy_socket *)master;
#if 0
	buddy_dir *data = malloc(sizeof(buddy_dir));
	recv(dev->client_fd,data,sizeof(buddy_dir),0);
	while(data->size-- >0)
		printf("%s\n",data->a[data->size]);
	/*
	 * get file number
	 */
	/*
	 * input file name
	 */
	#endif
	printf("========================================\n");

	input_file_name(dev);
}
