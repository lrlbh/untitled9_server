#include "protocol_laozi.h"
#include"debug_and_log_lr.h"
#include"gua_info.pb.h"
#include"my_sql.h"
#include<string>
#include"send_message.h"

using namespace std;
protocol_laozi::protocol_laozi(std::map<int, std::string>& data)
{
	//要发送的数据
	map<int, string> send_data;
	this->my_error = debug_and_log_lr::get_error_lr();

	//序列化
	dao_de_jing laozi;

	my_sql mysql;
	string sql = string("select ddj_message from ddj where ddj_id= ").append(to_string(rand() % 80 + 1));
	MYSQL_RES* laozi_res =mysql.sql_exec(sql);
	
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
			// cout << row[0] <<endl;
			// show_bin(row[0],10);
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
	 //填充两个字节的协议
	 buf.append("aa");
	 buf[buf.size() - 2] = 0B00000010;
	 //此时buf.size() 不会更新，所以需要+1
	 buf[buf.size() - 1] = '\0';

	 send_data.insert(make_pair((*data.lower_bound(0)).first, buf));
	 send_message s(send_data);
}
