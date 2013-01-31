/*
 * =====================================================================================
 *
 *       Filename:  jnxterm.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/30/13 20:33:47
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <stdio.h>
static int ISLOADING_SPIN = 0;
pthread_t loader_thread;
int fd;
fpos_t pos;
void jnx_term_override_stdout(char *path)
{
	fflush(stdout);
	fgetpos(stdout,&pos);
	fd = dup(fileno(stdout));
	freopen(path,"w",stdout);

}
void jnx_term_reset_stdout()
{
	fflush(stdout);
	dup2(fd,fileno(stdout));
	close(fd);
	clearerr(stdout);
	fsetpos(stdout,&pos);
}
void *loading_loop(void *ptr)
{
	struct timespec _nano;
	_nano.tv_sec = 0;
	_nano.tv_nsec = (100 * 1000000);

	char spin_status[4] = {'|','/','-','\\'};
	int counter = 0;

	printf("Loading -  ");

	while(ISLOADING_SPIN == 1)
	{
		printf("\033[1D\033[K%c", spin_status[counter%4]);
		fflush(stdout);
		++counter;
		nanosleep(&_nano,NULL);
	}
	printf("\n");
	return 0;
}
void jnx_term_load(int state)
{
	ISLOADING_SPIN = state;

	if(ISLOADING_SPIN == 1)
	{
		pthread_create(&loader_thread,NULL,loading_loop,NULL);
		return;
	}
	else
	{
		pthread_join(loader_thread,NULL);
	
	}
}
