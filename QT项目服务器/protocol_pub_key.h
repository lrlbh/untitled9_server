#include<iostream>
#include<map>

class debug_and_log_lr;
class protocol_pub_key
{
public:
	protocol_pub_key(std::map<int, std::string>& data);
private:
	debug_and_log_lr* my_error;
};