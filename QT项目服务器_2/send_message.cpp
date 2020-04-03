#include"send_message.h"
#include <unistd.h>
#include<map>
#include"debug_and_log_lr.h"
using namespace std;


send_message::send_message(send_data &data)
{
	debug_and_log_lr* my_error = debug_and_log_lr::get_error_lr();

	
	my_error->run_print("发送了", 
		to_string(write(data.m_ptr->fd, data.m_data.data(), data.m_data.size())).data(),
		to_string(data.m_ptr->fd).data());

}

send_data::send_data(std::string& data, ptr* ptr):m_data(data),m_ptr(ptr)
{
}

