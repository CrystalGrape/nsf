#include "module.h"
#include <dlfcn.h>
static WINSTANCE wInstance;
static MINSTANCE mInstance;
static int plg_amount = 0;					//插件数量
static void *plg_handle_list[200];		//最多200个插件
//加载插件
void *nsf_load_module(const char *path)
{
	int index;
	void *hModule;
	hModule = dlopen(path, RTLD_LAZY);
	if(!hModule){
		printf("open plugin [%s] error\n",path);
		return NULL;
	}
	int *type = (int *)dlsym(hModule,"_mtype_");
	char *init_func = (char *)dlsym(hModule,"_init_");
	char *proc_func = (char *)dlsym(hModule,"_proc_");
	if(type == 0||init_func==0||proc_func == 0){
		printf("cann't find symbol\n");
		dlclose(hModule);
		return NULL;
	}
	if(*type == 0){
		MINIT minit_func = (MINIT)dlsym(hModule,init_func);
		if(minit_func == 0)
		{
			printf("cann't find master symbol\n");
			dlclose(hModule);
			return NULL;
		}
		minit_func(mInstance);
		NsfftMsgproc mproc_func = (NsfftMsgproc)dlsym(hModule,proc_func);
		if(mproc_func == 0)
		{
			printf("cann't find master symbol\n");
			dlclose(hModule);
			return NULL;
		}
		nsf_register_msgproc(mproc_func);
	}
	if(*type == 1){
		WINIT winit_func = (WINIT)dlsym(hModule,init_func);
		if(winit_func == 0)
		{
			printf("cann't find worker symbol\n");
			dlclose(hModule);
			return NULL;
		}
		winit_func(wInstance);
		NsfftEvent wproc_func = (NsfftEvent)dlsym(hModule,proc_func);
		if(wproc_func == 0)
		{
			printf("cann't find worker symbol\n");
			dlclose(hModule);
			return NULL;
		}
		//注册事件
		int *wevl = (int *)dlsym(hModule,"_evl_");
		if(wevl == 0)
		{
			printf("cann't find worker symbol\n");
			dlclose(hModule);
			return NULL;
		}
		for(index = 0; wevl[index] != 101; index++){
			nsf_register_event(wevl[index], wproc_func);
		}
	}
	return hModule;
}

//重新装载工作进程插件
void nsf_workermodule_reload()
{
	int i;
	char type[100];
	char path[100];
	if(plg_amount != 0){						//如果已经存在插件，先释放所有事件
		nsf_free_event();
		for(i = 0; i < plg_amount; i++){
			dlclose(plg_handle_list[i]);			//关闭所有插件
		}
		plg_amount = 0;
	}

	FILE *fp=fopen(MODULE_CONFIG_PATH,"r");
	if(!fp)
		return;
	while(!feof(fp)){
		memset(type,0,100);
		memset(path,0,100);
		fscanf(fp,"%s %s", type, path);
		if(strcmp(type, "worker") == 0)
		{
			//printf("[%d]:load plugin %s amount %d\n",getpid(), path, plg_amount);
			plg_handle_list[plg_amount] = nsf_load_module(path);
			if(plg_handle_list[plg_amount] != NULL)
				plg_amount++;
		}
	}
	fclose(fp);
	printf("[%d]:reload module!\n", getpid());
}

void nsf_workermodule_init()
{
	wInstance.nsf_post_event = nsf_post_event;
	wInstance.nsf_get_event = nsf_get_event;
	wInstance.nsf_post_msg = nsf_post_msg;
	nsf_workermodule_reload();
}


void nsf_mastermodule_init()
{
	char type[100];
	char path[100];
	mInstance.nsf_notification = nsf_notification;
	FILE *fp=fopen(MODULE_CONFIG_PATH,"r");
	if(!fp)
		return;
	while(!feof(fp)){
		fscanf(fp,"%s %s", type, path);
		if(strcmp(type, "master") == 0)
		{
			nsf_load_module(path);
		}
	}
	fclose(fp);
}



