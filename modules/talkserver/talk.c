#include "module.h"
#include <stdio.h>
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
