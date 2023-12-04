#include <iostream>
#include "user_interface.h"
#include <conio.h>
#include "kernel.h"


int main()
{
	std::cout << "欢迎使用霍夫曼编码器" << std::endl;
	while (true)
	{
		show_hint();
		char choice=_getch();
		switch (choice)
		{
		case '1':
			process1();
			break;
		case '2':
			process2();
			break;
		case '3':
			goto END;
		default:
			break;
		}
	}
END:
	return 0;
}