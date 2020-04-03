#include<iostream>


//将当前进程设置为守护进程
class daemob
{
public:
	//参数1：是否将当前进程设置为守护进程
	//参数2：指定进程工作目录
	daemob(bool open,const char *path );

};