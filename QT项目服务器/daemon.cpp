#include "daemon.h"
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

daemob::daemob(bool open =false,const char* path ="./")
{
	if (open ==false)
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
