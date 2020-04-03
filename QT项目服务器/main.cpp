#include<iostream>
#include"server.h"
#include<queue>
#include<string>
#include"debug_and_log_lr.h"
#include"protocol_distribute.h"
#include"daemon.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;

debug_and_log_lr* my_error;

//函数功能：获取务器异常、崩溃状态,有时间删掉一个无效打印
void servers_status()
{
	int wstatus = 0;
	//pid_t error_ppid = wait(&wstatus);
	wait(&wstatus);
	if (WIFSIGNALED(wstatus))
	{
		char error_buf[60] = { 0 };
		sprintf(error_buf, "servers_error_voer:SIG=[%d]:RETURN=[%d]:",
			WTERMSIG(wstatus), WEXITSTATUS(wstatus));
		my_error->log_error_lr(error_buf);
	}

	if (WIFEXITED(wstatus))
	{
		char error_buf[60] = { 0 };
		sprintf(error_buf, "servers_normal_voer:SIG=[%d]:RETURN=[%d]:",
			WTERMSIG(wstatus), WEXITSTATUS(wstatus));
		my_error->log_error_lr(error_buf);
	}
}


int main()
{

	srand(time(NULL));
	daemob d(true, "./");
	//打印错误对象
	my_error = debug_and_log_lr::get_error_lr();
	my_error->up_out_lr(YES_DEBUG | YES_LOG | YES_RUN);


	pid_t pid = 0;
	//服务器终止后重启服务器
servers_reboot:

	pid = fork();
	//父进程获取服务器崩溃、退出情况
	if (pid > 0)
	{
		servers_status();
		sleep(10);
		goto servers_reboot;
	}
	//子进程  服务器
	else if (pid == 0)
	{
		//启动服务器
		server s(8000);
		//保存一个或多个套接字的，一条或者多条消息
		map<int, queue<string>> map_message;
		//消息分发对象
		protocol_distribute pd;

		while (1)
		{
			//这里是拷贝构造值传递，有时间重新设计一下
			//读
			map_message = s.wait_message();

			//处理
			pd.ststart(map_message);


		}
	}
	else
	{
		my_error->log_error_lr("main()->fork():");
	}




	

}
