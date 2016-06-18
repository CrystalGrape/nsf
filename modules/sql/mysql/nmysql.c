#include "nmysql.h"

int nsf_mysql_connect(MYSQL *mysql, const char *hostip, const char* username, const char *pwd, const char *dbname)
{
	if(hostip==NULL||username==NULL||pwd==NULL||dbname==NULL)
		return 0;
	if(!mysql_real_connect(mysql,hostip,username,pwd,dbname,0,NULL,0))
	{
		return 1;
	}
	else 
		return 0;
}

int nsf_mysql_writequery(MYSQL *mysql, const char *query)
{
	int flag;
	
	if(query==NULL)
		return -1;
	int queryLen = strlen(query);
	flag=mysql_real_query(mysql,query,queryLen);
	return flag;
}

int nsf_mysql_close(MYSQL *mysql)
{
	mysql_close(mysql);
}
