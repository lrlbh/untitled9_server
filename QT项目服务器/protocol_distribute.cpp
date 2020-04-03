#include "protocol_distribute.h"
#include "protocol_zhoyi.h"
#include"protoccol_rubbish.h"
#include<string>
#include"server.h"
#include"protocol_laozi.h"
using namespace std;

//class protocol_zhouyi;

//void show_bin(const char* bin, int len)
//{
//	if (len == 0)
//		len = 100;
//	for (int i = 0; i < len; i++)
//	{
//		char* temp_bin = (char*)bin + i;
//		printf("%d：", i);
//		for (int j = 0; j < 8; j++)
//		{
//
//			unsigned char temp2 = *temp_bin << j;
//			if (temp2 >> 7 == 0)
//			{
//				printf("0");
//
//			}
//			else if (temp2 >> 7 == 1)
//			{
//				printf("1");
//			}
//			else
//			{
//				printf("位移发送错误！\n");
//				return;
//			}
//		}
//		printf("\n");
//	}
//	printf("\n");
//
//}

protocol_distribute::protocol_distribute()
{
}

void protocol_distribute::ststart(map<int, queue<string>>& data)
{
	
	for (map<int, queue<string>>::iterator it = data.begin(); it != data.end(); ++it)
	{

		queue<string>& temp = (*it).second;	
		

		while (temp.empty() == false)
		{

			map<int,string> continue_data;
			string& message=temp.front();

			if (message.size() >= 10000 || message.size() <= 0)
			{
				//continue_data[(*it).first] = message;
				//protoccol_rubbish p(continue_data);
			}
			else if (message[message.size()-1] == 0B00000001)
			{
				
				message[message.size()-1] = '\0';
				continue_data[(*it).first] = message;

				protocol_zhouyi pz(continue_data);
			}
			else if (message[message.size() - 1] == 0B00000011)
			{
				message[message.size() - 1] = '\0';
				continue_data[(*it).first] = message;
				protocol_laozi laozi(continue_data);

			}
			else if (message[message.size() - 1] == 0B00000100)
			{
				message[message.size() - 1] = '\0';
				continue_data[(*it).first] = message;
				protocol_laozi laozi(continue_data);
			}
			else
			{
				/*cout << "无效请求" << endl;*/
			}


			temp.pop();
		}
	}
	
}
