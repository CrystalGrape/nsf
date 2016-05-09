#include "nsf_epoll.h"
#include "nsf_event.h"
#include "nsf_master.h"
#include <signal.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/prctl.h>

//0 用户连接 1 收到数据 2 用户退出
sem_t *sem;     	//监听套接字互斥信号量
sem_t *sem_write;	//共享互斥信号量
int shmid;      	//共享内存id
int epollfd;		//epoll套接字集合

extern void nsf_module_init(); 
/*设置套接字非阻塞*/
int setnoblocking(int fd)
{
	int opts;
    opts = fcntl(fd, F_GETFL);
    if(opts < 0)
        return -1;
    opts = opts | O_NONBLOCK;
    if(fcntl(fd, F_SETFL, opts) < 0)
    	return -2;
    return 0;
}

/*把套接字加入到监听队列*/
void nsf_add_epoll(int fd)
{
	struct epoll_event ev;
	ev.data.fd = fd;
    ev.events = EPOLLIN;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
}

/*从监听队列去掉套接字*/
int nsf_delete_epoll(int client)
{
	struct epoll_event event_del;  
	event_del.data.fd = client;
	event_del.events = 0;  
	epoll_ctl(epollfd, EPOLL_CTL_DEL, event_del.data.fd, &event_del);  
	return event_del.data.fd;
}

void nsf_epoll_loop(int listenfd, int core)
{
	int i;
	int all_cfd;
	int own_cfd = 0;						    //本进程拥有的客户端套接字数量
	socklen_t sin_size;	
	struct sockaddr_in addr_c;					//客户端地址
	int len;
	struct epoll_event events[MAXEVENTS];		//epoll事件集合
	NsfntPkg pkg;								//消息包
	int noticefd;								//通知频道
	struct nsf_notification_message msg;
	
	noticefd = nsf_create_workerclt();
	if(noticefd < 0)
		exit(0);
	epollfd=epoll_create(MAXFD);
	nsf_add_epoll(noticefd);
	
	memset(&pkg, 0, sizeof(pkg));
	nsf_post_msg(NM_CONNECT, pkg);
	
	while(1){
		//如果抢到信号量,则将监听套接字加入epoll中
		all_cfd = nsf_read_user();
		
		if(all_cfd >= own_cfd*core || all_cfd == 0){
			if(sem_trywait(sem) == 0){
				nsf_add_epoll(listenfd);
			}
		}
		
    	int nfds = epoll_wait(epollfd, events, MAXEVENTS, 200);
    	for(i = 0; i < nfds; i++){
    		if(noticefd == events[i].data.fd){
    			memset(&msg, 0, sizeof(msg));
    			len=read(noticefd,&msg,sizeof(msg));
    			if(len < 0)
    				continue;
    			nsf_default_workerproc(msg);
    			continue;
    		}
    		//有客户端链接
    		if(listenfd==events[i].data.fd){
    			memset(&pkg, 0, sizeof(pkg));
    			int cfd=accept(listenfd, (struct sockaddr *)&addr_c, &sin_size);
    			if(cfd == -1)
    				continue;
    			nsf_add_epoll(cfd);
    			nsf_delete_epoll(listenfd);
    			
    			//增加用户总套接字
    			all_cfd = nsf_read_user();
    			all_cfd++;
    			nsf_write_user(all_cfd);
    			own_cfd++;
    			sem_post(sem);
				
				//发送一个连接事件
    			pkg.cfd = cfd;
    			pkg.msg = NE_ACPT;
    			nsf_post_event(NE_ACPT, pkg);
    			
                continue;
    		}
    		
    		//从客户端收到消息
    		if(events[i].events&EPOLLIN){
    			memset(&pkg, 0, sizeof(pkg));
    			int tmpfd=events[i].data.fd;
    			len=read(events[i].data.fd,pkg.data,BUFFER_LEN);
    			if(len<=0){
    				nsf_delete_epoll(tmpfd);
					close(tmpfd);
					
					//总用户套接字减少
					all_cfd = nsf_read_user();
					all_cfd--;
					nsf_write_user(all_cfd);
					own_cfd--;
					
					//发送用户退出事件
					pkg.cfd = tmpfd;
					pkg.msg = NE_QUIT;
					nsf_post_event(NE_QUIT, pkg);
    			
    				continue;
    			}
    			//发送数据接收事件
    			pkg.data[len] = '\0';
    			pkg.datalen = len;
    			pkg.cfd = tmpfd;
    			pkg.msg = NE_RECV;
    			nsf_post_event(NE_RECV, pkg);
    			continue;
    		}
    	}
    }
}

int nsf_server_init(int port)
{
	struct sockaddr_in addr_s;
	int sfd=socket(AF_INET,SOCK_STREAM,0);
	if(sfd==-1)
		return -1;
	bzero(&addr_s,sizeof(addr_s));
	addr_s.sin_family=AF_INET;
	addr_s.sin_addr.s_addr=htonl(INADDR_ANY);
	addr_s.sin_port=htons(port);
	if(-1==bind(sfd,(struct sockaddr *)(&addr_s),sizeof(struct sockaddr)))
		return -2;  //bind fail;
	if(-1==listen(sfd,MAXFD))
		return -3;   //listen fail
	return sfd;
}

void nsf_start_epoll(int sfd, int core)
{
	int pid;
	if((pid=fork())==0){
		nsf_close_mastersrv();
		prctl(PR_SET_NAME, "nsf_worker", NULL, NULL, NULL);
		nsf_event_init(core);
		nsf_module_init();
		nsf_epoll_loop(sfd, core);
		exit(0);
	}
}

void nsf_start_master(int sfd, int core)
{
	signal(SIGCHLD,nsf_signal_handler);
	signal(SIGINT,nsf_signal_handler);
	prctl(PR_SET_NAME, "nsf_master", NULL, NULL, NULL);
	while(1)
	{
		if(nsf_check_exit()){
			nsf_start_epoll(sfd, core);
		}
		nsf_srvepoll();
	}
}

int nsf_start_worker(int sfd, int core)
{
	int i;
	
	sem = sem_open("listen", O_RDWR|O_CREAT, 00777, 1);
	sem_write = sem_open("sem_write", O_RDWR|O_CREAT, 00777, 1);
	shmid = shmget(IPC_PRIVATE,8196,IPC_EXCL|0660);
	
	if(shmid < 0){
		return -1;
	}
	
	nsf_write_user(0);
	for(i = 0; i < core; i++){
		nsf_start_epoll(sfd, core);
	}
	
	return 0;
}

void nsf_write_user(int count)
{
	sem_wait(sem_write);
	int *write = (int *)shmat(shmid,NULL,0);
	(*write) = count;
	shmdt((void *)write);
	sem_post(sem_write);
}

int nsf_read_user()
{
	int count;
	sem_wait(sem_write);
	int *read = (int *)shmat(shmid,NULL,0);
	count = *read;
	shmdt((void *)read);
	sem_post(sem_write);
	return count;
}

void nsf_unlink_sem()
{
	sem_unlink("listen");
	sem_unlink("sem_write");
	sem_close(sem);
	sem_close(sem_write);
}


