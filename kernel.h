#pragma once
#include "stat.h"
#include "huffmanTree.h"

void process1();
void process2();
void compress(HuffmanTree& tree, std::string& text, std::string& bin);
void getHuffmanCodeSeq(HuffmanTree& tree, std::string& text,std::string& seq);
void tobin(const std::string& seq, std::string& result);
void toseq(const std::string& bin, std::string& result);