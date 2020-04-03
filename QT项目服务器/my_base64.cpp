#include "my_base64.h"

using namespace std;

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

