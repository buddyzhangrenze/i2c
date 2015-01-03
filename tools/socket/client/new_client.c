/*
 * Copyright @2014 Buddy Zhang - ATU
 */
/*
 * This program is client for socket
 */
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#define DEBUG 0
/*
 * data segment
 */
#define BUFFER_SIZE 1024
#define SOCKET_COUNT 500
static int end_count = 0;
/*
 * buddy data
 */
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
	char *state;
	char *file_list;
	char *buf;
	char addr[256];
	int file_depth;
	buddy_file_attr file;
	buddy_exchange_data data;
}buddy_socket;
typedef struct
{
	char a[20][30];
	int size;
}buddy_dir;
/*   
 * buffer struct
 */
#define PORT        8888
/*
 * function head
 */
void buddy_socket_function(void *data);
void buddy_update(void *master);
void buddy_download(void *master);
void buddy_exchange_message(void *client,void *data,void *file_attr,int state);
void list_file_server(void *master);
void list_file_client(void *master);
void input_file_name(void *master);
void input_func(void *master);
void buddy_end(void *master,char *s);
inline void buddy_start_server(void *master);
inline void buddy_stop_server(void *master);
void buddy_recv_menu(void *master);
void buddy_recv_sub_menu(void *master);
void socket_delay(void);
void *trave_dir(void *master,void *b,int depth);
/*
 * main()
 */
