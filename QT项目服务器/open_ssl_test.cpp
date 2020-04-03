//#include"hash_lr.h"
//#include <openssl/rsa.h>
//#include <openssl/pem.h>
//#include"rsa_crypto.h"
//#include"aes_crypto.h"
//
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
//
//void test_hash()
//{
//	hash_lr hs;
//
//	hs.add_data("哈哈");
//
//	cout << hs.result() << endl;
//
//	hs.add_data("哈哈");
//
//	cout << hs.result() << endl;
//
//	hs.init();
//
//	hs.add_data("哈哈");
//	cout << hs.result() << endl;
//}
//
//void test_rsa()
//{
//	//
//	rsa_crypto rsa;
//	rsa.generate_key_pair(128*8);
//	
//	string miwen = rsa.rsa_pubkey_encrypt("呵呵");
//	string mingwen = rsa.rsa_prikey_decrypt(miwen);
//	
//	cout << mingwen << endl;
//
//	char buf[3096] = "早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，早晨阳光照耀，";
//
//	hash_lr hs;
//	hs.add_data(buf);
//	string hs_data = hs.result();
//
//	string hs_miwen = rsa.rsa_sign(hs_data);
//	string data = string(buf)
//		+ string("\0\0\0\0\0\0\0\0\0",10) 
//		+string(hs_miwen);
//
//
//
//	string c_data = data;
//	int map  = c_data.find(string("\0\0\0\0\0\0\0\0\0",10));
//	string c_data_1 = string(c_data.data(),map+1);
//	string c_data_2 = string(c_data.data()+map+10,
//		rsa.get_key_siz());
//
//	hash_lr c_hs;
//	c_hs.add_data((char*)c_data_1.data());
//	string c_hs_data = c_hs.result();
//	cout<<rsa.rsa_verify(c_hs_data,c_data_2)<<endl;
//
//}
//
//void test_aes()
//{
//
//	string key ("123987234asd\01svbzcvbxcvasdasdasdfasdada",32);
//	aes_crypto aes(key);
//	string jiami = aes.aes_cbc_encrypy(string("奥是少时诵诗书所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所少时诵诗书所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所少时诵诗书所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所1少时诵诗书所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所少时诵诗书所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所少时诵诗书所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所所1所所所所所2342所所所所所所所所所所所所所所所所所大法师阿斯顿发送到发送到发送到发打发水电费撒打算术大师大所大所的亲爱的趣味趣味趣味和法律框架爱喝酒卡萨丁宏即可撒地方科技拉萨的回复就卡死的回复枯竭阿士大夫四大航空股份将暗示的高科技ad黄瓜花功夫就暗示的话费卡接收到回复iahfjkwaehuwaehfjksdfhkjas"));
//	string jiemi = aes.aes_cbc_decrypy(jiami);
//	cout<< jiemi <<endl;
//}
//
//
//
//int main1()
//{
//	//test_rsa(); 
//	test_aes();
//	//show_bin((char*)NULL+10,10);
//
//	/*string s("as");
//	cout << s.size() << endl;
//*/
//	exit(0);
//}