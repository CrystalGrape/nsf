#include "nsql.h"
#include <stdlib.h>
NSQL *nsf_create_nsqlobject(SQL type)
{
	NSQL *nsql = NULL;
	switch(type)
	{
	case NMYSQL:
		nsql = (NSQL *)malloc(sizeof(NSQL));
		nsql->connect = nsf_mysql_connect;
		nsql->write_query = nsf_mysql_writequery;
		nsql->close = nsf_mysql_close;
		break;
	default:
		break;
	}
	return nsql;
}

int nsf_nsql_connect(NSQL *nsql, const char *hostip, const char* username, const char *pwd, const char *dbname)
{
	return nsql->connect(&(nsql->mysql), hostip, username, pwd, dbname);
}

int nsf_nsql_writequery(NSQL *nsql, const char *query)
{
	return nsql->write_query(&(nsql->mysql), query);
}

int nsf_nsql_free(NSQL *nsql)
{
	nsql->close(&(nsql->mysql));
	free(nsql);
	return 0;
}
