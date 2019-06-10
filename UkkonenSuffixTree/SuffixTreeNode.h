#pragma once

#include "UkkonenExport.h"

class SuffixTreeNode
{
public:
	int start;
	int* end;

	SuffixTreeNode* children[MAX_CHAR] = { 0 };

	SuffixTreeNode* suffixLink;

	SuffixTreeNode(int start, int* end, SuffixTreeNode* link);
	~SuffixTreeNode();

	int CalculateLength();
private:

};