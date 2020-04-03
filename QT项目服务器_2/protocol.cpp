#include "protocol.h"
#pragma once
#include<map>
#include<string>
#include"debug_and_log_lr.h"
#include"gua_info.pb.h"
#include"tool.h"
#include"send_message.h"
#include"enc_and_dec.h"

using namespace std;

one_message::one_message(std::string& data, ptr* ptr):m_data(data),m_ptr(ptr)
{
}

protocol::protocol(message& cfd_message)
{
	//queue<string>& temp = cfd_mmessage.data;
	while (cfd_message.data.empty() == false)
	{
		string& message = cfd_message.data.front();

		if (message.size() >= 10000 || message.size() <= 0)
		{
			//continue_data[(*it).first] = message;
			//protoccol_rubbish p(continue_data);
		}
		else if (message[message.size() - 1] == 0B00000001)  //周易
		{
			//把协议部分舍弃,下一个对象可以直接使用protobuf反序列化，不需要在处理
			message.erase(message.size() - 1, 1);
			//创建需要传输给下一个对象的数据
			one_message my_message(message, cfd_message.message_ptr);
			//把数据传输给下一个对象处理
			protocol_zhouyi pz(my_message);
		}
		else if (message[message.size() - 1] == 0B00000011)  //老子
		{
			//把协议部分舍弃,下一个对象可以直接使用protobuf反序列化，不需要在处理
			message.erase(message.size() - 1, 1);
			//创建需要传输给下一个对象的数据
			one_message my_message(message, cfd_message.message_ptr);
			//把数据传输给下一个对象处理
			protocol_laozi laozi(my_message);

		}
		else if (message[message.size() - 1] == 0B00000100)	//客服端打开登陆页面时发送公钥过来
		{
			//把协议部分舍弃,下一个对象可以直接使用protobuf反序列化，不需要在处理
			message.erase(message.size() - 1, 1);
			//创建需要传输给下一个对象的数据
			one_message my_message(message, cfd_message.message_ptr);

			//把数据传输给下一个对象处理
			prorocol_log_in_ui prv(my_message);
		}
		else if (message[message.size() - 1] == 0B00000101)
		{
			//把协议部分舍弃,下一个对象可以直接使用protobuf反序列化，不需要在处理
			message.erase(message.size() - 1, 1);
			//创建需要传输给下一个对象的数据
			one_message my_message(message, cfd_message.message_ptr);

			//把数据传输给下一个对象处理
			protocol_verification pv(my_message);
			
		}
		else if (message[message.size() - 1] == 0B00000110)
		{
			//把协议部分舍弃,下一个对象可以直接使用protobuf反序列化，不需要在处理
			message.erase(message.size() - 1, 1);
			//组织需要传输给下一个对象的数据
			one_message my_message(message, cfd_message.message_ptr);

			//把数据传输给下一个对象处理
			protocol_verification_check pvc(my_message);
		}
		else
		{
			/*cout << "无效请求" << endl;*/
		}

		//处理一条数据后，移除队头元素
		cfd_message.data.pop();
	}


}


protocol_laozi::protocol_laozi(one_message& my_one_message)
{
	this->my_error = debug_and_log_lr::get_error_lr();

	//序列化
	dao_de_jing laozi;

	my_sql mysql;
	string sql = string("select ddj_message from ddj where ddj_id= ").append(to_string(rand() % 80 + 1));
	MYSQL_RES* laozi_res = mysql.sql_exec(sql);

	//通过结果集获取数据
	if (laozi_res != NULL)
	{
		//获取当前结果集的字段数(列)
		//unsigned int num_fields = mysql_num_fields(res);
		//保存结果集的每一行，其实就是个char二级指针
		MYSQL_ROW row = NULL;
		//获取结果集合的每一行
		while (row = mysql_fetch_row(laozi_res))
		{
			laozi.set_message((const char*)row[0]);
		}
		////获取结果集的表头
		//MYSQL_FIELD* fields = mysql_fetch_fields(res);
		//for (i = 0; i < num_fields; i++)
		//	printf("%s\t", fields[i].name);
		//printf("\n--------------------\
 		//			--------------------------------\n");
	}
	else
	{
		this->my_error->log_error_lr("laozi_res == NULL", sql.data());
	}
	mysql_free_result(laozi_res);


	string buf;
	laozi.SerializeToString(&buf);
	//填充两个字节的协议,确保发送时size属性的正确
	buf.append("aa");
	buf[buf.size() - 2] = 0B00000010;
	buf[buf.size() - 1] = '\0';
	
	//发送数据
	send_data sd(buf,my_one_message.m_ptr);
	send_message s(sd);

}


