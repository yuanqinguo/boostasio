#include "Server.h"
#include <iostream>

int main(int argc, char**argv)
{
	try
	{
		Server ser("192.168.199.146", "12345", 32);
		ser.OnRun();

		getchar();
	}
	catch (std::exception& e)
	{
		std::cout<<"Server Start Exception"<<std::endl;
		return -1;
	}
	return 0;
}