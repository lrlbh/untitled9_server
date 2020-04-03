#pragma once
#include <iostream>
#include<map>
#include<queue>


class protocol_distribute
{
public:
	protocol_distribute();

	void ststart(std::map<int, std::queue<std::string>> &data);

};