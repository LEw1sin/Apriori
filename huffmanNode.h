#pragma once
#include <string>
class HuffmanNode {
private:
	HuffmanNode* left;
	HuffmanNode* right;
	char value;
	int freq;
	bool leaf;
	std::string code;
public:
	HuffmanNode(char v,int f):value(v),freq(f),left(nullptr),right(nullptr),leaf(true) {};
	HuffmanNode(HuffmanNode* l, HuffmanNode* r) :value(0),left(l), right(r),leaf(false) {
		freq = l->getfreq() + r->getfreq();
	};
	int getfreq()const { return freq; };
	bool isleaf() const { return leaf; };
	void setCode(std::string& code) {
		this->code = code;
	}
	char getValue() const { return value; };
	HuffmanNode* getleft() const { return left; }
	HuffmanNode* getright() const { return right; }
	std::string getCode() const { return code; }
};

