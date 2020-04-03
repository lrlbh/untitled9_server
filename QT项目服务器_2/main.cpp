#include"server.h"
#include"tool.h"
int main()
{
	daemob d(true,"./");

	server s;
	s.wait_message();
}