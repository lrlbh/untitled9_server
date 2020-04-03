#pragma once
#include"enc_and_dec.h"
#include  <openssl/pem.h>
#include<string.h>
using namespace std;


bio_base64::bio_base64()
{
}

bio_base64::~bio_base64()
{
}

string bio_base64::coding_base64(unsigned char* data, int len)
{

	//编码的数据必须长一点
	if (len > 128)
	{
		len += len * 0.2;
	}
	else
	{
		len += 20;
	}
	//使用base64编码
	//准备一个 base64 BIO对象
	BIO* b64 = BIO_new(BIO_f_base64());
	// 准备一个 内存的  BIO对象
	BIO* mem = BIO_new(BIO_s_mem());
	// 将base64 和 内存 bio 对象 -> bio链
	BIO_push(b64, mem);	// 逻辑关系: b64->mem
	// 数据通过base64进行编码之后 -> 存储到内存
	// 往bio链中写数据
	// 调用bio_write, base64对象默认做编码操作,此处长度必须比i2d_RSAPublicKey返回的值大，大多少不知道，测试128字节的秘钥大20可以，同时长度不能不比buf大
	BIO_write(b64, data, len);//此行代码特别注意，一定要比需要编码的字符串长度要长
	// 将编码数据从 bio内存对象中读出
	BUF_MEM* ptrMem;
	BIO_get_mem_ptr(b64, &ptrMem);
	//存储编码后的数据
	return std::string(ptrMem->data, ptrMem->length);


	//释放资源
	BIO_free_all(b64);
}

string bio_base64::decoding_base64(const char* data, int len)
{
	//使用base64解码
// 准备一个 base64 BIO对象
	BIO* b64 = BIO_new(BIO_f_base64());
	// 准备一个 内存的  BIO对象
#if 0//另外一种写法
	BIO * mem = BIO_new(BIO_s_mem());
	BIO_write(mem, p, strlen(p));
#else
	BIO* mem = BIO_new_mem_buf(data, strlen(data));
#endif

	// 组织bio链
	BIO_push(b64, mem);	// 逻辑关系: b64->mem
	// 解码
	unsigned char* buf = (unsigned char*)malloc(len + 1);
	memset(buf, 0, len + 1);
	BIO_read(b64, buf, len);

	string ret_str = string((const char*)buf, len);

	//释放资源IO_free_all(b64);
	free(buf);

	return ret_str;


}



hash_lr::hash_lr(hash_type type)
{
	this->init(type);

}

hash_lr::~hash_lr()
{

}

void hash_lr::init(hash_type type)
{
	this->type = type;
	switch (type)
	{
	case type_md5:
		MD5_Init(&this->md5_ctx);
		break;

	case type_sha1:
		SHA1_Init(&this->sha1_ctxl);
		break;

	case type_sha224:
		SHA224_Init(&this->sha224_ctx);
		break;

	case type_sha256:
		SHA256_Init(&this->sha256_ctx);
		break;

	case type_sha384:
		SHA384_Init(&this->sha384_ctx);
		break;

	case type_sha512:
		SHA512_Init(&this->sha512_ctx);
		break;

	default:
		MD5_Init(&this->md5_ctx);
		break;
	}
}

void hash_lr::add_data(char* data)
{
	switch (this->type)
	{
	case type_md5:
		MD5_Update(&this->md5_ctx, data, strlen(data));
		break;

	case type_sha1:
		SHA1_Update(&this->sha1_ctxl, data, strlen(data));
		break;

	case type_sha224:
		SHA224_Update(&this->sha224_ctx, data, strlen(data));
		break;

	case type_sha256:
		SHA256_Update(&this->sha256_ctx, data, strlen(data));
		break;

	case type_sha384:
		SHA384_Update(&this->sha384_ctx, data, strlen(data));
		break;

	case type_sha512:
		SHA512_Update(&this->sha512_ctx, data, strlen(data));
		break;

	default:
		MD5_Update(&this->md5_ctx, data, strlen(data));
		break;
	}
}

