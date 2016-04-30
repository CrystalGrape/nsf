#ifndef nsf_epoll_h
#define nsf_epoll_h
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <memory.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>
#define MAXEVENTS 	100
#define MAXFD		10000
void nsf_epoll_loop(int listenfd, int core);
int nsf_server_init(int port);
void nsf_start_epoll(int sfd, int core);
int nsf_start_worker(int sfd, int core);
void nsf_start_master(int sfd, int core);
void nsf_write_user(int count);
int nsf_read_user();
#endif
