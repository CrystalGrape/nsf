#include "module.h"
#include <stdio.h>
#define NM_USER_TEST	130
#define NE_USER_PRINT	30
static WINSTANCE wInstance;
EVENT_START(NE_ACPT),
	EVENT(NE_RECV),
	EVENT(NE_QUIT),
	EVENT(NE_INIT),
EVENT_END();

void worker_init(WINSTANCE w)
{
	memcpy(&wInstance,&w,sizeof(w));
}

void worker_proc(int event, NsfntPkg pkg)
{
	switch(event){
	case NE_INIT:
		printf("module init success\n");
		break;
	case NE_ACPT:
		printf("someone client online\n");
		break;
	case NE_RECV:	
		write(pkg.cfd, pkg.data, pkg.datalen);
		if(pkg.msg != NM_USER_TEST){
			wInstance.nsf_post_msg(NM_USER_TEST, pkg);
		}
		wInstance.nsf_post_event(NE_USER_PRINT, pkg);
		break;
	case NE_QUIT:
		printf("someone client quit\n");
		break;
	default:
		break;
	}
}

WORKER_MODULE_INIT(worker_init);
MODULE_PROC(worker_proc);