string hash_lr::result()
{

	switch (this->type)
	{
	case type_md5:
		return this->md5_result();
		break;

	case type_sha1:
		return this->sh1_result();
		break;

	case type_sha224:
		return this->sha224_result();
		break;

	case type_sha256:
		return this->sha256_result();
		break;

	case type_sha384:
		return this->sha384_result();
		break;

	case type_sha512:
		return this->sha512_result();
		break;

	default:
		return this->md5_result();
		break;
	}



}

string hash_lr::md5_result()
{
	unsigned char md[MD5_DIGEST_LENGTH];
	char res[MD5_DIGEST_LENGTH * 2 + 1];
	MD5_Final(md, &this->md5_ctx);
	for (int i = 0; i < MD5_DIGEST_LENGTH; i++)
		sprintf(&res[i * 2], "%02x", md[i]);
	return string(res, MD5_DIGEST_LENGTH * 2 + 1);
}

string hash_lr::sh1_result()
{
	unsigned char md[SHA_DIGEST_LENGTH];
	char res[SHA_DIGEST_LENGTH * 2 + 1];
	SHA1_Final(md, &this->sha1_ctxl);
	for (int i = 0; i < SHA_DIGEST_LENGTH; i++)
		sprintf(&res[i * 2], "%02x", md[i]);
	return string(res, SHA_DIGEST_LENGTH * 2 + 1);
}

string hash_lr::sha224_result()
{

	unsigned char md[SHA224_DIGEST_LENGTH];
	char res[SHA224_DIGEST_LENGTH * 2 + 1];
	SHA224_Final(md, &this->sha224_ctx);
	for (int i = 0; i < SHA224_DIGEST_LENGTH; i++)
		sprintf(&res[i * 2], "%02x", md[i]);
	return string(res, SHA224_DIGEST_LENGTH * 2 + 1);
}

string hash_lr::sha256_result()
{
	unsigned char md[SHA256_DIGEST_LENGTH];
	char res[SHA256_DIGEST_LENGTH * 2 + 1];
	SHA256_Final(md, &this->sha256_ctx);
	for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
		sprintf(&res[i * 2], "%02x", md[i]);
	return string(res, SHA256_DIGEST_LENGTH * 2 + 1);
}

string hash_lr::sha384_result()
{
	unsigned char md[SHA384_DIGEST_LENGTH];
	char res[SHA384_DIGEST_LENGTH * 2 + 1];
	SHA384_Final(md, &this->sha384_ctx);
	for (int i = 0; i < SHA384_DIGEST_LENGTH; i++)
		sprintf(&res[i * 2], "%02x", md[i]);
	return string(res, SHA384_DIGEST_LENGTH * 2 + 1);
}

string hash_lr::sha512_result()
{
	unsigned char md[SHA512_DIGEST_LENGTH];
	char res[SHA512_DIGEST_LENGTH * 2 + 1];
	SHA512_Final(md, &this->sha512_ctx);
	for (int i = 0; i < SHA512_DIGEST_LENGTH; i++)
		sprintf(&res[i * 2], "%02x", md[i]);
	return string(res, SHA512_DIGEST_LENGTH * 2 + 1);
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
	FILE* fp = fopen(key, "r");
	if (fp == NULL)
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
	this->key = string(this->base64.base64_dec((const unsigned char*)temp_str.data()));


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
	this->key = this->base64.base64_dec((const unsigned char*)key.data());
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
	FILE* fp = fopen(file_name, "w");
	if (fp == NULL)
	{
		return false;
	}

	//对原始秘钥字符串进行编码
	string temp_str = this->base64.base64_enc((const unsigned char*)this->key.data(), len);

	//编码后的秘钥字符串，写入文件中
	if (fputs(temp_str.data(), fp) != 0)
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
	len = (len / 16 + 1) * 16;


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
	ret_str = temp_base64.base64_enc((const unsigned char*)temp_str.data(), KEY_LEN_BYTE);
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
std::string my_aes::aes_crypto_cbc(string data, int len, int crypto)
{
	//判断当前需要加密还是解密，保存对应的秘钥
	AES_KEY* temp_key = crypto == AES_ENCRYPT ?
		&this->enc_key : &this->dec_key;

	//如果是解密需要先解码
	string temp_data;
	if (crypto == AES_ENCRYPT)
	{
		temp_data = data;
	}
	else
	{
		//base64解码
		temp_data = this->base64.base64_dec((const unsigned char*)data.data());
	}



	//加解密时传入参数lenth，需要是秘钥长度的整数倍，不足补齐
	int len_1 = temp_data.size();
	if (len_1 / this->key.size() != 0)
	{
		len_1 = (len_1 / this->key.size() + 1) * this->key.size();
	}

	//加解密时参与运算的向量
	unsigned char ivec[AES_BLOCK_SIZE];
	memset(ivec, 0, AES_BLOCK_SIZE);
	generate_ivec(ivec);

	//接收，传出加解密数据
	//这里malloc的数据一释放就出错
	unsigned char* buf = (unsigned char*)malloc(len_1 + 1);
	memset(buf, 0, len_1 + 1);

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
		string ret_str(this->base64.base64_enc(buf, len_1));
		return ret_str;

	}
	else
	{
		//解密直接返回
		string str_ret((char*)buf);
		return str_ret;

	}

}




