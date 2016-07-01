#include "nsf_master.h"
#include "module.h"

int nsf_worker_client;

int nsf_create_workerclt()
{
	struct sockaddr_in server;
	int lfd=socket(AF_INET,SOCK_STREAM,0);
	if(lfd==-1)
		return -1;
	bzero(&server,sizeof(struct sockaddr_in));
	server.sin_family=AF_INET;
	server.sin_addr.s_addr=inet_addr("127.0.0.1");
	server.sin_port=htons(12111);
	if(-1==connect(lfd,(struct sockaddr *)(&server),sizeof(struct sockaddr)))
	{
		return -2;
	}
	nsf_worker_client = lfd;
	return lfd;
}

void nsf_default_workerproc(struct nsf_notification_message msg)
{
	switch(msg.message)
	{
	case NM_NOTICE:
		msg.pkg.msg=NM_NOTICE;
		nsf_post_event(NE_RECV, msg.pkg);
		break;
	case NM_CLOSE:
		close(nsf_worker_client);
		exit(0);
		break;
	case NM_REMOD:
		nsf_workermodule_reload();
		break;
	default:
		msg.pkg.msg = msg.message;
		nsf_post_event(NE_RECV, msg.pkg);
		break;
	}
}

void nsf_post_msg(int message, NsfntPkg pkg)
{
	struct nsf_notification_message msg;
	msg.message = message;
	msg.destpid = 0;
	msg.srcpid = getpid();
	msg.srcfd = 0;
	memcpy(&(msg.pkg), &pkg, sizeof(pkg));
	write(nsf_worker_client, (char *)&msg, sizeof(msg));
}
