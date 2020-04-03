#pragma once
#include<iostream>
using namespace std;
#include <openssl/md5.h>
#include <openssl/sha.h>
#include<string.h>


//类的功能：
	//1、封装了哈希算法


enum hash_type
{
	type_md5,
	type_sha1,
	type_sha224,
	type_sha256,
	type_sha384,
	type_sha512
};

class hash_lr
{
public:
	//功能：构造创建一个进行哈希运算的对象
	//参数1：枚举，选择需要进行的哈希运算类型
	hash_lr (hash_type type = type_md5);

	~hash_lr();

	//功能：初始化哈希对象
	//参数：枚举，选择需要进行的哈希运算类型
	void init(hash_type type = type_md5);

	//功能：添加需要进行哈希运算的数据
	//参数：传入需要运算的数据
	void add_data(char* data);

	//功能：获取哈希运算结果
	//返回值：返回哈希运算结果
	string result();

private:
	string md5_result();
	string sh1_result();
	string sha224_result();
	string sha256_result();
	string sha384_result();
	string sha512_result();



private:
	hash_type type;
	MD5_CTX md5_ctx;
	SHA_CTX sha1_ctxl;
	SHA256_CTX sha224_ctx;
	SHA256_CTX sha256_ctx;
	SHA512_CTX sha384_ctx;
	SHA512_CTX sha512_ctx;
};

