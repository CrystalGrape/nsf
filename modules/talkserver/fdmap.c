/*
	套接字映射表
TODO
	插入方法：通过套接字，获取到map_num(用户映射号)、slot_num(内部槽映射号)
根据用户映射号找到映射节点，查看映射节点状态，如果映射槽空闲，为槽申请一个
用户id空间，将用户id写入并将相应位置一。
	查找方法：通过套接字，获取到map_num(用户映射号)、slot_num(内部槽映射号)
根据用户映射号找到映射节点，查看映射节点状态，如果找到了映射槽，返回用户id，
否则返回错误
	删除方法：通过套接字，获取到map_num(用户映射号)、slot_num(内部槽映射号)
根据用户映射号找到映射节点，查看映射节点状态，如果映射槽存在，释放用户id空
间并将相应位清零。否则什么都不做。
*/
#include <memory.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
//存储的最大套接字
#define		MAX_FDS		5000

struct fdmap_node
{
	char *slot[8];				//用户槽
	unsigned char state;		//表状态
}FdMap[MAX_FDS];

void nsf_fdmap_init()
{
	memset(FdMap, 0, MAX_FDS*sizeof(struct fdmap_node));
}

int nsf_fdmap_setbit(int map_num, int slot_num)
{
	FdMap[map_num].state = FdMap[map_num].state|(1<slot_num);
	return 0;
}

int nsf_fdmap_clrbit(int map_num, int slot_num)
{
	FdMap[map_num].state = FdMap[map_num].state&(~(1<slot_num));
	return 0;
}

int nsf_fdmap_bitstate(int map_num, int slot_num)
{
	unsigned char state = FdMap[map_num].state;
	
	if(state&(1<slot_num))
		return 1;
	else
		return 0;
}

int nsf_fdmap_insert(int fd, const char *id)
{
	int slot_num;
	int map_num;
	if(id == NULL)
		return -1;
	if(strlen(id) > 32 || strlen(id) <= 0)
		return -2;
		
	 slot_num = fd/MAX_FDS;
	 map_num = fd%MAX_FDS;
	 
	 if(slot_num > 8)
	 	return -3;
	 	
	 if(nsf_fdmap_bitstate(map_num, slot_num) == 1){
	 	return -4;
	 }
	 
	FdMap[map_num].slot[slot_num] = (char *)malloc(32*sizeof(char));
	strcpy(FdMap[map_num].slot[slot_num], id);
	nsf_fdmap_setbit(map_num, slot_num);
	return 0;
}

int nsf_fdmap_find(int fd, _out(char *str))
{
	int slot_num;
	int map_num;
	if(str == NULL)
		return -1;
		
	slot_num = fd/MAX_FDS;
	map_num = fd%MAX_FDS;
	
	if(nsf_fdmap_bitstate(map_num, slot_num) == 0)
		return -2;
	strcpy(str,FdMap[map_num].slot[slot_num]);
	return 0;
}

int nsf_fd_map_remove(int fd)
{
	int slot_num;
	int map_num;
		
	slot_num = fd/MAX_FDS;
	map_num = fd%MAX_FDS;
	
	if(nsf_fdmap_bitstate(map_num, slot_num) == 0)
		return -1;
	
	free(FdMap[map_num].slot[slot_num]);
	nsf_fdmap_clrbit(map_num, slot_num);
	return 0;
}

