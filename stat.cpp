#include "stat.h"
#include <iostream>

void stat(std::string& text,int *freq_table) {
	for(char c:text){
		freq_table[c]++;
	}
}