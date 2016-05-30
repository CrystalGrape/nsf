/*
                   _ooOoo_
                  o8888888o
                  88" . "88
                  (| -_- |)
                  O\  =  /O
               ____/`---'\____
             .'  \\|     |//  `.
            /  \\|||  :  |||//  \
           /  _||||| -:- |||||-  \
           |   | \\\  -  /// |   |
           | \_|  ''\---/''  |   |
           \  .-\__  `-`  ___/-. /
         ___`. .'  /--.--\  `. . __
      ."" '<  `.___\_<|>_/___.'  >'"".
     | | :  `- \`.;`\ _ /`;.`/ - ` : | |
     \  \ `-.   \_ __\ /__ _/   .-` /  /
======`-.____`-.___\_____/___.-`____.-'======
                   `=---='
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
         佛祖保佑       永无BUG
*/
#include "nsf_event.h"
#include "nsf_epoll.h"
#include "nsf_config.h"
#include "nsf_master.h"
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/stat.h>
extern void nsf_master_module_init();
void init_daemon()
{
	int pid;
	int i;
	if(pid=fork())
		exit(0);
	else if(pid< 0)
		exit(1);
	setsid();
	if(pid=fork())
		exit(0);
	else if(pid< 0)
		exit(1);

	for(i=0;i< NOFILE;++i)
		close(i);
	chdir("/var");
	umask(0);
	return;
}

int main()
{
	
	int serverfd;
	struct nsf_config cfg;
	
	//读取配置文件
	if(nsf_config(&cfg) != 0)
		return -1;
		
	//init_daemon();	
	//服务器初始化
	serverfd = nsf_server_init(cfg.port);
	
	if(serverfd < 0){
		return -2;
	}
	//判断是否单核运行
	if(cfg.single){
		if(nsf_create_mastersrv(1) < 0)
			return 0;
		nsf_master_module_init();
		
		if(nsf_start_worker(serverfd, 1) == 0)
			nsf_start_master(serverfd, 1);
	}else{
		if(nsf_create_mastersrv(cfg.core*2) < 0)
			return 0;
		nsf_master_module_init();
		
		if(nsf_start_worker(serverfd, cfg.core*2) == 0)
			nsf_start_master(serverfd, cfg.core*2);
	}
	
	return 0;
}
