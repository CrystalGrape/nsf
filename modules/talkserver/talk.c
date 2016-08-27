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
		break;
	case NE_ACPT:
		break;
	case NE_RECV:	
		break;
	case NE_QUIT:
		break;
	default:
		break;
	}
}

WORKER_MODULE_INIT(worker_init);
MODULE_PROC(worker_proc);
