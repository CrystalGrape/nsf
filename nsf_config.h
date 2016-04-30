#ifndef nsf_config_h
#define nsf_config_h
#include <stdio.h>
#include <stdlib.h>
#define CONFIG_PATH "nsf.cfg"

struct nsf_config
{
	int port;			//端口配置 
	int single;			//单核模式
	int core;			//内核数量
};
int nsf_config(struct nsf_config *config);
#endif
