#pragma once
#include<iostream>
#include"server.h"

//对应协议类发送数据时，传过来的
class send_data
{
public:
	send_data(std::string& data, ptr* ptr);
	std::string& m_data;
	ptr* m_ptr;
};

class debug_and_log_lr;

class send_message
{
public:
	send_message(send_data& data);

};
