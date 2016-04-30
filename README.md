# nsf
模仿nginx架构的TCP服务器，仅供娱乐

整个架构采用的是事件驱动机制,模块化设计，框架仅实现了数据接入层代码\<br>
，所有业务逻辑代码，包括数据库都需要用户自定义模块实现。\<br>
##添加一个nsf处理模块\<br>
1.在modules目录下添加一个test.c文件，包含头文件#include "nsf_event.h"\<br>
2.nsf模块需要必须实现一个消息回调函数，函数定义如下\<br>
	void onRecv(int event,NsfntPkg pkg);\<br>
3.nsf采用事件驱动机制，但是使用的事件号有限支持0-100,0号事件是框架内\<br>
保留事件，表示收到数据。\<br>
4.比如我们创建一个收到消息，回发的事件处理\<br>
	void onRecv(int event,NsfntPkg pkg)\<br>
	{\<br>
		if(event == 0)\<br>
			write(pkg.cfd, pkg.data, pkg.datalen);\<br>
	}\<br>
5.写完后保存文件，打开module_init.c，添加\<br>
	extern void onRecv(int event,NsfntPkg pkg);\<br>
6.在void nsf_module_init()函数中添加\<br>
	nsf_register_event(0,onRecv);\<br>
  保存\<br>
7.修改Makefile加入\<br>
	test.o:\<br>
		gcc -c test.c ${INC}\<br>
  add标签后加入 test.o\<br>
8.回到根目录，make编译\<br>