int main()
{
	buddy_socket buddy_fd;
	struct sockaddr_in  server_addr;
	/*
	 * create socket
	 */
	buddy_fd.client_fd = socket(AF_INET,SOCK_STREAM,0);
	if(buddy_fd.client_fd == -1)
	{
		perror("Client_Buddy: Can't create client\n");
		exit(-1);
	}
	printf("Client_buddy:succeed to create socket\n");
#if DEBUG
#else
	/*
	 * user input
	 */
	printf("Please input server address:\n");
	scanf("%s",buddy_fd.addr);
#endif
	/*
	 * set struct sockaddr_in
	 */
	bzero(&server_addr,sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port   = htons(PORT);
#if DEBUG
	server_addr.sin_addr.s_addr   = inet_addr("127.0.0.1");
#else
	server_addr.sin_addr.s_addr   = inet_addr(buddy_fd.addr);
#endif
	/*
	 * call connect(),connet to server
	 */
	connect(buddy_fd.client_fd,(struct sockaddr *)&server_addr,sizeof(server_addr));
	printf("Client_buddy: Succeed to connect\n");
	/*
	 * send message to server
	 */
	buddy_start_server(&buddy_fd);
	/*
	 * User operation
	 */
	buddy_socket_function((void *)&buddy_fd);
	/*
	 * ending
	 */
	buddy_stop_server(&buddy_fd);
	close(buddy_fd.client_fd);
	exit(1);
}
/*
 * function select
 */
void buddy_socket_function(void *master)
{
	buddy_socket *socket = (buddy_socket *)master;
	/*
	 * area of idle
	 */
	printf("=============Welcom to Buddy socket==============\n");
	/*
	 * Recv menu
	 */
	buddy_recv_menu(socket);
}
/*
 * buddy_update
 */
void buddy_update(void *master)
{
	buddy_socket *dev = (buddy_socket *)master;
	buddy_file_attr *file = malloc(sizeof(buddy_file_attr));
	/*
	 * open file
	 */
	printf("file_name:%s\n",dev->file.name);
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
	printf("Client send %d bytes\n",dev->file.count);
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
 * buddy_downlaod client
 */
void buddy_download(void *master)
{
	buddy_socket *dev    = (buddy_socket *)master;
	buddy_file_attr *file = malloc(sizeof(buddy_file_attr));
	/*
	 * Create file
	 */
	printf("===================================\n");
	printf("|| file_name: %s\n",dev->file.name);
	printf("===================================\n");
	printf("|| state : %s\n",dev->state);
	printf("===================================\n");
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
	printf("=========================================\n");
	file->count = fwrite((void *)dev->buf,1,file->size,dev->file.input);
	if(!file->count)
		printf("Cannot write file\n");
	printf("|| Recevice %d bytes \n",file->count);
	printf("=========================================\n");
	/*
	 * server return
	 */
	free(dev->buf);
	fclose(dev->file.input);
	free(file);
}
/*
 * send/recv message
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
			send(dev->client_fd,tmp + current_pos,count,0);
		} else if(strncmp(dev->state,"download",8) == 0)
		{
			recv(dev->client_fd,tmp + current_pos,count,0);
		}
		current_pos += count;
		size -= count;
	}
	printf("|| exchange message complete\n");
}
/*
 * input file name
 */
void input_file_name(void *master)
{
	int ret;
	buddy_socket *dev = (buddy_socket *)master;
	buddy_file_attr *file = malloc(sizeof(buddy_file_attr));
	printf("=============Input file name=========\n");
	printf("> ");
#if 0
	file->name = "buddy.mp3";
#else
	file->name = malloc(256);
	scanf("%s",file->name);
#endif
	printf("======================================\n");
	file->size = strlen(file->name);
	ret = send(dev->client_fd,(void *)file,sizeof(buddy_file_attr),0);
	ret = send(dev->client_fd,(void *)file->name,file->size,0);
	/*
	 * file operation
	 */
	dev->file.name = malloc(256);
	dev->file.name = file->name;
#if DEBUG
#else
	free(file->name);
#endif
	free(file);
	input_func(dev);
}
/*
 * input function 
 */
void input_func(void *master)
{
	buddy_exchange_data *data = malloc(sizeof(buddy_exchange_data));
	buddy_socket *dev = (buddy_socket *)master;
	/*
	 * user menu
	 */
	/*
	 * user select
	 */
	if(strncmp(dev->state,"update",6) == 0)
	{
		data->cmd = "update";
		data->size = 6;
		send(dev->client_fd,data,sizeof(buddy_exchange_data),0);
		send(dev->client_fd,data->cmd,strlen(data->cmd),0);
		free(data);
		dev->state = data->cmd;
		buddy_update(dev);
	} else if(strncmp(dev->state,"download",8) == 0)
	{
		data->cmd = "download";
		data->size = 8;
		send(dev->client_fd,data,sizeof(buddy_exchange_data),0);
		send(dev->client_fd,data->cmd,data->size,0);
		dev->state = data->cmd;
		free(data);
		buddy_download(dev);
	}
}
/*
 * start server
 */
inline void buddy_start_server(void *master)
{
	int d;
	buddy_socket *dev = (buddy_socket *)master;
	dev->state = malloc(5);
	send(dev->client_fd,"start",5,0);
	recv(dev->client_fd,(void *)dev->state,3,0);
	if(strncmp(dev->state,"YES",3) == 0)
	{
		printf("======================================\n");
		printf("==========Client start server=========\n");
	} else
	{
		printf("======================================\n");
		printf("============Fail to server============\n");
	}
	free(dev->state);
	printf("1--end\n2--continu\ninput you number\n");
	scanf("%d",&d);
	if(d == 1)
		buddy_end(dev,"Y");
	else
		buddy_end(dev,"N");
}
/*
 * stop server
 */
inline void buddy_stop_server(void *master)
{
	
}
/*
 * recv menu
 */
void buddy_recv_menu(void *master)
{
	buddy_socket *dev = (buddy_socket *)master;
	buddy_exchange_data *tmp = (buddy_exchange_data *)malloc(sizeof(buddy_exchange_data));
	/*
	 * Recv meum size
	 */
	recv(dev->client_fd,tmp,sizeof(buddy_exchange_data),0);
	dev->buf = malloc(tmp->size);
	recv(dev->client_fd,dev->buf,tmp->size,0);
	printf("\n%s\n",dev->buf);
	/*
	 * User select
	 */
	printf("=======Input you want========\n");
	printf("> ");
	scanf("%d",&tmp->count);
	if(tmp->count == 1)
	{
		tmp->cmd = "download";
		dev->state = "download";
		tmp->size = 8;
	} else if(tmp->count == 2)
	{
		tmp->cmd = "update";
		dev->state = "update";
		tmp->size = 6;
	}
	send(dev->client_fd,(void *)tmp,sizeof(buddy_exchange_data),0);
	send(dev->client_fd,(void *)tmp->cmd,(strlen(tmp->cmd)),0);

	free(dev->buf);
	/*
	 * Function entry
	 */
	free(tmp);
    buddy_recv_sub_menu(dev);
	
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
		if(stat(file->d_name,&sb) >= 0 && S_ISDIR(sb.st_mode) && depth <= 3)
		{
			trave_dir(dev,data,depth+1);
		}
	}
	closedir(dir);
	return data;
}
/*
 * recv sub menu
 */
void buddy_recv_sub_menu(void *master)
{
	buddy_socket *dev = (buddy_socket *)master;
	buddy_exchange_data *tmp = (buddy_exchange_data *)malloc(sizeof(buddy_exchange_data));
	/*
	 * Recv meum size
	 */
	recv(dev->client_fd,tmp,sizeof(buddy_exchange_data),0);
	dev->buf = malloc(tmp->size);
	recv(dev->client_fd,dev->buf,tmp->size,0);
	printf("===============================\n");
	printf("\n%s\n",dev->buf);
	/*
	 * User select
	 */
	printf("\n=======Input you want========\n");
	printf("> ");
	scanf("%d",&tmp->count);
	if(tmp->count == 1)
	{
		tmp->cmd = "list_server";
		tmp->size = 11;
	} else if(tmp->count == 2)
	{
		tmp->cmd = "list_client";
		tmp->size = 11;
	}
	send(dev->client_fd,(void *)tmp,sizeof(buddy_exchange_data),0);
	send(dev->client_fd,(void *)tmp->cmd,(strlen(tmp->cmd)),0);
	printf("===========file list===============\n");
	free(dev->buf);
	/*
	 * Function entry
	 */
	if(tmp->count == 1)
	{
		list_file_server(dev);
	} else if(tmp->count == 2)
	{
		list_file_client(dev);
	}
	
}
/*
 * recv list
 */
void list_file_server(void *master)
{
	buddy_socket *dev = (buddy_socket *)master;
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
	printf("=========================================\n");
	input_file_name(dev);
}
/*
 * recv list_client
 */
void list_file_client(void *master)
{
	buddy_socket *dev = (buddy_socket *)master;
#if 0
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
#endif
	input_file_name(dev);
}
void buddy_end(void *master,char *s)
{
	buddy_socket *dev = (buddy_socket *)master;
	char *state = malloc(1);
	end_count += 1;
	send(dev->client_fd,(void *)s,1,0);
	recv(dev->client_fd,state,1,0);
	if(strncmp(state,"Y",1) == 0)
	{
		printf("==>[%d]->end socket\n",end_count);
		free(state);
	} else
	{
		printf("==>[%d]->no end socket\n",end_count);
		free(state);
	}

}
