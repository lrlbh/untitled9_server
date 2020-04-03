#pragma once
#include<iostream>
#include<mysql/mysql.h>
#define MY_SQL_PORT 3306
#define MY_SQL_HOST "49.234.181.2"
#define	MY_SQL_USER_NAME "root"
#define MY_SQL_USER_PASSWD "go456"
#define DB_NAME "sjs"

class debug_and_log_lr;
class my_sql
{
public:
	//构造，和mysql数据库建立连接
	my_sql(int port = MY_SQL_PORT,
		std::string db_name =DB_NAME,
		std::string host = MY_SQL_HOST,
		std::string user_name = MY_SQL_USER_NAME,
		std::string user_passwd = MY_SQL_USER_PASSWD);

	//执行sql语句，MYSQL_RES需要自己释放
	MYSQL_RES* sql_exec(std::string sql);
	

	~my_sql();

private:
	debug_and_log_lr* my_error;
	int port;
	std::string host;
	std::string	user_name;
	std::string user_passwd;
	std::string db_name;
	MYSQL* mysql = NULL;



};

