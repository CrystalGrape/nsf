#ifndef module_h
#define module_h
#include "nsf_config.h"
#include "nsf_epoll.h"
#include "nsf_event.h"
#include "nsf_master.h"
#include "nsf_signal.h"
#define MODULE_CONFIG_PATH "nsf_module.cfg"
//工作进程实例 
typedef struct worker_instance
{
	//事件相关API
	int (*nsf_post_event)(int event, NsfntPkg pkg);
	int (*nsf_get_event)(int *event, NsfntPkg *pkg);
	//消息相关API
	void (*nsf_post_msg)(int message, NsfntPkg pkg);
}WINSTANCE;

//守护进程实例 
typedef struct master_instance
{
	void (*nsf_notification)(int pid, struct nsf_notification_message msg);
}MINSTANCE;

#define MASTER_MODULE_INIT(func)	int _mtype_=0;char _init_[30]=#func
#define WORKER_MODULE_INIT(func)	int _mtype_=1;char _init_[30]=#func
#define MODULE_PROC(func)			char _proc_[30]=#func	

#define EVENT_START(event)			int _evl_[]={event
#define EVENT(event)				event
#define EVENT_END()					101}
typedef void (*MINIT)(MINSTANCE mInstance);
typedef void (*WINIT)(WINSTANCE wInstance);

//用于主程序，插件不可用
void *nsf_load_module(const char *path);
void nsf_workermodule_reload();
#endif
