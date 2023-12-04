#include "huffmanTree.h"

HuffmanTree::HuffmanTree(int* freq_table) {
	count = 0;
	num_of_char = 0;
	root = nullptr;
	for (int i = 0; i < 128; i++) {
		if (freq_table[i] != 0) {
			count++;
			num_of_char += freq_table[i];
		}
	}
	std::cout << num_of_char << std::endl;
	huffman_array = new HuffmanNode * [count];
	int n = 0;
	for (int i = 0; i < 128; i++) {
		if (freq_table[i] != 0) {
			huffman_array[n++] = new HuffmanNode((char)i, freq_table[i]);
		}
	}
}

HuffmanTree::~HuffmanTree() {
	clear();
	delete huffman_array;
	//todo
}

void HuffmanTree::build() {
	heap<HuffmanNode*, Comparation> minheap(huffman_array, count, 256);
	while (minheap.size() > 1) {
		//printf("minheapsize:%d\n", minheap.size());
		HuffmanNode* temp1 = minheap.removefirst();
		HuffmanNode* temp2 = minheap.removefirst();
		//printf("%d %d\n", temp1->getfreq(), temp2->getfreq());
		minheap.insert(new HuffmanNode(temp1, temp2));
	}
	root = minheap.removefirst();
	int idx = 0;
	refresh_freq_table(root, idx);
}

void HuffmanTree::code() {
	code_helper(root);
}

void HuffmanTree::decode(std::string& huffmanseq,std::string& result) {
	int i = 0;
	int n = 0;
	while (i < huffmanseq.size()&&n<num_of_char) {
		result += decode_helper(root, huffmanseq, i);
		n++;
	}
}

void HuffmanTree::clear() {
	clear_helper(root);
}

std::string HuffmanTree::getHuffmanCode(char c) {
	for (int i = 0; i < count; i++) {
		if (huffman_array[i]->getValue() == c) return huffman_array[i]->getCode();
	}
	return "";
}

void HuffmanTree::print(std::ostream& out) {
	for (int i = 0; i < count; i++) {
		HuffmanNode* node = huffman_array[i];
		if (node->getValue() == '\n') {
			out << "\\n" << "\t" << node->getfreq() << "\t" << node->getCode() << std::endl;
		}
		else if (node->getValue() == ' ') {
			out << "space" << "\t" << node->getfreq() << "\t" << node->getCode() << std::endl;
		}
		else {
			std::string s(1,node->getValue());
			out << s << "\t" << node->getfreq() << "\t" << node->getCode() << std::endl;
		}
	}
}

void HuffmanTree::refresh_freq_table(HuffmanNode* root, int& i) {
	if (root == nullptr) return;
	if (!root->isleaf()) {
		refresh_freq_table(root->getleft(), i);
		refresh_freq_table(root->getright(), i);
	}
	else {
		huffman_array[i++] = root;
	}
}

void HuffmanTree::clear_helper(HuffmanNode* node) {
	if (node == nullptr) return;
	if (!node->isleaf()) {
		clear_helper(node->getleft());
		clear_helper(node->getright());
	}
	delete node;
}
void HuffmanTree::code_helper(HuffmanNode* root) {
	static int index = 1;
	if (root != nullptr) {
		if (root->getleft() == NULL && root->getright() == NULL && index == 1) {
			currentPath += "0";
		}
		index = 0;

		if (!root->isleaf()) {
			currentPath += '0';
			code_helper(root->getleft());
			currentPath.pop_back();
			currentPath += '1';
			code_helper(root->getright());
			currentPath.pop_back();
		}
		else {
			root->setCode(currentPath);
			return;
		}
	}
}

char HuffmanTree::decode_helper(HuffmanNode* node, std::string& huffmanseq, int& i) {
	if (node->isleaf())  return node->getValue();
	if (i + 1 > huffmanseq.size()) return ' ';
	return decode_helper(huffmanseq[i++] == '0' ? node->getleft() : node->getright(), huffmanseq, i);
}
