#pragma once
#include<iostream>
#include <openssl/md5.h>
#include <openssl/sha.h>
#include<openssl/aes.h>
#include<openssl/rsa.h>

#define KEY_LEN_BYTE 24					//对称加密，秘钥大小，单位字节，一般使用16、24、32 
#define KEY_LENGTH  1024                //非对称加密，密钥长度,单位 位(bits)  
#define PUB_KEY_FILE "pubkey.pem"		//非对称加密，公钥文件放在那  
#define PRI_KEY_FILE "prikey.pem"		//非对称加密，私钥文件放在那 


//类的功能：自己写的base64编解码
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

//类的功能：封装了openssl的对称加密
class my_aes
{
public:


	//通过字符串设置key
	//参数1：保存有秘钥信息的字符串
	//参数2：需要生成的秘钥长度，单位字节
	//注意：如果传入秘钥字符串大于参数len，大于部分将被舍弃
	bool add_str_key(std::string key, int len = KEY_LEN_BYTE);

	//通过保存有秘钥信息的文件设置key
	//参数1：保存有秘钥信息的文件名
	//参数2：需要生成的秘钥长度，单位字节
	bool add_file_key(const char* key, int len = KEY_LEN_BYTE);

	//通过base64编码设置key
	//参数1：base64编码过的，保存秘钥信息的字符串
	//参数2：需要生成的秘钥长度，单位字节
	bool add_base64_key(std::string key, int len = KEY_LEN_BYTE);

	//保存秘钥到本地文件，如果存在老的秘钥则删除
	//参数1：保存秘钥到什么位置
	//参数2：秘钥的长度
	//注意：保存在文件中的秘钥都是base64编码过的
	bool key_write_file(const char* file_name, int len = KEY_LEN_BYTE);

	//功能：加密，返回加密后的字符串
	//参数1：传入需要加密的数据
	//注意：暂时不支持二级制数据的加解密，例如有 '\0'
	std::string encrypy_aes(std::string data);

	//功能：解密，返回解密后的字符串
	//参数1：传入需要解密的数据
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

//类的功能：封装了openssl/bio库的base64编解码，非对称加密传输秘钥时使用
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

//类的功能：封装了openssl的非对称加密
class my_rsa
{
public:
	//构造为pub_key和pri_key分配空间
	my_rsa();
	//析构释放pub_key和pri_key
	~my_rsa();

	//生成秘钥,秘钥长度建议长度建议这么写，1024（128字节），2048（256字节），4096（512字节）
	bool generate_key_pair(int bist = KEY_LENGTH);

	//获取base64编码后的字符流公钥,
	//参数1：传入参数，返回字符流转RSA时使用的长度
	//参数2：传入参数，返回base64编码公钥的字符流
	void  get_pub_key_str_base64(std::string& ret_pub_key, int& str_len);

	//传入base64编码的公钥字符流，设置成员属性公钥
	bool set_pub_key(std::string pub_key_str, int len);

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

//类的功能：封装了openssl里面的哈希算法
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
	hash_lr(hash_type type = type_md5);

	~hash_lr();

	//功能：初始化哈希对象
	//参数：枚举，选择需要进行的哈希运算类型
	void init(hash_type type = type_md5);

	//功能：添加需要进行哈希运算的数据
	//参数：传入需要运算的数据
	void add_data(char* data);

	//功能：获取哈希运算结果
	//返回值：返回哈希运算结果
	std::string result();

private:
	std::string md5_result();
	std::string sh1_result();
	std::string sha224_result();
	std::string sha256_result();
	std::string sha384_result();
	std::string sha512_result();



private:
	hash_type type;
	MD5_CTX md5_ctx;
	SHA_CTX sha1_ctxl;
	SHA256_CTX sha224_ctx;
	SHA256_CTX sha256_ctx;
	SHA512_CTX sha384_ctx;
	SHA512_CTX sha512_ctx;
};

