#pragma once

#include<map>
#include<queue>
#include<iostream>
#include<vector>

class debug_and_log_lr;

class protocol_zhouyi
{
public:
	protocol_zhouyi(std::map<int, std::string>& data);

private:
	int get_one_yao();
	std::vector<int> get_one_gua(char* yao_yy);
	debug_and_log_lr* my_error;
};