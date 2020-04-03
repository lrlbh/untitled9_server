#include "tool.h"
#include<iostream>
#include<mysql/mysql.h>
#include"debug_and_log_lr.h"
#include<string>
#include"string.h"


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>



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




sms::sms()
{
	this->my_error = debug_and_log_lr::get_error_lr();


	this->curl = curl_easy_init();

}

int sms::send_sms(char* mobile,char* n)
{

	mobile = curl_easy_escape(this->curl, mobile, strlen(mobile));
	if (NULL == this->curl)
	{ 
		this->my_error->error_lr("sms", "curl_easy_escape()");
		return -1;
	}
	//生成验证码
	string temp_text = "【云片网】您的验证码是";
	temp_text.append(n);


	char params[MAXPARAM + 1];
	char* cp = params;

	sprintf(params, "apikey=%s&mobile=%s&text=%s", this->apikey, mobile, temp_text.data());
	return send_data(url_send_sms, params);

}

sms::~sms()
{
	curl_easy_cleanup(this->curl);
}

int sms::send_data(char* url, char* data)
{
	int ret_int = -1;
	// specify the url
	curl_easy_setopt(this->curl, CURLOPT_URL, url);
	// specify the POST data
	curl_easy_setopt(this->curl, CURLOPT_POSTFIELDS, data);
	//重定向输出到文件
	FILE* temp = freopen("temp_sms_return.txt","w",stdout);
	// get response data
	curl_easy_perform(this->curl);
	//重定向输出
	freopen("/dev/tty", "w", stdout);

	//打开保存返回数据的文件
	FILE* temp_read = fopen("temp_sms_return.txt","r");
	//保存文件中读取到的数据
	char temp_ret[256] = {0};
	//读取文件中的数据
	fgets(temp_ret, sizeof(temp_ret), temp_read);
	//关闭文件指针
	if (temp_read!=NULL)
	{
		fclose(temp_read);
	}
	//查找指定内容
	if (strstr(temp_ret, "\"code\":0")!=NULL)		//发送成功
		ret_int = 0;
	else if (strstr(temp_ret, "\"code\":33") != NULL)	//每个号码发送30秒内只能发一次
		ret_int = 33;
	else if (strstr(temp_ret, "\"code\":2") != NULL && strstr(temp_ret, "\"code\":22") == NULL)	//手机号码有误
		ret_int = 2;
	else if (strstr(temp_ret, "\"code\":22") != NULL)	//每个号码1小时最多发送3次
		ret_int = 22;
	else
		ret_int = -1;							//其他错误

	cout << "返回值" << temp_ret <<endl;
	cout << "验证码状态" <<ret_int<< endl;


	//关闭文件
	//if (temp != NULL)
	//{
	//	fclose(temp);
	//}
	
	return ret_int;
}




daemob::daemob(bool open = false, const char* path = "./")
{
	if (open == false)
		return;

	//父进程fork子进程, 然后退出
	pid_t pid = fork();
	if (pid > 0 || pid < 0)
	{
		//perror("fork()");
		exit(0);
	}

	//子进程调用setsid函数创建会话
	setsid();

	//修改当前的工作目录
	chdir(path);

	//重设文件掩码
	umask(0000);

	//关闭文件描述符
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
}

