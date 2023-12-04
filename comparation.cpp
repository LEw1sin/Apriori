#include "comparation.h"

bool Comparation::prior(HuffmanNode* a, HuffmanNode* b) {
	return a->getfreq() < b->getfreq();
}