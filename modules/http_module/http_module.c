#include "module.h"
#include <stdio.h>
#define NE_USER_PRINT		30
static WINSTANCE wInstance;
EVENT_START(NE_USER_PRINT),
EVENT_END();

void worker_init(WINSTANCE w)
{
	memcpy(&wInstance,&w,sizeof(w));
}

void worker_proc(int event, NsfntPkg pkg)
{
	switch(event){
	case NE_USER_PRINT:
		printf("%s\n", pkg.data);
		break;
	}
}

WORKER_MODULE_INIT(worker_init);
MODULE_PROC(worker_proc);
