#include "nsf_event.h"
#include "nsf_master.h"
extern void onRecv(int event,NsfntPkg pkg);
extern void nsf_msgproc(struct nsf_notification_message msg);
void nsf_module_init()
{
	nsf_register_event(NE_RECV,onRecv);
	nsf_register_event(NE_ACPT,onRecv);
	nsf_register_event(NE_QUIT,onRecv);
}

void nsf_master_module_init()
{
	//nsf_register_msgproc(nsf_msgproc);
}
