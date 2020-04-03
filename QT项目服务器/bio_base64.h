#pragma once
#include<iostream>

class bio_base64
{
public:
	bio_base64();
	~bio_base64();

	//对数据进行base64编码
	//参数1：需要编码数据的字符流
	//参数2：需要编码数据的长度,单位是字节
	//返回值：编码后的字符串
	std::string coding_base64(unsigned char* str_pub_key, int len);

	//对数据进行base64解码
	//参数1：需要解码的数据
	//参数2：需要解码的数据原始长度，编码前长度，单位字节
	//返回值：解码后的字符串
	std::string decoding_base64(const char* data, int len);

};