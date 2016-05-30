#include "mysql/nmysql.h"
#define NMYSQL		0
typedef int SQL;
typedef struct nsf_nsql_connection
{
	MYSQL mysql;
	int (*connect)(MYSQL*, const char*, const char*, const char*, const char*);
	int (*write_query)(MYSQL*, const char*);
	int (*close)(MYSQL *);
}NSQL;

NSQL *nsf_create_nsqlobject(SQL type);
int nsf_nsql_connect(NSQL *nsql, const char *hostip, const char* username, const char *pwd, const char *dbname);
int nsf_nsql_writequery(NSQL *nsql, const char *query);
int nsf_nsql_free(NSQL *nsql);

