#include "protocol_pub_key.h"
#include"debug_and_log_lr.h"
#include"debug_and_log_lr.h"
#include"my_rsa.h"
#include"gua_info.pb.h"
using namespace std;

protocol_pub_key::protocol_pub_key(std::map<int, std::string>& data)
{
	//要发送的数据
	map<int, string> send_data;
	this->my_error = debug_and_log_lr::get_error_lr();

	//获取到客服端发送的公钥
	map<int, string>::iterator it = data.begin();
	verification vft; //protobuf里面定义的数据类型
	vft.ParseFromString((*it).second);	//反序列化
	
	//获取到公钥
	my_rsa rsa;
	rsa.set_pub_key(vft.pub_key,vft.pub_key_len);


}
