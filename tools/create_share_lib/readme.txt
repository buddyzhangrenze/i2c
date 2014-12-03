Create a share lib (xxx.so)
setp 1:
	include file: buddy.c main.c buddy.h
	The buddy.h context is:
	/* 
	 * The head of hello.c
	 */
	#ifndef HELLO_H
	#define HELLO_H

	void buddy_show(const char *buf);

	#endif

	The buddy.c context is:
	/*
	 * The source code of buddy.c
	 */
	#include <stdio.h>

	void buddy_show(const char *buf)
	{
		printf("buddy-%s\n",buf);
	}

	The main.c context is:
	#include "hello.h"

	int main()
	{
		buddy_show("everything");
		return 0;
	}

setp 2:
	gcc hello.c -c
setp 3:
	gcc hello.o -shared -fPIC -o libhello.so
setp 4:
	sudo mv libhello.so /usr/lib
set 5:
	modify main.c and add "hello.h" into main.c,then
	gcc main.c -L. -lhello -o show
set 6:
	./show
	everything
