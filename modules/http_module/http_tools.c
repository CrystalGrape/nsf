#include "http_type.h"

http_header_t *http_get_header(const char *request)
{
	http_header_t *header 
		= (http_header_t *)malloc(sizeof(http_header_t));
	http_get_line(&(header->request), request, 0);
	http_get_line(&(header->agent), request, 1);
	http_get_line(&(header->doctype), request, 2);
	http_get_line(&(header->doclang), request, 3);
	http_get_line(&(header->ecncode), request, 4);
	http_get_line(&(header->connstate), request, 5);
	return header;
}

void http_get_line(http_string_t *dest, const char *request, int line)
{
	char 	buffer[1024];
	int 	buffer_len = 0;
	int 	linecount = 0;
	int 	index;
	
	for(index = 0; index < strlen(request); index++){
		if(request[index] == '\r' && request[index+1] == '\n'){
			if(linecount == line){
				buffer[buffer_len] = '\0';
				break;
			}
			linecount++;
			buffer_len = 0;
			index += 2;
		}
		buffer[buffer_len] = request[index];
		buffer_len++;
		buffer[buffer_len] = '\0';
	}
	dest->strlen = buffer_len;
	dest->data = (char *)malloc(buffer_len + 1);
	memset(dest->data, 0, buffer_len);
	memcpy(dest->data, buffer, buffer_len);
}

void http_header_free(http_header_t *header)
{
	free(header->request.data);
	free(header->agent.data);
	free(header->doctype.data);
	free(header->doclang.data);
	free(header->ecncode.data);
	free(header->connstate.data);
	free(header);
}

http_request_t *http_get_request(http_string_t request)
{
	char 	buffer[1024];
	int 	blen = 0;
	int 	index;
	int		scount = 0;
	http_request_t *req 
		= (http_request_t *)malloc(sizeof(http_request_t));
		
	for(index = 0; index < request.strlen; index++){
		if(request.data[index] == ' '){
			if(scount == 0){
				req->method.strlen = blen;
				req->method.data = (char *)malloc(blen + 1);
				memset(req->method.data, 0, blen + 1);
				memcpy(req->method.data, buffer, blen);
			}
			if(scount == 1){
				req->url.strlen = blen;
				req->url.data = (char *)malloc(blen + 1);
				memset(req->url.data, 0, blen + 1);
				memcpy(req->url.data, buffer, blen);
			}
			index++;
			scount++;
			blen = 0;
		}
		buffer[blen] = request.data[index];
		buffer[blen + 1] = '\0';
		blen++;
	}
	
	req->version.strlen = blen;
	req->version.data = (char *)malloc(blen + 1);
	memset(req->version.data, 0, blen + 1);
	memcpy(req->version.data, buffer, blen);
	return req;
}
void http_request_free(http_request_t *request)
{
	free(request->method.data);
	free(request->url.data);
	free(request->version.data);
	free(request);
}






