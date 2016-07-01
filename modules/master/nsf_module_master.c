#include "module.h"
#include <stdio.h>
#define NM_USER_TEST	130
static MINSTANCE mInstance;
void nsf_master_msgproc(struct nsf_notification_message msg)
{
	switch(msg.message)
	{
	case NM_CONNECT:
		break;
	case NM_USER_TEST:
		mInstance.nsf_notification(0, msg);
		break;
	}
}

void master_init(MINSTANCE m)
{
	memcpy(&mInstance,&m,sizeof(m));
}

MASTER_MODULE_INIT(master_init);
MODULE_PROC(nsf_master_msgproc);
