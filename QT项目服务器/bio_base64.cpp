#include "bio_base64.h"
#include  <openssl/pem.h>
#include<string.h>
using namespace std;


void show_bin1(const char* bin, int len)
{
	if (len == 0)
		len = 100;
	for (int i = 0; i < len; i++)
	{
		char* temp_bin = (char*)bin + i;
		printf("%d:", i);
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
bio_base64::bio_base64()
{
}

bio_base64::~bio_base64()
{
}

string bio_base64::coding_base64(unsigned char* data, int len)
{

	//编码的数据必须长一点
	if (len>128)
	{
		len += len * 0.2;
	}
	else
	{
		len +=  20;
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

	cout << "编码后的数据" << endl;
	show_bin1(ptrMem->data, 10);

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