std::string my_base64::base64_enc(const unsigned char* bin, int len)
{
	//返回base64编码后的字符串
	string ret_str;

	//计算有数据有多少位（bits）
	len = len * 8;

	// i 用于控制传入数据的每一个字节
	// x 用于控制编码后数据的每一个字节
	// y 用于控制编码后数据的值
	// z 用于控制编码了多少位，编码完成后退出循环
	for (int i = 0, x = 0, y = 0, z = 0; i < len; i++)
	{
		//获取传入数据的 一个字节数据
		char* temp_bin = (char*)bin + i;
		for (int j = 0; j < 8; j++)
		{
			//左移顶出不需要的位（bits）
			unsigned char temp2 = *temp_bin << j;
			//判断当前位（是否为0）
			if (temp2 >> 7 == 0)
			{
				//已经操作了一位，++
				z++;
				//已经操作了一位，++
				x++;
				//x等于6，已经获取到一个字节的编码后数据
				if (x == 6)
				{
					//y是传入数据的值
					if (y <= 25)
					{
						//这里是base64后的值
						ret_str += y + 65;
					}
					else if (y <= 51)
					{
						ret_str += 71 + y;
					}
					else if (y <= 61)
					{
						ret_str += y - 4;
					}
					else if (y == 62)
					{
						ret_str += 43;
					}
					else if (y == 63)
					{
						ret_str += 47;
					}
					y = 0;
					x = 0;
					if (z >= len)
					{
						ret_str += "\0";
						return ret_str;
					}
				}

			}
			else if (temp2 >> 7 == 1)
			{
				z++;
				x++;
				if (x == 1)
				{
					y += 32;
				}
				else if (x == 2)
				{
					y += 16;
				}
				else if (x == 3)
				{
					y += 8;
				}
				else if (x == 4)
				{
					y += 4;
				}
				else if (x == 5)
				{
					y += 2;
				}
				else if (x == 6)
				{
					y += 1;
				}
				if (x == 6)
				{

					if (y <= 25)
					{
						ret_str += y + 65;
					}
					else if (y <= 51)
					{
						ret_str += 71 + y;
					}
					else if (y <= 61)
					{
						ret_str += y - 4;
					}
					else if (y == 62)
					{
						ret_str += 43;
					}
					else if (y == 63)
					{
						ret_str += 47;
					}
					y = 0;
					x = 0;
					if (z >= len)
					{
						ret_str += "\0";
						return ret_str;
					}

				}
			}
			else
			{
				return string();
			}
		}
	}
	return string();
}

