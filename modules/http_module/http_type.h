#ifndef http_type_h
#define http_type_h
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <memory.h>

typedef struct http_string_t
{
	int strlen;
	char *data;
}http_string_t;

typedef struct http_header_t
{
	http_string_t request;
	http_string_t agent;
	http_string_t doctype;
	http_string_t doclang;
	http_string_t ecncode;
	http_string_t connstate;
}http_header_t;

typedef struct http_request_t
{
	http_string_t method;
	http_string_t url;
	http_string_t version;
}http_request_t;

http_header_t *http_get_header(const char *request);
void http_get_line(http_string_t *dest, const char *request, int line);
void http_header_free(http_header_t *header);
http_request_t *http_get_request(http_string_t request);
void http_request_free(http_request_t *request);
#endif
