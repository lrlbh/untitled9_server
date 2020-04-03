#define _CRT_SECURE_NO_WARNINGS
#include "my_aes.h"
#include<stdio.h>
#include<string>
using namespace std;

void show_bin(const char* bin, int len)
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

bool my_aes::add_str_key(string key, int len)
{

	//传入正确长度的秘钥
	if (key.size() != 16 && key.size() != 24 && key.size() != 32)
	{
		return false;
	}

	//设置加密秘钥
	AES_set_encrypt_key(
		(const unsigned char*)key.data(),
		KEY_LEN_BYTE * 8,
		&this->enc_key);

	//设置解密秘钥
	AES_set_decrypt_key(
		(const unsigned char*)key.data(),
		KEY_LEN_BYTE * 8,
		&this->dec_key);

	//保存秘钥字符串
	this->key = key;

	return true;
}

bool my_aes::add_file_key(const char* key, int len)
{
	//暂时存储，读出来的数据
	string temp_str;

	//打开文件
	FILE* fp = fopen(key,"r");
	if (fp==NULL)
	{
		return false;
	}

	//每次一个字节方式读取文件
	//feof返回值：
		//非0值：已经到文件结尾
		//0：没有到文件结尾
	while (!feof(fp))
	{
		temp_str += fgetc(fp);
	}
	//读出来的数据，末尾有东西弹出去
	temp_str.pop_back();
	

	//需要解码数据的长度，明显不对，小于秘钥字符串长度
	if (temp_str.size() < KEY_LEN_BYTE)
	{
		return false;
	}

	//获取解码后的秘钥字符串
	this->key  = string(this->base64.base64_dec((const unsigned char*)temp_str.data()));


	//设置加密秘钥
	AES_set_encrypt_key(
		(const unsigned char*)this->key.data(),
		KEY_LEN_BYTE * 8,
		&this->enc_key);

	//设置解密秘钥
	AES_set_decrypt_key(
		(const unsigned char*)this->key.data(),
		KEY_LEN_BYTE * 8,
		&this->dec_key);

	return true;
}

bool my_aes::add_base64_key(std::string key, int len)
{
	////需要解码数据的长度，明显不对，小于秘钥字符串长度
	//if (key.size()< len)
	//{
	//	return false;
	//}

	//解码,保存秘钥字符串
	this->key =this->base64.base64_dec((const unsigned char*)key.data());
	//设置加密秘钥
	AES_set_encrypt_key(
		(const unsigned char*)this->key.data(),
		KEY_LEN_BYTE * 8,
		&this->enc_key);


	//设置解密秘钥
	AES_set_decrypt_key(
		(const unsigned char*)this->key.data(),
		KEY_LEN_BYTE * 8,
		&this->dec_key);



	return true;
}

bool my_aes::key_write_file(const char* file_name, int len)
{
	//创建文件保存秘钥
	FILE* fp = fopen(file_name,"w");
	if (fp==NULL)
	{
		return false;
	}
	
	//对原始秘钥字符串进行编码
	string temp_str = this->base64.base64_enc((const unsigned char*)this->key.data(),len);

	//编码后的秘钥字符串，写入文件中
	if (fputs(temp_str.data(), fp)!= 0)
	{
		return false;
	}
	
	return true;
}

string my_aes::encrypy_aes(string data)
{
		

	//加解密时传入参数lenth，
	//需要是16的整数倍，不足补齐，
	//如果恰好是整数倍，也需要扩充一个秘钥的长度(字节)
	int len = data.size();
	len = (len / 16 +1) * 16;


	//加解密时参与运算的向量
	unsigned char ivec[AES_BLOCK_SIZE];
	memset(ivec, 0, AES_BLOCK_SIZE);
	//generate_ivec(ivec);

	//接收，传出加解密数据
	//这里malloc的数据一释放就出错
	unsigned char* buf = (unsigned char*)malloc(len);
	memset(buf, 0, len);


	//加解密
	AES_cbc_encrypt(
		(const unsigned char*)data.data(), //需要加解密的数据
		buf,								//传出加解密后的数据
		len,								//传入参数lenth，需要是秘钥长度的整数倍，不足补齐
		&this->enc_key,							//对应加解密秘钥
		ivec,								//向量，加解密时需要一样
		AES_ENCRYPT);							//表示本次函数调用时加密还是解密


		//如果是加密,需要编码后返回
		string ret_str(this->base64.base64_enc(buf, len));
		free(buf); 
		return ret_str;

}

