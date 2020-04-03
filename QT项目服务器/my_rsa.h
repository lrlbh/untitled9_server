#pragma once
#include<openssl/rsa.h>
#include<iostream>
#include"bio_base64.h"

#define KEY_LENGTH  1024               // 密钥长度,单位 位(bits)  
#define PUB_KEY_FILE "pubkey.pem"    // 公钥文件放在那  
#define PRI_KEY_FILE "prikey.pem"    // 私钥文件放在那 


class my_rsa
{
public:
	//构造为pub_key和pri_key分配空间
	my_rsa();
	//析构释放pub_key和pri_key
	~my_rsa();

	//生成秘钥,秘钥长度建议长度建议这么写，1024（128字节），2048（256字节），4096（512字节）
	bool generate_key_pair(int bist= KEY_LENGTH);

	//获取base64编码后的字符流公钥,
	//参数1：传入参数，返回字符流转RSA时使用的长度
	//参数2：传入参数，返回base64编码公钥的字符流
	void  get_pub_key_str_base64( std::string& ret_pub_key,int& str_len);

	//传入base64编码的公钥字符流，设置成员属性公钥
	void set_pub_key(std::string pub_key_str, int len);

	//使用公钥加密,返回base64编码后的加密数据
	//参数1：传入需要加密的数据
	//返回值：
		//成功返回：base64编码后的加密数据
		//失败返回：string()空构造
	std::string rsa_pubkey_encrypt(std::string data);

	//使用私钥解密，返回解密后的数据
	//参数1：传入需要解密的数据
	//返回值:
		//成功返回：解密后的数据
		//失败返回：string()空构造
	std::string rsa_prikey_decrypt(std::string data);

//私有成员
private:
	RSA* pub_key;
	RSA* pri_key;
	
	bio_base64 base;

	//私有方法
	//为pub_key和pri_key分配空间
	void new_rsa();


};