protocol_zhouyi::protocol_zhouyi(one_message& my_one_message)
{
	//要发送的数据
	this->my_error = debug_and_log_lr::get_error_lr();

	//序列化
	gua my_gua;


	//获取一挂
	char yao_yy[12];
	vector<int> gua_info = this->get_one_gua(yao_yy);

	//得到卦的变爻数量
	my_gua.set_bian_yao_count(gua_info[2]);


	//连接mysql数据库
	my_sql mysql;
	//执行sql语言
	string sql;
	sql = "select gua_name,gua_message from gua_64 where gua_id=";

	//得到本卦卦名和信息
	sql.append(to_string(gua_info[0]));
	MYSQL_RES* res = mysql.sql_exec(sql);
	//通过结果集获取数据
	if (res != NULL)
	{
		//获取当前结果集的字段数(列)
		//unsigned int num_fields = mysql_num_fields(res);
		//保存结果集的每一行，其实就是个char二级指针
		MYSQL_ROW row = NULL;
		//获取结果集合的每一行
		while (row = mysql_fetch_row(res))
		{
			my_gua.set_ben_gua_name((const char*)row[0]);
			my_gua.set_ben_gua_info((const char*)row[1]);
		}
		////获取结果集的表头
		//MYSQL_FIELD* fields = mysql_fetch_fields(res);
		//for (i = 0; i < num_fields; i++)
		//	printf("%s\t", fields[i].name);
		//printf("\n--------------------\
		//			--------------------------------\n");
	}
	else
	{
		this->my_error->log_error_lr("res == NULL", sql.data());
	}
	mysql_free_result(res);

	//查询变卦卦名和信息
	sql = "select gua_name,gua_message from gua_64 where gua_id=";
	sql.append(to_string(gua_info[1]).data());
	res = mysql.sql_exec(sql);
	if (res != NULL)
	{
		//保存结果集的每一行，其实就是个char二级指针
		MYSQL_ROW row = NULL;
		//获取结果集合的每一行
		while (row = mysql_fetch_row(res))
		{
			my_gua.set_bian_gua_name((const char*)row[0]);
			my_gua.set_bian_gua_info((const char*)row[1]);
		}
	}
	else
	{
		this->my_error->log_error_lr("res == NULL", sql.data());
	}
	mysql_free_result(res);


	//查询本卦的爻信息
	sql = "select yao_id,yao_message from yao_386 where gua_id = " + string(to_string(gua_info[0]).data()).append(" order by yao_id asc");
	res = mysql.sql_exec(sql);
	if (res != NULL)
	{
		int i = 0;
		//获取当前结果集的列
		unsigned int num_fields = mysql_num_fields(res);
		//保存结果集的每一行，其实就是个char二级指针
		MYSQL_ROW row = NULL;
		//获取结果集合的每一行
		while (row = mysql_fetch_row(res))
		{
			//protobuf对象数组，先挂上树，后赋值
			yao* temp_yao = my_gua.add_ben_gua_yao_info();
			if (yao_yy[i] == '1')
			{
				temp_yao->set_is_yang(true);
			}
			else
			{
				temp_yao->set_is_yang(false);
			}
			temp_yao->set_info(row[1]);
			i++;
		}
	}
	else
	{
		this->my_error->log_error_lr("res == NULL", sql.data());
	}
	mysql_free_result(res);

	//查询变卦的爻信息
	sql = "select yao_id,yao_message from yao_386 where gua_id = " + string(to_string(gua_info[1]).data()).append(" order by yao_id asc");
	res = mysql.sql_exec(sql);
	if (res != NULL)
	{
		int i = 0;
		//获取当前结果集的列
		unsigned int num_fields = mysql_num_fields(res);
		//保存结果集的每一行，其实就是个char二级指针
		MYSQL_ROW row = NULL;
		//获取结果集合的每一行
		while (row = mysql_fetch_row(res))
		{
			yao* temp_yao = my_gua.add_bian_gua_yao_info();
			temp_yao->set_info(row[1]);
			if (yao_yy[i + 6] == '1')
			{
				temp_yao->set_is_yang(true);
			}
			else
			{
				temp_yao->set_is_yang(false);
			}

			i++;
		}
	}
	else
	{
		this->my_error->log_error_lr("res == NULL", sql.data());
	}
	mysql_free_result(res);



	string buf;
	my_gua.SerializeToString(&buf);
	//填充两个字节的协议，确保发送时size属性的正确
	buf.append("aa");
	buf[buf.size() - 2] = 0B00000001;
	buf[buf.size() - 1] = '\0';

	//发送数据
	send_data sd(buf, my_one_message.m_ptr);
	send_message s(sd);

}

