#include"send_message.h"
#include <unistd.h>
#include<map>
using namespace std;

send_message::send_message(std::map<int,std::string>& data)
{
	std::map <int, std::string>::iterator it = data.lower_bound(0);

	cout<< "发送了："<<write((*it).first, (*it).second.data(), (*it).second.size()) <<endl;;
}
