# nsf
模仿nginx架构的TCP服务器，仅供娱乐  

整个架构采用的是事件驱动机制,模块化设计，框架仅实现了数据接入层代码   
，所有业务逻辑代码，包括数据库都需要用户自定义模块实现。  
##添加一个nsf处理模块    
1.在modules目录下添加一个test.c文件，包含头文件#include "nsf_event.h"   
2.nsf模块需要必须实现一个消息回调函数，函数定义如下   
	void onRecv(int event,NsfntPkg pkg);   
3.nsf采用事件驱动机制，但是使用的事件号有限支持0-100,0号事件是框架内   
保留事件，表示收到数据。   
4.比如我们创建一个收到消息，回发的事件处理   
	>void onRecv(int event,NsfntPkg pkg)   
	>{   
	>>	if(event == 0)     
	>>		write(pkg.cfd, pkg.data, pkg.datalen);   
	>}   
5.写完后保存文件，打开module_init.c，添加   
	>extern void onRecv(int event,NsfntPkg pkg);   
6.在void nsf_module_init()函数中添加   
	>nsf_register_event(0,onRecv);   
  >保存   
7.修改Makefile加入   
	>test.o:   
	>>	gcc -c test.c ${INC}  
  >add标签后加入 test.o   
8.回到根目录，make编译   
