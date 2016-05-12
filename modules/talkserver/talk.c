#include "../module.h"
#include <stdio.h>

void nsf_talk_proc(int event, NsfntPkg pkg);
void nsf_on_recv(NsfntPkg pkg);

void nsf_talk_proc(int event, NsfntPkg pkg)
{
	switch(event)
	{
	case NE_ACPT:
		break;
	case NE_RECV:
		break;
	case NE_QUIT:
		break;
	}
}

void nsf_on_recv(NsfntPkg pkg)
{
	
}
