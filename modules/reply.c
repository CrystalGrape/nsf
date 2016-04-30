#include "nsf_event.h"

void onRecv(int event,NsfntPkg pkg)
{
	if(event == 0)
		write(pkg.cfd, pkg.data, pkg.datalen);
}
