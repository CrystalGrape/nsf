#include "../module.h"
#include <stdio.h>
#include <stdlib.h>
#include <cjson/cJSON.h>
void nsf_talk_proc(int event, NsfntPkg pkg);
void nsf_on_recv(NsfntPkg pkg);
static int cfd=0;    //用于存储中间件客户端套接字
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
		nsf_on_recv(pkg);
		break;
	case NE_QUIT:
		break;
	}
}

void nsf_on_recv(NsfntPkg pkg)
{
	char query[100];
	cJSON *pRoot = cJSON_Parse(pkg.data);
	cJSON *pId = cJSON_GetObjectItem(pRoot, "id");
	if(pkg.msg == NM_NOTICE)
	{
		if(cfd != 0)
			send(cfd,pkg.data,pkg.datalen,0);
		cJSON_Delete(pRoot);
		return;
	}
	if(strcmp(pId->valuestring,"WEB")==0){
		//WEB传来消息，直接发送到中间件客户端
		//中间件客户端本进程，直接发送，否则通知master进程
		if(cfd!=0)
			send(cfd,pkg.data,pkg.datalen,0);
		else
			nsf_post_msg(NM_NOTICE,pkg);
	}else{
		cfd = pkg.cfd;
		double value = cJSON_GetObjectItem(pRoot,"value")->valuedouble;
		sprintf(query,"./insertdata.sh %s %f",pId->valuestring,value);
		system(query);
	}
	cJSON_Delete(pRoot);
}
