#include <signal.h>
#include <sys/wait.h>
#include "nsf_epoll.h"
#include "nsf_master.h"
#include <stdio.h>
#include <stdlib.h>
int exit_flag = 0;
int stop_flag = 0;

void nsf_signal_handler(int signo)
{
	pid_t pid;
	int stat;
	struct nsf_notification_message msg;
	memset(&msg, 0, sizeof(msg));
	switch(signo){
	case SIGCHLD:
		while((pid=waitpid(-1,&stat,WNOHANG))>0)
			exit_flag++;
		break;
	case SIGINT:
		//退出前先卸载有名信号量
		printf("卸载信号量\n");
		nsf_unlink_sem();
		msg.message = NM_CLOSE;
		printf("通知子进程关闭\n");
		nsf_notification(0, msg);
		while(exit_flag != nsf_fork_amount());
		exit(0);
		break;
	default:
		break;
	}
	return;
}

void nsf_stop_sys()
{
	stop_flag = 1;
}

int nsf_check_exit()
{
	if(exit_flag == 0 || stop_flag == 1)
		return 0;
	exit_flag--;
	return 1;
}
