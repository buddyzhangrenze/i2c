#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


int main()
{
	pid_t pid;
	char *message;
	int n;

	printf("fork program starting\n");
	pid = fork();
	switch(pid)
	{
		case -1:
			perror("fork failed");
			exit(1);
		case 0:
			message = "This is the child";
			n = 5;
			break;
		default:
			message = "This is the parent";
			n = 10;
			break;
	}
	for(;n > 0; n--)
	{
		printf("%s\n",message);
		sleep(1);
	}
	if(pid != 0)
	{
		/*
		 * Father task
		 */
		int start_val;
		pid_t child_pid;

		child_pid = wait(&start_val);
		printf("Child has finished:PID = %d\n",child_pid);
		if(WIFEXITED(start_val))
			printf("Child exited with code %d\n",child_pid);
		else
			printf("Child terminated abnormally\n");
	}
	exit(0);
}
