#include "nsf_event.h"
#include "nsf_master.h"
#include <stdio.h>
void onRecv(int event,NsfntPkg pkg)
{
	switch(event)
	{
	case NE_ACPT:
		break;
	case NE_RECV:
		write(pkg.cfd, pkg.data, pkg.datalen);
		break;
	case NE_QUIT:
		break;
	default:
		break;
	}
}

void nsf_msgproc(struct nsf_notification_message msg)
{
}
