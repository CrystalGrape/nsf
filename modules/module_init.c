#include "nsf_event.h"
extern void onRecv(int event,NsfntPkg pkg);
void nsf_module_init()
{
	nsf_register_event(0,onRecv);
}