string my_aes::decrypy_aes(string data)
{


	//解密先解码
	string temp_data(this->base64.base64_dec((const unsigned char*)data.data()));

	//加解密时传入参数lenth，
	//需要是16的整数倍，不足补齐，
	//如果恰好是整数倍，也需要扩充一个秘钥的长度(字节)
	int len = temp_data.size();
	len = (len / 16 + 1) * 16;

	cout << len << endl;

	//加解密时参与运算的向量
	unsigned char ivec[AES_BLOCK_SIZE];
	memset(ivec, 0, AES_BLOCK_SIZE);
	//generate_ivec(ivec);

	//接收，传出加解密数据
	//这里malloc的数据一释放就出错
	unsigned char* buf = (unsigned char*)malloc(len);
	memset(buf, 0, len);



	//加解密
	AES_cbc_encrypt(
		(const unsigned char*)temp_data.data(), //需要加解密的数据
		buf,								//传出加解密后的数据
		len,								//传入参数lenth，需要是秘钥长度的整数倍，不足补齐
		&this->dec_key,						//对应加解密秘钥
		ivec,								//向量，加解密时需要一样
		AES_DECRYPT);							//表示本次函数调用时加密还是解密


		//解密直接返回
		string str_ret((char*)buf);
		free(buf);
		return str_ret;

}


std::string my_aes::rand_key(int len)
{

	//保存没有base64编码的秘钥字符串
	string temp_str;
	//保存base64编码后的秘钥字符串
	string ret_str;

	//初始化返回字符串
	for (int i = 0; i < KEY_LEN_BYTE; i++)
	{
		temp_str.append("1");
	}

	//随机生成一个秘钥字符串
	for (int i = 0; i < KEY_LEN_BYTE; i++)
	{
		temp_str[i] = rand() % 256;
	}
	//show_bin(temp_str.data(),10);
	
	//对秘钥字符串进行编码
	my_base64 temp_base64;
	ret_str = temp_base64.base64_enc((const unsigned char *)temp_str.data(), KEY_LEN_BYTE);
	//show_bin(ret_str.data(), 10);
	return ret_str;
}

void my_aes::generate_ivec(unsigned char* ivec)
{
	//简单的把秘钥逆序一下
	for (int i = 0; i < AES_BLOCK_SIZE; ++i)
		ivec[i] = this->key[AES_BLOCK_SIZE - 1 - i];
		
}

//废弃使用
std::string my_aes::aes_crypto_cbc(string data, int len ,int crypto)
{
	//判断当前需要加密还是解密，保存对应的秘钥
	AES_KEY* temp_key = crypto == AES_ENCRYPT ?
		&this->enc_key : &this->dec_key;

	//如果是解密需要先解码
	string temp_data;
	if (crypto == AES_ENCRYPT)
	{
		temp_data=data;
	}
	else
	{
		//base64解码
		temp_data=this->base64.base64_dec((const unsigned char*)data.data());
	}

	

	//加解密时传入参数lenth，需要是秘钥长度的整数倍，不足补齐
	int len_1 = temp_data.size();
	if (len_1 / this->key.size() != 0)
	{
		len_1 = (len_1 / this->key.size() + 1)* this->key.size();
	}

	//加解密时参与运算的向量
	unsigned char ivec[AES_BLOCK_SIZE];
	memset(ivec, 0, AES_BLOCK_SIZE);
	generate_ivec(ivec);

	//接收，传出加解密数据
	//这里malloc的数据一释放就出错
	unsigned char* buf = (unsigned char*)malloc(len_1+1);
	memset(buf, 0, len_1+1);

	//if (crypto != AES_ENCRYPT)
	//{
	//	len_1 = temp_data.size();
	//}

	//加解密
	 AES_cbc_encrypt(
		(const unsigned char*)temp_data.data(), //需要加解密的数据
		buf,								//传出加解密后的数据
		len_1,								//传入参数lenth，需要是秘钥长度的整数倍，不足补齐
		temp_key,							//对应加解密秘钥
		ivec,								//向量，加解密时需要一样
		crypto);							//表示本次函数调用时加密还是解密

	if (crypto == AES_ENCRYPT)
	{
		//如果是加密,需要编码后返回
		string ret_str(this->base64.base64_enc(buf, len_1)) ;
		return ret_str;
		
	}
	else
	{
		//解密直接返回
		string str_ret((char*)buf);
		return str_ret;
		
	}

}
