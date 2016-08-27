#include "module.h"
#include "http_type.h"
#define WWW_PATH	"/home/jay/www"
static WINSTANCE wInstance;
EVENT_START(NE_RECV),
EVENT_END();

void handle_http_request(NsfntPkg pkg);
void do_http_get(int cfd, http_request_t *req);
void not_found(int client);
void worker_init(WINSTANCE w)
{
	memcpy(&wInstance,&w,sizeof(w));
}


void worker_proc(int event, NsfntPkg pkg)
{
	switch(event){
	case NE_RECV:
		handle_http_request(pkg);
		break;
	}
}
char *http_ok="HTTP/1.1 200 OK\r\n";
char *content_type="text/html; charset=utf-8\r\n";
char *server="NiServerFramework 4.0.0\r\n";

void handle_http_request(NsfntPkg pkg)
{
	http_header_t *header = http_get_header(pkg.data);
	http_request_t *req = http_get_request(header->request);
	if(strcmp(req->method.data, "GET") == 0){
		do_http_get(pkg.cfd, req);
	}
	if(strcmp(req->method.data, "POST") == 0){
	
	}
	http_header_free(header);
	http_request_free(req);
}

void do_http_get(int cfd, http_request_t *req)
{
	char 	url[1024];
	char 	path[2046];
	int 	index;
	char 	content_length[1024];
	
	for(index = 0; index < req->url.strlen; index++){
		if(req->url.data[index] == '?')
			break;
		url[index] = req->url.data[index];
	}
	url[index] = '\0';
	
	if(url[strlen(url) - 1] == '/'){
		strcat(url,"index.html");
	}
    
    write(cfd,http_ok,strlen(http_ok));
    write(cfd,server,strlen(server));
    write(cfd,content_type,strlen(content_type));
  
    sprintf(path, "%s%s", WWW_PATH, url);
    printf("%s\n", path);
	FILE *fp=fopen(path, "r");
	if(fp <= 0){
		not_found(cfd);
	}
    fseek(fp, 0L, SEEK_END);
    int filesize = ftell(fp);
    fseek(fp,0L,SEEK_SET);
    sprintf(content_length,"Content-Length: %d\r\n\r\n", filesize);
    printf("%s\n", content_length);
    write(cfd,content_length,strlen(content_length));
    int len;
    char buffer[1025];
    while((len = fread(buffer, 1, 1024, fp)) > 0){
    	printf("%d\n", len);
    	write(cfd, buffer, len);
    	memset(buffer, 0, 1025);
    }  
    printf("send over\n");  
}

void not_found(int client)
{
	char buf[1024];
	sprintf(buf, "HTTP/1.0 404 NOT FOUND\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, server);
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "Content-Type: text/html\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "<HTML><TITLE>Not Found</TITLE>\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "<BODY><P>The server could not fulfill\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "your request because the resource specified\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "is unavailable or nonexistent.\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "</BODY></HTML>\r\n");
	send(client, buf, strlen(buf), 0);
}
WORKER_MODULE_INIT(worker_init);
MODULE_PROC(worker_proc);
