#pragma once
#include "my_rsa.h"
#include<string>
#include<openssl/rsa.h>
#include<openssl/pem.h>
#include<iostream>
using namespace std;



my_rsa::my_rsa()
{
	this->new_rsa();
}

my_rsa::~my_rsa()
{
	RSA_free(this->pri_key);
	RSA_free(this->pub_key);
}

bool my_rsa::generate_key_pair(int bist)
{
	//传入秘钥长度有误
	if (bist < 1024)
		bist= 1024;
	
	//生成秘钥对存放的位置
	RSA* key_pair = RSA_new();
	if (key_pair == NULL)
		return false;

	//生成秘钥时需要使用的数据，需要使用奇数
	BIGNUM* e= BN_new();
	if (e == NULL)
	{
		RSA_free(key_pair);
		return false;
	}
	//需要使用奇数
	BN_set_word(e, 65537);

	//生成秘钥对
	if (RSA_generate_key_ex(key_pair, bist, e, NULL) == 0)
	{
		BN_free(e);
		RSA_free(key_pair);
		return false;
	}
		
	//得到私钥
	this->pri_key = RSAPrivateKey_dup(key_pair);
	if (this->pri_key == NULL)
	{
		BN_free(e);
		RSA_free(key_pair);
		return false;
	}
	//得到公钥
	this->pub_key = RSAPublicKey_dup(key_pair);
	if (this->pub_key == NULL)
	{
		BN_free(e);
		RSA_free(key_pair);
		return false;
	}
	
	//生成秘钥成功释放资源
	BN_free(e);
	RSA_free(key_pair);
	return true;
}

void  my_rsa::get_pub_key_str_base64(string &ret_pub_key,int& str_len )
{

	//公钥转化为字符流，p必需要使用，否则str会被更改，这里返回值需要比需要长度大，比如128字节的秘钥返回，140
	//len保存需要申请空间大小
	int len = RSA_size(this->pub_key);
	//公钥转换字符流
	unsigned char* str_pub_key =(unsigned char*) malloc(len * 0.4+len);
	unsigned char* p = str_pub_key;
	str_len = i2d_RSAPublicKey(this->pub_key, &p);

	
	//存储编码后的数据
	ret_pub_key = this->base.coding_base64(str_pub_key, len);


	//释放资源
	free(str_pub_key);

}

void my_rsa::set_pub_key(string pub_key_str,int len)
{
	//释放构造申请的空间
	RSA_free(this->pub_key);

	//base64解码
	string str =this->base.decoding_base64(pub_key_str.data(),pub_key_str.size());

	//字符流装换为RSA秘钥，这里pp也必须使用，len为i2d_RSAPublicKey的返回值
	unsigned char* pp = (unsigned char*)str.data();

	//设置当前公钥
	this->pub_key = d2i_RSAPublicKey(NULL, (const unsigned char**)& pp, len);

}

string my_rsa::rsa_pubkey_encrypt(string data)
{
	if (this->pub_key == NULL || data.size() <= 0)
		return	string();
	//计算公钥长度
	int key_len = RSA_size(this->pub_key);
	//加密后数据存储的位置
	unsigned char* encode = new unsigned char[key_len + 1];
	//加密
	int ret = RSA_public_encrypt(data.size(),
		(const unsigned char*)data.data(),
		(unsigned char*)encode, this->pub_key,
		RSA_PKCS1_PADDING);
	if (ret == -1)
	{
		delete[] encode;
		return string();
	}

	//编码后返回
	string ret_str =  this->base.coding_base64(encode,ret);
	delete[] encode;
	return ret_str;


}

string my_rsa::rsa_prikey_decrypt(string data)
{
	
	//base64解码
	string data_1 = this->base.decoding_base64(data.data(), KEY_LENGTH/8);

	//计算私钥长度
	int key_len = RSA_size(this->pri_key);
	//保存解密后数据的位置
	char* decode = new char[key_len + 1];
	//使用私钥解密
	int ret = RSA_private_decrypt(key_len,
		(const unsigned char*)data_1.data(),
		(unsigned char*)decode,
		this->pri_key,
		RSA_PKCS1_PADDING);
	string ret_str = string();
	if (ret == -1)
	{

		delete[] decode;
		return string();
	}

	ret_str = string(decode, ret);
	delete[] decode;
	return ret_str;
}






void my_rsa::new_rsa()
{
	this->pri_key = RSA_new();
	this->pub_key = RSA_new();
}

//std::string my_rsa::coding_base64(unsigned char* str_pub_key,int len)
//{
//	//使用base64编码
//// 准备一个 base64 BIO对象
//	BIO* b64 = BIO_new(BIO_f_base64());
//	// 准备一个 内存的  BIO对象
//	BIO* mem = BIO_new(BIO_s_mem());
//	// 将base64 和 内存 bio 对象 -> bio链
//	BIO_push(b64, mem);	// 逻辑关系: b64->mem
//	// 数据通过base64进行编码之后 -> 存储到内存
//	// 往bio链中写数据
//	// 调用bio_write, base64对象默认做编码操作,此处长度必须比i2d_RSAPublicKey返回的值大，大多少不知道，测试128字节的秘钥大10可以，同时长度不能不比buf大
//	BIO_write(b64, str_pub_key, len);//此行代码特别注意，一定要比需要编码的字符串长度要长
//	// 将编码数据从 bio内存对象中读出
//	BUF_MEM* ptrMem;
//	BIO_get_mem_ptr(b64, &ptrMem);
//	//存储编码后的数据
//	return std::string(ptrMem->data, ptrMem->length);
//
//
//	//释放资源
//	BIO_free_all(b64);
//}
//
//
//string my_rsa::decoding_base64(const char* data,int len)
//{
//	//使用base64解码
//// 准备一个 base64 BIO对象
//	BIO* b64 = BIO_new(BIO_f_base64());
//	// 准备一个 内存的  BIO对象
//#if 0//另外一种写法
//	BIO * mem = BIO_new(BIO_s_mem());
//	BIO_write(mem, p, strlen(p));
//#else
//	BIO* mem = BIO_new_mem_buf(data,strlen(data));
//#endif
//
//	// 组织bio链
//	BIO_push(b64, mem);	// 逻辑关系: b64->mem
//	// 解码
//	unsigned char* buf = (unsigned char*)malloc(len+1);
//	memset(buf,0,len+1);
//	BIO_read(b64, buf, len);
//	
//	string ret_str = string((const char*)buf, len);
//
//	//释放资源IO_free_all(b64);
//	free(buf);
//
//	return ret_str;
//}


