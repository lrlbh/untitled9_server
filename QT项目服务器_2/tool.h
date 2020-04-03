#pragma once
#include<iostream>
#include<mysql/mysql.h>
#include <curl/curl.h>
#include <stdarg.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#define MY_SQL_PORT 3306
#define MY_SQL_HOST "49.234.181.2"
#define	MY_SQL_USER_NAME "root"
#define MY_SQL_USER_PASSWD "go456"
#define DB_NAME "sjs"

#define MAXPARAM 2048

class debug_and_log_lr;

//连接mysql类
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

//发送短信的类
class sms
{
public:
	sms();
	int send_sms(char* mobile,char* n);
	~sms();
private:
	char* url_send_sms = "https://sms.yunpian.com/v2/sms/single_send.json";
	char* apikey = "c38f2462fb9eb4b3abd874032a584a29";
	CURL* curl;
	CURLcode res;
	debug_and_log_lr* my_error;
	

	int send_data(char* url, char* data);
};

//创建守护进程的类
//将当前进程设置为守护进程
class daemob
{
public:
	//参数1：是否将当前进程设置为守护进程
	//参数2：指定进程工作目录
	daemob(bool open, const char* path);

};

