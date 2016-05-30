#include "../module.h"
#include <stdio.h>
#include "avl.h"

AVLTree tree;
void nsf_talk_proc(int event, NsfntPkg pkg);
void nsf_on_recv(NsfntPkg pkg);

void nsf_talk_proc(int event, NsfntPkg pkg)
{
	switch(event)
	{
	case NE_INIT:
		printf("module init success\n");
		break;
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