std::string my_base64::base64_dec(const unsigned char* data)
{
	//返回解码后的数据
	string ret_str;
	//获取当前编码后的数据有多少个字节
	int len = strlen((const char*)data);
	//存储中间数据
	string buf;



	//从base64转回原始数据，但是这里的数据有每个字节，前面两个位，是无用的，需要进行处理
	for (int i = 0; i < len; i++)
	{
		if (data[i] >= 48 && data[i] <= 57)
		{
			for (int j = 0; j < 10; j++)
			{
				if (data[i] == j + 48)
				{
					buf += (52 + j);
					break;
				}
			}
		}
		else if (data[i] >= 65 && data[i] <= 90)
		{
			for (int j = 0; j < 26; j++)
			{
				if (data[i] == j + 65)
				{
					buf += (0 + j);
					break;
				}
			}

		}
		else if (data[i] >= 97 && data[i] <= 122)
		{
			for (int j = 0; j < 26; j++)
			{
				if (data[i] == j + 97)
				{
					buf += (26 + j);
					break;
				}
			}
		}
		else if (data[i] == 47)
		{
			buf += 63;
		}
		else if (data[i] == 43)
		{
			buf += 62;
		}
		else
		{
			return string();
		}
	}


	//把每个字节，前面两个位的0去掉，然后拼接
	//j 控制原始数据的位
	//x 控制解码后数据的位
	//y 保存解码后数据的值
	for (int i = 0, x = 0, y = 0; i < len; i++)
	{
		unsigned char* temp_bin = (unsigned char*)buf.data() + i;
		for (int j = 0; j < 8; j++)
		{
			if (j < 2)
			{
				continue;
			}
			unsigned char temp2 = *temp_bin << j;
			if (temp2 >> 7 == 1)
			{
				x++;
				if (x == 1)
				{
					y += 128;
				}
				else if (x == 2)
				{
					y += 64;
				}
				else if (x == 3)
				{
					y += 32;
				}
				else if (x == 4)
				{
					y += 16;
				}
				else if (x == 5)
				{
					y += 8;

				}
				else if (x == 6)
				{
					y += 4;
				}
				else if (x == 7)
				{
					y += 2;
				}
				else if (x == 8)
				{
					y += 1;

					ret_str += y;

					y = 0;
					x = 0;
				}

			}
			else if (temp2 >> 7 == 0)
			{
				x++;
				if (x == 8)
				{

					ret_str += y;

					y = 0;
					x = 0;
				}

			}
			else
			{
				return string();
			}
		}
	}


	return ret_str;
}




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
		bist = 1024;

	//生成秘钥对存放的位置
	RSA* key_pair = RSA_new();
	if (key_pair == NULL)
		return false;

	//生成秘钥时需要使用的数据，需要使用奇数
	BIGNUM* e = BN_new();
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

void  my_rsa::get_pub_key_str_base64(string& ret_pub_key, int& str_len)
{

	//公钥转化为字符流，p必需要使用，否则str会被更改，这里返回值需要比需要长度大，比如128字节的秘钥返回，140
	//len保存需要申请空间大小
	int len = RSA_size(this->pub_key);
	//公钥转换字符流
	unsigned char* str_pub_key = (unsigned char*)malloc(len * 0.4 + len);
	unsigned char* p = str_pub_key;
	str_len = i2d_RSAPublicKey(this->pub_key, &p);


	//存储编码后的数据
	ret_pub_key = this->base.coding_base64(str_pub_key, len);


	//释放资源
	free(str_pub_key);

}

bool my_rsa::set_pub_key(string pub_key_str, int len)
{
	//释放构造申请的空间
	RSA_free(this->pub_key);

	//base64解码
	string str = this->base.decoding_base64(pub_key_str.data(), pub_key_str.size());

	//字符流装换为RSA秘钥，这里pp也必须使用，len为i2d_RSAPublicKey的返回值
	unsigned char* pp = (unsigned char*)str.data();

	//设置当前公钥
	this->pub_key = d2i_RSAPublicKey(NULL, (const unsigned char**)& pp, len);
	if (this->pub_key == NULL)
	{
		return false;
	}

	return true;

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
	string ret_str = this->base.coding_base64(encode, ret);
	delete[] encode;
	return ret_str;


}

string my_rsa::rsa_prikey_decrypt(string data)
{

	//base64解码
	string data_1 = this->base.decoding_base64(data.data(), KEY_LENGTH / 8);

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