#include "nsf_event.h"
#include <memory.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <time.h>
#include <stdio.h>
//全局变量区
pthread_mutex_t event_mutex;					 //事件互斥量
NsfntEvent event_list[EVENT_AMOUNT];
struct msg_queue
{
	struct msg_node *ptr_msg_head;				 //消息队列头指针
	struct msg_node *ptr_msg_tail;				 //消息队列尾指针
	int amount;									 //消息队列长度
	int dynamic;								 //动态最大长度
	int max;									 //消息队列最大长度
	int flag;									 //消息队列负载标志位
	sem_t mq_sem;								 //消息可读信号量
	pthread_mutex_t msg_mutex;					 //消息队列互斥量
	pthread_mutex_t sem_mutex;					 //信号互斥量
}mq;

//消息队列平衡方法
void nsf_mq_balance()
{
	if(mq.flag < -10){
		mq.dynamic = mq.dynamic/2;
		mq.flag = mq.flag/2;
	}
	if(mq.flag > 10){
		mq.dynamic = mq.dynamic*2;
		mq.flag = mq.flag/2;
	}
	if(mq.dynamic <= 10)
		mq.dynamic = 10;
	if(mq.dynamic >= mq.max)
		mq.dynamic = mq.max;
}

//事件表初始化
void nsf_event_init(int core)
{
	int i;
	memset(event_list, 0, EVENT_AMOUNT*sizeof(NsfntEvent));
	memset(&mq, 0, sizeof(struct msg_queue));
	
	mq.dynamic = 200;
	mq.max = 20000;
	mq.flag = 0;
		
	sem_init(&(mq.mq_sem),0,0);			//信号灯初始化为0
	pthread_mutex_init(&(mq.msg_mutex),NULL);
	pthread_mutex_init(&(mq.sem_mutex),NULL);
	pthread_mutex_init(&event_mutex,NULL);
	
	for(i = 0; i < core; i++){
		pthread_t tid;
		pthread_create(&tid,0,nsf_event_loop,0);
	}
}

//注册事件回调函数
int nsf_register_event(int event, NsfftEvent cblk)
{
	if(event < 0 || event >=EVENT_AMOUNT)
		return -1;
	struct nsf_event_node *ptr = (struct nsf_event_node *)malloc(sizeof(struct nsf_event_node));
	ptr->nevent = cblk;
	
	if(event_list[event].amount == 0){
		event_list[event].ptr_event_list = ptr;
		ptr->ptr_next = NULL;
	}else{
		ptr->ptr_next = event_list[event].ptr_event_list;
		event_list[event].ptr_event_list = ptr;
	}
	event_list[event].amount++;
	return 0;
}

//释放所有事件
void nsf_free_event()
{
	int i;
	
	struct nsf_event_node *front;
	struct nsf_event_node *next;
	pthread_mutex_lock(&event_mutex);
	for(i = 0; i < EVENT_AMOUNT;i++){
		if(event_list[i].amount == 0)
			continue;
		front = event_list[i].ptr_event_list;
		while(event_list[i].amount)
		{
			next = front->ptr_next;
			free(front);
			front = next;
			event_list[i].amount--;
		}
		event_list[i].ptr_event_list = NULL;
	}
	pthread_mutex_unlock(&event_mutex);
}

//调用事件回调函数
int nsf_call_event(int event, NsfntPkg pkg)
{
	int i;
	struct nsf_event_node *ptr;
	pthread_mutex_lock(&event_mutex);
	if(event < 0 || event >= EVENT_AMOUNT){
		pthread_mutex_unlock(&event_mutex);
		return -1;
	}
	if(event_list[event].amount == 0){
		pthread_mutex_unlock(&event_mutex);
		return -2;
	}
	ptr = event_list[event].ptr_event_list;

	for(i = 0; i < event_list[event].amount; i++){
		(ptr->nevent)(event,pkg);
		ptr = ptr->ptr_next;
	}
	pthread_mutex_unlock(&event_mutex);
	return 0;
}

//投递一个事件
int nsf_post_event(int event, NsfntPkg pkg)
{
	struct msg_node *ptr_msg = (struct msg_node *)malloc(sizeof(struct msg_node));
	ptr_msg->event = event;
	memcpy(&(ptr_msg->pkg), &pkg, sizeof(pkg));

	pthread_mutex_lock(&(mq.msg_mutex));

	if(mq.amount >= mq.dynamic){
		mq.flag++;
		nsf_mq_balance();
		if(mq.amount >= mq.dynamic){
			free(ptr_msg);
			pthread_mutex_unlock(&(mq.msg_mutex));
			return -1;
		}
	}
	
	if(mq.amount == 0){
		mq.ptr_msg_head = ptr_msg;
		mq.ptr_msg_tail = ptr_msg;
	}else{
		mq.ptr_msg_tail->ptr_next = ptr_msg;
		mq.ptr_msg_tail = ptr_msg;
	}
	mq.amount++;
	pthread_mutex_unlock(&(mq.msg_mutex));
	sem_post(&(mq.mq_sem));
	return 0;
}

//取出一个事件
int nsf_get_event(int *event, NsfntPkg *pkg)
{
	struct timespec ts;
	struct msg_node *ptr_msg;
	
	pthread_mutex_lock(&(mq.sem_mutex));
	ts.tv_sec=time(NULL) + 2;   // 重点
    ts.tv_nsec=0;
	if(sem_timedwait(&(mq.mq_sem), &ts) == -1){
		pthread_mutex_lock(&(mq.msg_mutex));
		mq.flag--;
		nsf_mq_balance();
		pthread_mutex_unlock(&(mq.msg_mutex));
		pthread_mutex_unlock(&(mq.sem_mutex));
		return -1;
	}
	pthread_mutex_unlock(&(mq.sem_mutex));
	
	//sem_wait(&(mq.mq_sem));				//有任务才进入
	pthread_mutex_lock(&(mq.msg_mutex));
	ptr_msg = mq.ptr_msg_head;
	if(mq.amount == 1)
		mq.ptr_msg_head = NULL;
	else
		mq.ptr_msg_head = ptr_msg->ptr_next;
	mq.amount--;
	pthread_mutex_unlock(&(mq.msg_mutex));
	
	(*event) = ptr_msg->event;
	memcpy(pkg, &(ptr_msg->pkg), sizeof(NsfntPkg));
	free(ptr_msg);
	
	return 0;
}

//事件循环
void *nsf_event_loop(void *arg)
{
	pthread_detach(pthread_self());
	while(1){
		int event;
		NsfntPkg pkg;
		if(nsf_get_event(&event, &pkg) == -1){
			continue;
		}
		nsf_call_event(event, pkg);		
	}
}
