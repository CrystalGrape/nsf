#include <signal.h>
#include <sys/wait.h>
#include "nsf_epoll.h"
#include <stdio.h>
int exit_flag = 0;

void nsf_signal_handler(int signo)
{
	pid_t pid;
	int stat;
	switch(signo){
	case SIGCHLD:
		while((pid=waitpid(-1,&stat,WNOHANG))>0)
			exit_flag++;
		break;
	case SIGINT:
		//退出前先卸载有名信号量
		nsf_unlink_sem();
		exit(0);
		break;
	default:
		break;
	}
	return;
}

int nsf_check_exit()
{
	if(exit_flag == 0)
		return 0;
	exit_flag--;
	return 1;
}
