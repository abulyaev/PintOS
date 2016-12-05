#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"

static void syscall_handler (struct intr_frame *);

extern struct semaphore check;
extern struct exit_info exite;

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
	/*
		void *pp = malloc(sizeof(void *));
  	memcpy(pp, esp, sizeof(void *));
	(*esp) -= sizeof(char **);
  	memcpy(*esp, pp, sizeof(char **));
  	(*esp) -= sizeof(int);
  	memcpy(*esp, &count_arg, sizeof(int));
  	//printf("%d count_arg\n", count_arg);
  	void *return_ptr = 0;
  	(*esp) -= sizeof(void *);
  	memcpy(*esp, &return_ptr, sizeof(void *));
	*/	
	if (*(int*) f->esp == SYS_EXIT)
	{
		int exit_status = ((size_t*) f->esp)[1];
		exite.exit_code = exit_status;
		sema_up(&check);
		thread_exit();
	}
	if(*(int*) f->esp == SYS_WRITE) 
	{
		putbuf(((const char**) f->esp)[2], ((size_t*) f->esp)[3]);
		return;
	}

	printf ("call system!\n");
	thread_exit();
}
