#include"protocol_zhoyi.h"
#include"my_sql.h"
#include<string.h>
#include<mysql/mysql.h>
#include"debug_and_log_lr.h"
#include"send_message.h"
#include"gua_info.pb.h"
using namespace std;


protocol_zhouyi::protocol_zhouyi(std::map<int, std::string>& data)
{
	//要发送的数据
	map<int, string> send_data;
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
			if (yao_yy[i]=='1')
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
			if (yao_yy[i+6]=='1')
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
	//填充两个字节的协议
	buf.append("aa");
	buf[buf.size() - 2] = 0B00000001;
	//此时buf.size() 不会更新，所以需要+1
	buf[buf.size() - 1] = '\0';

	send_data.insert(make_pair((*data.lower_bound(0)).first,buf));
	send_message s(send_data);
}





int protocol_zhouyi::get_one_yao()
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

vector<int> protocol_zhouyi::get_one_gua(char *yao_yy)
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
			yao_yy[i+6] = '1';
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
