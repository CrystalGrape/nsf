#ifndef NMYSQL_H
#define NMYSQL_H
#include <mysql/mysql.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
int nsf_mysql_connect(MYSQL *mysql, const char *hostip, const char* username, const char *pwd, const char *dbname);
int nsf_mysql_writequery(MYSQL *mysql, const char *query);
int nsf_mysql_close(MYSQL *mysql);
#endif
