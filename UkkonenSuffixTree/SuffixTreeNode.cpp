#include "SuffixTreeNode.h"

SuffixTreeNode::SuffixTreeNode(int start, int* end, SuffixTreeNode* link)
{
	/*For root node, suffixLink will be set to nullptr
	For internal nodes, suffixLink will be set to root
	by default in  current extension and may change in
	next extension*/
	suffixLink = link;
	start = start;
	end = end;
}

SuffixTreeNode::~SuffixTreeNode()
{
	suffixLink = nullptr;
	end = nullptr;

	for (int i = 0; i < MAX_CHAR; i++)
		if (children[i] != nullptr)
			delete children[i];

	delete[] children;
}

int SuffixTreeNode::CalculateLength()
{
	return *(end)-(start)+1;
}