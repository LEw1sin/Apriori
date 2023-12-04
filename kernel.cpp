#pragma warning(disable:4996)
#include "kernel.h"
#include <iostream>
#include <sstream>
#include <string>
#include "huffmanTree.h"
#include <conio.h>
#include <string.h>
void process1()
{
	std::cout << "输入文件名称：" << std::endl;
	std::string path;
	std::cin >> path;
	std::ifstream in(path);
	if (!in.is_open()) { //打开失败
		std::cout << "文件打开失败" << std::endl;
		return;
	}
	std::ostringstream tmp;
	tmp << in.rdbuf();
	std::string str = tmp.str();

	//std::cout << str << std::endl;

	int freq_table[128] = { 0 };
	stat(str, freq_table);
	HuffmanTree tree(freq_table);
	tree.build();
	tree.code();
	tree.print(std::cout);

	std::string result;
	compress(tree, str, result);
	std::cout << "压缩率为：" << 1 - (float)result.size() / (float)str.size() << std::endl;
	std::cout << "请问是否保存文件(Y/N):";
	char choice = _getch();
	if (choice != 'Y' && choice != 'y') {
		return;
	}
	std::cout << "\n输入编码后的文件名(.huff)：" << std::endl;
	std::string name;
	std::cin >> name;
	std::ofstream outfile;
	outfile.open(name);
	if (!outfile) { //打开失败
		std::cout << "文件打开失败" << std::endl;
		return;
	}
	outfile << result;
	outfile.close();
	outfile.open(name + ".help");
	if (!outfile) { //打开失败
		std::cout << "文件打开失败" << std::endl;
		return;
	}
	tree.print(outfile);
	std::cout << "*已保存*" << std::endl;

}

void process2()
{
	std::cout << "输入文件名称：" << std::endl;
	std::string path;
	char ch[128];
	int freq;
	int freq_table[128] = { 0 };

	char str[128];
	do {
		std::cin >> path;
	} while (path == "");
	std::cout << path << std::endl;
	FILE* fp = fopen((path + ".help").c_str(), "r+");;
	if (fp == NULL) { //打开失败
		std::cout << "文件打开失败" << std::endl;
		return;
	}
	else {
		while (!feof(fp)) {
			fscanf(fp, "%s\t%d\t%s\t", ch, &freq, str);
			//printf("%s\t%d\t%s\t\n", ch, freq, str);
			std::string ss(ch);
			if (ss == "\\n") {
				//std::cout << "hahahahahahahahaha";
				freq_table['\n'] = freq;
			}
			else if (ss == "space") {
				//std::cout << "hehehehehehehehhehehe";
				freq_table[' '] = freq;
			}
			else {
				freq_table[ch[0]] = freq;
			}
		}
	}
	std::ifstream in(path, std::ios::binary);
	if (!in.is_open()) { //打开失败
		std::cout << "文件打开失败" << std::endl;
		return;
	}
	std::ostringstream tmp;
	tmp << in.rdbuf();
	std::string bin_res = tmp.str();
	std::cout << "解码中..." << std::endl;
	HuffmanTree tree(freq_table);
	tree.build();
	tree.code();
	std::string seq = "";
	toseq(bin_res, seq);
	std::string result;
	tree.decode(seq, result);
	std::cout << "解码结果为：" << std::endl;
	std::cout << result << std::endl;
	std::cout << "是否另存为文本文件？（Y/N)" << std::endl;
	char choice = _getch();
	if (choice != 'y' && choice != 'Y') return;
	std::cout << "请输入保存的文件名：" << std::endl;
	std::string name;
	std::cin >> name;
	std::ofstream outfile;
	outfile.open(name);
	if (!outfile) { //打开失败
		std::cout << "文件打开失败" << std::endl;
		return;
	}
	outfile << result;
	outfile.close();
	std::cout << "已保存" << std::endl;
}

void compress(HuffmanTree& tree, std::string& text, std::string& result) {
	std::string huffmancodeseq;
	getHuffmanCodeSeq(tree, text, huffmancodeseq);
	//std::cout << huffmancodeseq << std::endl;//debug
	tobin(huffmancodeseq, result);
}

void getHuffmanCodeSeq(HuffmanTree& tree, std::string& text, std::string& seq) {
	for (char c : text) {
		seq += tree.getHuffmanCode(c);
	}
}

void tobin(const std::string& seq, std::string& result) {
	char unit;
	unsigned int i = 0;
	while (i < seq.size()) {
		memset(&unit, 0, sizeof(unit));
		for (int n = 0; n < 8 && i < seq.size(); n++) {
			if (seq[i] == '1') unit |= (1 << (7 - n));
			i++;
		}
		result += unit;
	}
}

void toseq(const std::string& bin, std::string& result) {
	int i = 0;
	while (i < bin.size()) {
		for (int n = 0; n < 8; n++) {
			char f = bin[i] & (1 << 7 - n);
			result += (f == 0 ? '0' : '1');
		}
		i++;
	}
}

