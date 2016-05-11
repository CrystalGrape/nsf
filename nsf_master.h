#ifndef nsf_master_h
#define nsf_master_h
#include "nsf_epoll.h"
#include "nsf_event.h"
#include "nsf_signal.h"
#include <signal.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/prctl.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

#define NM_CONNECT	101
#define NM_NOTICE	102
#define NM_CLOSE	103
#define NM_ADDUSER	104
#define NM_DELUSER	105
#define NM_CMD		106

#pragma pack(push)
#pragma pack(1)
struct nsf_notification_message
{
	int message;
	int destpid;
	int srcpid;
	int srcfd;
	NsfntPkg pkg;
};
#pragma pack(pop)

struct worker_mgr
{
	int nsf_worker_pid;				//工作进程ID
	int nsf_worker_client;			//工作进程客户端套接字
	int nsf_client_amount;			//工作进程当前客户数量
	int state;
};

typedef void (*NsfftMsgproc)(struct nsf_notification_message msg);
//master
int nsf_fork_amount();
int nsf_register_msgproc(NsfftMsgproc cblk);
void nsf_srvepoll();
int nsf_create_mastersrv(int core);
void nsf_close_mastersrv();
void nsf_notification(int pid, struct nsf_notification_message msg);
void nsf_add_srvepoll(int fd);
int nsf_delete_srvepoll(int client);
void nsf_default_msgproc(struct nsf_notification_message msg);

//worker
int nsf_create_workerclt();
void nsf_default_workerproc(struct nsf_notification_message msg);
void nsf_post_msg(int message, NsfntPkg pkg);
#endif
