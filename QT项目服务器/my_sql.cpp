#include "my_sql.h"
#include<iostream>
#include<mysql/mysql.h>
#include"debug_and_log_lr.h"
#include<string>

using namespace std;



my_sql::my_sql(int port, std::string db_name, std::string host, std::string user_name, std::string user_passwd)
{
	this->my_error = debug_and_log_lr::get_error_lr();
	this->port = port;
	this->db_name = db_name;
	this->host = host;
	this->user_name = user_name;
	this->user_passwd = user_passwd;


	//初始化mysql结构体
	this->mysql = NULL;
	this->mysql = mysql_init(NULL);
	if (this->mysql == NULL)
	{
		this->my_error->log_error_lr("mysql_init()", mysql_error(this->mysql));
	}

	//建立连接
	if (mysql_real_connect(
		this->mysql,		//mysql结构体
		this->host.data(),	//服务器IP
		this->user_name.data(),	//登陆服务器的用户名
		this->user_passwd.data(),//登陆服务器的密码/usr/lib64/mysql/
		this->db_name.data(),	//使用哪个数据库
		this->port,	//服务器使用端口
		NULL,		//如果服务器是以本地套接字部署的，这里填本地套接字地址
		0) == NULL)
	{
		//mysql只要初始化后就可以释放不会出错
		mysql_close(this->mysql);
		this->mysql == NULL;
		this->my_error->log_error_lr(
			"mysql_real_connect()",mysql_error(this->mysql));
	}

	//修改当前连接的字符集
	if (this->mysql != NULL)
	{
		if (mysql_set_character_set(this->mysql, "UTF8") == -1)
		{
			this->my_error->log_error_lr(
				"mysql_set_character_set()", mysql_error(this->mysql));

		}
	}






}

MYSQL_RES* my_sql::sql_exec(std::string sql)
{
	if (this->mysql ==NULL )
	{
		this->my_error->log_error_lr("mysql==NULL");
		return NULL;
	}

	//执行sql语句
	if (mysql_query(mysql, sql.data()) == 0)
	{
		//执行sql语句后，获取结果集
		MYSQL_RES* res = mysql_store_result(mysql);
		if (res ==NULL)
		{
			return NULL;
		}
		return res;
	}
	else
	{
		return NULL;
	}

	
}

my_sql::~my_sql()
{
	if (this->mysql!= NULL)
	{
		mysql_close(mysql);
	}
}


