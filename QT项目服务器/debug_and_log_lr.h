#pragma once
#include<iostream>

//功能：
	//1、负责打印错误和错误日志


#define YES_LOG	0B00000001		//打印错误日志
#define YES_DEBUG 0B00000010	//打印错误
#define YES_RUN 0B00000100	//运行打印

//待更新功能
	//2、提供一个 获取当前打印标记 的方法


class debug_and_log_lr
{
public:
	~debug_and_log_lr();

	//打印错误
	void error_lr(const char* err, const char* err1 = NULL, const char* err2 = NULL);
	//打印错误日志
	void log_lr(const char* err,const char* err1 =NULL, const char* err2 = NULL);
	//打印错误日志和打印错误
	void log_error_lr(const char* err, const char* err1 = NULL, const char* err2 = NULL);
	//运行打印方便调试
	void run_print(const char* str, const char* str1 = NULL, const char* str2 = NULL);
	//打印二级制数据
	static void show_bin(const unsigned char* bin, int len);

	//判断数据中是否有'\0'
	static void is_0B0(const unsigned char* bin, int len);

	//返回单例对象
	static debug_and_log_lr* get_error_lr();
	
	//更新保存当前系统时间的 lt 成员的值
	void up_time_lr();

	//保存系统时间
	struct tm lt;

	//控制是否打印错误和错误日志,默认不打印
	void up_out_lr(int mode=0);

	//设置打印错误日志的路径
	void set_path_lr(const char* path);

	

private:
	//单例私有构造
	debug_and_log_lr();
	debug_and_log_lr(const debug_and_log_lr&);

	char path_lr[1024] = {0};

	//保存单例对象
	static debug_and_log_lr * dl_lr;	

	//标记，是否进行错误输出和错误日志输出，默认都输出,使用了c++14还是c++11的新特性
	unsigned char out_lr = YES_DEBUG|YES_LOG|YES_RUN;
};


