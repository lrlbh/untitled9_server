#include<iostream>



class my_base64
{
public:
	//对数据进行base64编码
	//参数1：传入数据
	//参数2：需要编码多少个字节
	std::string base64_enc(const unsigned char* bin, int len);

	//对数据进行base64解码
	//参数1：传入需要解码的数据
	std::string base64_dec(const unsigned char* data);
	

};