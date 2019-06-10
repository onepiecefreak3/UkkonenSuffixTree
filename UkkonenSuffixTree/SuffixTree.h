#pragma once

#include "SuffixTreeNode.h"

class SuffixTree
{
public:
	SuffixTreeNode* root = nullptr;

	SuffixTree();
	~SuffixTree();

	void Build(char* input, int position, int size);

private:
	int* rootEnd;

	SuffixTreeNode* lastNewNode = nullptr;
	SuffixTreeNode* activeNode = nullptr;

	int activeEdge = -1;
	int activeLength = 0;

	int remainingSuffixCount = 0;
	int leafEnd = -1;
	int* splitEnd = nullptr;

	void ExtendSuffixTree(char* input, int position);
	int WalkDown(SuffixTreeNode* node);
};