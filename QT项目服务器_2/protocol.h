#pragma once
#include <iostream>
#include"server.h"

//protocol类，给其他对象传输需要处理的数据时使用
class one_message
{
public:
	one_message(std::string& data, ptr * ptr);
	std::string& m_data;
	ptr* m_ptr;
};

//当服务器，读取到一个套接字的多条数据时，使用该类获取每一条数据，分发给对应的类处理
class protocol
{
public:
	protocol(message &cfd_message);
};

//处理协议 OB00000010，从数据库读取一章道德经然后返回
class protocol_laozi
{
public:
	protocol_laozi(one_message& my_one_message);
private:
	debug_and_log_lr* my_error;
};

// 处理协议 OB00000001，计算出需要的数据，然后从数据库查询后返回
class protocol_zhouyi
{

public:
	protocol_zhouyi(one_message& my_one_message);

private:
	//获取一个爻
	int get_one_yao();
	//获取一个卦
	std::vector<int> get_one_gua(char* yao_yy);
	debug_and_log_lr* my_error;

};

//处理协议 OB00000100，客服端打开登陆页面时发送
class prorocol_log_in_ui
{
public:
	prorocol_log_in_ui(one_message& my_one_message);
private:
	debug_and_log_lr* my_error;
};

//处理协议 OB00000101，客服端获取验证码时发送
class protocol_verification
{
public:
	protocol_verification(one_message& my_one_message);
private:
	debug_and_log_lr* my_error;

};

//处理协议 OB00000110，客服端注册，发送验证码
class protocol_verification_check
{
public:
	protocol_verification_check(one_message& my_one_message);
private:
	debug_and_log_lr* my_error;
};


//处理协议 0B00000111，客服端关闭登陆页面时发送
class protocol_delete_rsa
{
public:
	protocol_delete_rsa(one_message& my_one_message);
private:
	debug_and_log_lr* my_error;

};

