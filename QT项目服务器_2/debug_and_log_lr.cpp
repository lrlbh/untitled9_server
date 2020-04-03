#include "debug_and_log_lr.h"
#include<errno.h>
#include <time.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
using namespace std;


debug_and_log_lr* debug_and_log_lr::dl_lr = new debug_and_log_lr;
debug_and_log_lr::~debug_and_log_lr()
{
}

void debug_and_log_lr::error_lr(const char* err, const char* err1 , const char* err2 )
{
	if ((this->out_lr << 6) >> 7)
	{
		cout << err << " --> ";
		if (err1)
			cout << err1 << " --> ";
			
		if (err2)
			cout << err2 << " --> ";

		cout <<"错误号:"<<errno<<" --> "
			<<"错误提示:" <<strerror(errno) << endl;
	}

}

void debug_and_log_lr::log_lr(const char* err, const char* err1 , const char* err2 )
{
	if ((this->out_lr << 7) >> 7)
	{
		FILE* fp = fopen(this->path_lr, "a");
		if (fp == NULL)
			return;
		this->up_time_lr();

		fprintf(fp, "%d/%d/%d  %d:%d:%d --> %s --> ",
			this->lt.tm_year, this->lt.tm_mon, this->lt.tm_mday,
			this->lt.tm_hour, this->lt.tm_min, this->lt.tm_sec, err);

		if (err1)
			fprintf(fp, "%s --> ",err1);
		
		if (err2)
			fprintf(fp, "%s --> ", err2);

		fprintf(fp, "错误号:%d --> 错误提示:%s\n", errno,strerror(errno));

		fclose(fp);
	}
}

void debug_and_log_lr::log_error_lr(const char* err, const char* err1 , const char* err2 )
{
	this->error_lr(err,err1,err2);
	this->log_lr(err,err1,err2);
}

void debug_and_log_lr::run_print(const char* str, const char* str1, const char* str2)
{
	if ((this->out_lr << 5) >> 7)
	{
		cout << str ;
		if (str1)
			cout << " --> " << str1 ;

		if (str2)
			cout << " --> " << str2 ;

		cout << endl;
	}

}

void debug_and_log_lr::show_bin(const unsigned char* bin, int len)
{
	if (len == 0)
		len = 100;
	for (int i = 0; i < len; i++)
	{
		char* temp_bin = (char*)bin + i;
		printf("%d:", i + 1);
		for (int j = 0; j < 8; j++)
		{

			unsigned char temp2 = *temp_bin << j;
			if (temp2 >> 7 == 0)
			{
				printf("0");
			}
			else if (temp2 >> 7 == 1)
			{
				printf("1");
			}
			else
			{
				printf("位移发送错误！\n");
				return;
			}
		}
		printf("\n");
	}
}

void debug_and_log_lr::is_0B0(const unsigned char* bin, int len)
{
	int is_0 = 0;
	if (len == 0)
		len = 100;
	for (int i = 0; i < len; i++)
	{
		is_0 = 0;
		char* temp_bin = (char*)bin + i;
		printf("%d：", i);
		for (int j = 0; j < 8; j++)
		{

			unsigned char temp2 = *temp_bin << j;
			if (temp2 >> 7 == 0)
			{
				printf("0");
				is_0++;
			}
			else if (temp2 >> 7 == 1)
			{
				printf("1");
			}
			else
			{
				printf("位移发送错误！\n");
				return;
			}

			if (j == 7)
			{
				if (is_0 >= 8)
				{
					//cout << len - 10;
					printf("有零");
					return;
				}
			}
		}
		printf("\n");
	}
	printf("\n");


}


debug_and_log_lr* debug_and_log_lr::get_error_lr()
{
	return dl_lr;
}


void debug_and_log_lr::up_time_lr()
{
	//获取系统时间
	time_t t;
	struct tm* temp_lt;
	time(&t);//获取Unix时间戳。
	temp_lt = localtime(&t);//转为时间结构。
	temp_lt->tm_year = temp_lt->tm_year + 1900;
	temp_lt->tm_mon = temp_lt->tm_mon + 1;

	//更新时间
	this->lt.tm_year = temp_lt->tm_year;
	this->lt.tm_mon = temp_lt->tm_mon;
	this->lt.tm_mday = temp_lt->tm_mday;
	this->lt.tm_hour = temp_lt->tm_hour;
	this->lt.tm_min = temp_lt->tm_min;
	this->lt.tm_sec = temp_lt->tm_sec;
}


void debug_and_log_lr::up_out_lr(int mode)
{
	this->out_lr= (unsigned char)mode;
}

void debug_and_log_lr::set_path_lr(const char* path)
{
	memset(this->path_lr,0,strlen(this->path_lr));
	strcpy(this->path_lr, path);
	strcat(this->path_lr,"/log_lr.log");
}

debug_and_log_lr::debug_and_log_lr()
{
	strcpy(this->path_lr, "./log_lr.log");
}

debug_and_log_lr::debug_and_log_lr(const debug_and_log_lr& e)
{
}
