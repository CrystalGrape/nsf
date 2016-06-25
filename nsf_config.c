#include "nsf_config.h"
#include <string.h>

int nsf_config(struct nsf_config *config)
{
	char seagment[20];
	char value[20];
	FILE *fp=fopen(CONFIG_PATH,"r");
	if(!fp)
		return -1;
	while(!feof(fp)){
		fscanf(fp,"%s %s",seagment,value);
		if(strcmp(seagment,"port") == 0)
			config->port=atoi(value);
		if(strcmp(seagment,"core") == 0)
			config->core=atoi(value);
		if(strcmp(seagment,"single") == 0)
			config->single=atoi(value);
	}
	fclose(fp);
	return 0;
}
