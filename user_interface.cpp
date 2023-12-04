#include "user_interface.h"
#include <iostream>
void show_hint()
{
	std::cout << std::endl
		<< "*********************" << std::endl
		<< "请选择操作" << std::endl
		<< "1.压缩" << std::endl
		<< "2.解压" << std::endl
		<< "3.退出" << std::endl
		<< "*********************" << std::endl;
}