std::vector<int> protocol_zhouyi::get_one_gua(char* yao_yy)
{
	//按顺序存储384爻减少查询数据量、查询次数
	char buf11[64][6] = { {'1','1','1','1','1','1'},
	{'0','0','0','0','0','0'},{'1','0','0','0','1','0'},
	{'0','1','0','0','0','1'},{'1','1','1','0','1','0'},
	{'0','1','0','1','1','1'},{'0','1','0','0','0','0'},
	{'0','0','0','0','1','0'},{'1','1','1','0','1','1'},
	{'1','1','0','1','1','1'},{'1','1','1','0','0','0'},
	{'0','0','0','1','1','1'},{'1','0','1','1','1','1'},
	{'1','1','1','1','0','1'},{'0','0','1','0','0','0'},
	{'0','0','0','1','0','0'},{'1','0','0','1','1','0'},
	{'0','1','1','0','0','1'},{'1','1','0','0','0','0'},
	{'0','0','0','0','1','1'},{'1','0','0','1','0','1'},
	{'1','0','1','0','0','1'},{'0','0','0','0','0','1'},
	{'1','0','0','0','0','0'},{'1','0','0','1','1','1'},
	{'1','1','1','0','0','1'},{'1','0','0','0','0','1'},
	{'0','1','1','1','1','0'},{'0','1','0','0','1','0'},
	{'1','0','1','1','0','1'},{'0','0','1','1','1','0'},
	{'0','1','1','1','0','0'},{'0','0','1','1','1','1'},
	{'1','1','1','1','0','0'},{'0','0','0','1','0','1'},
	{'1','0','1','0','0','0'},{'1','0','1','0','1','1'},
	{'1','1','0','1','0','1'},{'0','0','1','0','1','0'},
	{'0','1','0','1','0','0'},{'1','1','0','0','0','1'},
	{'1','0','0','0','1','1'},{'1','1','1','1','1','0'},
	{'0','1','1','1','1','1'},{'0','0','0','1','1','0'},
	{'0','1','1','0','0','0'},{'0','1','0','1','1','0'},
	{'0','1','1','0','1','0'},{'1','0','1','1','1','0'},
	{'0','1','1','1','0','1'},{'1','0','0','1','0','0'},
	{'0','0','1','0','0','1'},{'0','0','1','0','1','1'},
	{'1','1','0','1','0','0'},{'1','0','1','1','0','0'},
	{'0','0','1','1','0','1'},{'0','1','1','0','1','1'},
	{'1','1','0','1','1','0'},{'0','1','0','0','1','1'},
	{'1','1','0','0','1','0'},{'1','1','0','0','1','1'},
	{'0','0','1','1','0','0'},{'1','0','1','0','1','0'},
	{'0','1','0','1','0','1'} };
	//存储爻阴阳
	char buf[6] = { 0 };
	//存储卦ID
	int gua_id = 0;
	//存储变爻数量
	int yao = 0;
	//保存 卦id 和 爻阴阳的信息
	vector<int> gua_info;


	//获取每一爻的信息保存
	for (int i = 0; i < 6; i++)
	{
		int n = this->get_one_yao() / 4;

		if (n == 6)
		{
			memset(&buf[i], '6', 1);
			yao_yy[i] = '0';
			yao_yy[i + 6] = '1';
		}
		else if (n == 7)
		{
			memset(&buf[i], '7', 1);
			yao_yy[i] = '1';
			yao_yy[i + 6] = '1';
		}
		else if (n == 8)
		{
			memset(&buf[i], '8', 1);
			yao_yy[i] = '0';
			yao_yy[i + 6] = '0';
		}
		else if (n == 9)
		{
			memset(&buf[i], '9', 1);
			yao_yy[i] = '1';
			yao_yy[i + 6] = '0';
		}
	}

	//其中偶数代表阴，奇数代表阳，两极为太，两中为少，即：6为太阴，8为少阴，7为少阳，9为太阳。

	//依次和64卦比较,得到本卦ID
	for (int i = 0; i < 64; i++)
	{
		//和每一个卦比较
		for (int j = 0; j < 6; j++)
		{
			//把6、7、8、9、转换成0、1，变卦中6、8为阴爻，变卦中7、9为阳爻
			char tmp_buf = '0';
			if (buf[j] == '7' || buf[j] == '9')
			{
				tmp_buf = '1';
			}
			//比较一爻
			if (tmp_buf == buf11[i][j])
			{
				//5爻都对，取得卦id
				if (j == 5)
				{
					gua_id = i + 1;
					gua_info.push_back(gua_id);
				}
			}
			else//某个爻比较失败，比较下一卦
			{
				break;
			}

		}
	}



	//依次和64卦比较,得到变卦ID
	for (int i = 0; i < 64; i++)
	{
		//和每一个卦比较
		for (int j = 0; j < 6; j++)
		{
			char tmp_buf = '0';
			if (buf[j] == '7' || buf[j] == '6')
			{
				tmp_buf = '1';
			}
			//比较一爻
			if (tmp_buf == buf11[i][j])
			{
				//5爻都对，取得卦id
				if (j == 5)
				{
					gua_id = i + 1;
					gua_info.push_back(gua_id);
				}
			}
			else//某个爻比较失败，比较下一卦
			{
				break;
			}

		}
	}

	//变卦数量
	for (int i = 0; i < 6; i++)
	{
		if (buf[i] == '6' || buf[i] == '9')
		{
			yao++;
		}
	}



	gua_info.push_back(yao);
	return gua_info;
}

