#include "nsf_master.h"
#include <string.h>

int nsf_create_adminclt()
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
	return lfd;
}

void parse_cmd(const char *str, char *cmd, char *arg)
{
	sscanf(str,"%s %s", cmd, arg);
}

int main()
{
	int i;
	int len;
	char input[40];
	char cmd[10];
	char varg[30];
	int nsf_client = nsf_create_adminclt();
	struct nsf_notification_message msg;
	while(1){
		memset(&msg, 0, sizeof(msg));
		msg.message = NM_CMD;
		printf("nsf>:");	
		scanf("%s",cmd);
		//scanf("%[^\n]%*c", input);
		memcpy(msg.pkg.data, cmd, strlen(cmd));
		msg.pkg.datalen = strlen(cmd);
		//parse_cmd(input,cmd, varg);

		//printf("cmd:%s\narg:%s\n",cmd,varg);
		//scanf("%s", cmd);
		if(strcmp(cmd, "stop") == 0){
			msg.message = NM_CLOSE;
			write(nsf_client, (char *)&msg, sizeof(msg));
			close(nsf_client);
			printf("nsf server exit success\n");
			exit(0);
		}
		
		if(strcmp(cmd, "quit") == 0){
			close(nsf_client);
			exit(0);
		}
		
		if(strcmp(cmd, "state") == 0){
			write(nsf_client, (char *)&msg, sizeof(msg));
			memset(&msg, 0, sizeof(msg));
			len = read(nsf_client, (char *)&msg, sizeof(msg));
			for(i = 0;i < msg.pkg.datalen/(sizeof(int)*2); i++){
				printf("pid[%d]\t\t%d\n", ((int *)msg.pkg.data)[i*2],((int *)msg.pkg.data)[i*2+1]);
			}
		}


		if(strcmp(cmd, "remod") == 0)
		{
			write(nsf_client, (char *)&msg, sizeof(msg));
		}
	}
}
