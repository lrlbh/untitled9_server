#include<iostream>
#include<map>

class debug_and_log_lr;
class protocol_laozi
{
public:
	protocol_laozi(std::map<int, std::string>& data);
private:
	debug_and_log_lr* my_error;
};