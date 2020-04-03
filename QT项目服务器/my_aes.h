#pragma once
#include<iostream>
#include<openssl/aes.h>
#include"my_base64.h"
#define KEY_LEN_BYTE 24 //秘钥大小，单位字节，一般使用16、24、32 

class my_aes
{
public:


	//通过字符串设置key
	//参数1：保存有秘钥信息的字符串
	//参数2：需要生成的秘钥长度，单位字节
	//注意：如果传入秘钥字符串大于参数len，大于部分将被舍弃
	bool add_str_key(std::string key, int len= KEY_LEN_BYTE);

	//通过保存有秘钥信息的文件设置key
	//参数1：保存有秘钥信息的文件名
	//参数2：需要生成的秘钥长度，单位字节
	bool add_file_key(const char * key, int len = KEY_LEN_BYTE);
	
	//通过base64编码设置key
	//参数1：base64编码过的，保存秘钥信息的字符串
	//参数2：需要生成的秘钥长度，单位字节
	bool add_base64_key(std::string key,int len = KEY_LEN_BYTE);

	//保存秘钥到本地文件，如果存在老的秘钥则删除
	//参数1：保存秘钥到什么位置
	//参数2：秘钥的长度
	//注意：保存在文件中的秘钥都是base64编码过的
	bool key_write_file(const char* file_name, int len = KEY_LEN_BYTE);

	//功能：加密，返回加密后的字符串
	//参数1：传入需要加密的数据
	//参数2：需要加密的数据长度，单位字节
	//注意：暂时不支持二级制数据的加解密，例如有 '\0'
	std::string encrypy_aes(std::string data);

	//功能：解密，返回解密后的字符串
	//参数1：传入需要解密的数据
	//参数2：需要解密的数据长度，单位字节，数据加密前的长度
	//注意：暂时不支持二级制数据的加解密，例如有 '\0'
	std::string decrypy_aes(std::string data);

	//随机生成一个秘钥字符串，编码过的
	static std::string rand_key(int len = KEY_LEN_BYTE);

private:
	//秘钥字符串，二进制数据
	std::string key;

	//加密秘钥
	AES_KEY enc_key;
	//解密秘钥
	AES_KEY dec_key;

	//编码对象
	my_base64 base64;

	//功能：生成参与加解密运算的字符串（向量）
	//参数1：传出参数，返回一个根据秘钥生成的固定字符串
	void generate_ivec(unsigned char* ivec);

	
	//功能：编解码函数
	//参数1：需要加密的字符串，
	//参数2：需要加解密的数据长度
	//参数3：传宏，判断需要加密还是解密
	//注意：废弃使用了
	std::string aes_crypto_cbc(std::string data, int len, int crypto);
};
