#ifndef types_h
#define types_h

#define _out(var) var
//用户映射表
struct nsf_user_map
{
	char user_id[32];
	int clt_fd;
};
#endif