int  protocol_zhouyi::get_one_yao()
{
	//数50用49 
	int n = 49;
	for (int i = 0; i < 3; i++)
	{
		//分而为二
		int nt = (rand() % (n - 1) + 1);//随机数为1-48
		int nd = n - nt;

		//挂一象三
		if (rand() % 2 == 0)
		{
			nt--;
		}
		else
		{
			nd--;
		}

		//揲之以四 以象四时
		//除以4产生的余数不要，如果余数为0减4
		int tmpnt = nt;
		int tmpnd = nd;
		nt = nt / 4 * 4;
		nd = nd / 4 * 4;
		if (nt == tmpnt && nt != 0)
		{
			nt -= 4;
		}
		if (nd == tmpnd && nd != 0)
		{
			nd -= 4;
		}

		n = nt + nd;
	}
	return n;
}

prorocol_log_in_ui::prorocol_log_in_ui(one_message& my_one_message)
{
	this->my_error = debug_and_log_lr::get_error_lr();
	//获取到客服端发送过来的公钥
	send_pub_key pub_key;
	pub_key.ParseFromString(my_one_message.m_data);

	//公钥保存到，当前套接字
	my_rsa* rsa =new my_rsa;
	if ( rsa->set_pub_key(pub_key.pub_key(), pub_key.pub_key_len()) == true)
	{
		my_one_message.m_ptr->is_close = false;
		my_one_message.m_ptr->cfd_temp_key = rsa;
		//my_error->show_bin((const unsigned char*)pub_key.pub_key().data(), pub_key.pub_key_len());
		
	}
	else
	{
		delete rsa;
		rsa == NULL;
		my_one_message.m_ptr->is_close  = true;
		my_error->error_lr("读取客服端发送公钥失败！");
	}
	
	//随机生成秘钥数据
	string key = my_aes::rand_key();
	//创建aes秘钥对象，保存到当前套接字
	my_one_message.m_ptr->aes = new my_aes;
	//添加秘钥到秘钥对象
	my_one_message.m_ptr->aes->add_base64_key(key);

	

	//组织需要回发的数据
	aes_key aes;
	//把aes秘钥，使用公钥加密
	aes.set_key(my_one_message.m_ptr->cfd_temp_key->rsa_pubkey_encrypt(key));
	aes.set_is_pub_key(!my_one_message.m_ptr->is_close);
	//序列化
	string buf;
	aes.SerializeToString(&buf);
	//填充两个字节
	buf.append("aa");
	buf[buf.size() - 2] = 0B00000100;
	buf[buf.size() - 1] = '\0';


	//发送数据
	send_data sd(buf, my_one_message.m_ptr);
	send_message s(sd);
}

