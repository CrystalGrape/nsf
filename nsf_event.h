#ifndef nsf_h
#define nsf_h
//可以使用的事件范围(0到EVENT_AMOUNT)
#define EVENT_AMOUNT			100

#define NE_ACPT					0
#define NE_RECV					1
#define NE_QUIT					2
#define NE_INIT					3

#define BUFFER_LEN				8096
#pragma pack(push)
#pragma pack(1)
typedef struct
{
	int msg;					//消息号
	int cfd;					//来源套接字
	int datalen;				//数据长度
	char data[BUFFER_LEN];		//数据
}NsfntPkg;
#pragma pack(pop)

//事件回调函数指针
typedef void(*NsfftEvent)(int, NsfntPkg);

struct nsf_event_node
{
	NsfftEvent nevent;
	struct nsf_event_node *ptr_next;
};
/*事件定义*/
typedef struct
{
	int amount;									//当前回调数量
	struct nsf_event_node *ptr_event_list;		//事件表
}NsfntEvent;


struct msg_node
{
	int event;
	NsfntPkg pkg;
	struct msg_node *ptr_next;
};

void nsf_event_init(int core);
void nsf_free_event();
int nsf_register_event(int event, NsfftEvent cblk);
int nsf_call_event(int event, NsfntPkg pkg);
int nsf_post_event(int event, NsfntPkg pkg);
int nsf_get_event(int *event, NsfntPkg *pkg);
void *nsf_event_loop(void *arg);
void nsf_signal_handler(int signo);
int nsf_check_exit();
void nsf_unlink_sem();
#endif