protocol_delete_rsa::protocol_delete_rsa(one_message& my_one_message)
{
	this->my_error = debug_and_log_lr::get_error_lr();

	//客服端，已经关闭登陆页面，允许当前套接字不活跃时关闭
	my_one_message.m_ptr->is_close = true;

	//释放秘钥资源
	if (my_one_message.m_ptr->cfd_temp_key !=NULL)
	{
		delete my_one_message.m_ptr->cfd_temp_key;
		my_one_message.m_ptr->cfd_temp_key = NULL;

		my_one_message.m_ptr->vft = string();
	}
	

}

protocol_verification::protocol_verification(one_message& my_one_message)
{
	this->my_error = debug_and_log_lr::get_error_lr();
	//aes 解密，获取到手机号码然后发送
	if (my_one_message.m_ptr->aes!=NULL)
	{
		//发送短信对象
		sms s;

		//随机生成验证码
		string n = to_string(rand() % 8999 + 1000);

		//保存验证码到当前套接字
		my_one_message.m_ptr->vft = n;

		//回发客服端，验证码的发送情况数据
		ret_registered_phone v;
		v.set_verification_state(s.send_sms((char*)my_one_message.m_ptr->aes->decrypy_aes(my_one_message.m_data).data(),(char*)n.data()));
		
		//验证码发送成功，保存验证码到当前套接字
		if (v.verification_state() == 0)
		{
			my_one_message.m_ptr->vft = n;
		}

		//序列化
		string buf;
		v.SerializeToString(&buf);
		//填充两个字节
		buf.append("aa");
		buf[buf.size() - 2] = 0B00000101;
		buf[buf.size() - 1] = '\0';


		//发送数据
		send_data sd(buf, my_one_message.m_ptr);
		send_message sm(sd);
		

	}
	else
	{
		my_error->run_print("客服端太长时间未活跃，秘钥资源以被释放！无法获取验证码！");
	}
	
}

protocol_verification_check::protocol_verification_check(one_message& my_one_message)
{

	bool ret_bool =false;
	this->my_error = debug_and_log_lr::get_error_lr();

	//判断客服端发送过来的验证码是否正确
	if (my_one_message.m_ptr->vft == my_one_message.m_data)
	{
		ret_bool = true;
	}
	else
	{
		ret_bool = false;
	}

	//组织回发数据
	is_ok isok;
	isok.set_is_pub_key(ret_bool);

	//序列化
	string buf;
	isok.SerializeToString(&buf);

	//填充两个字节
	buf.append("aa");
	buf[buf.size() - 2] = 0B000001000;
	buf[buf.size() - 1] = '\0';


	//发送数据
	send_data sd(buf, my_one_message.m_ptr);
	send_message sm(sd);

}
