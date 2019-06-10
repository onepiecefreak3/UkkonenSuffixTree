#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "UkkonenExport.h"
#include "SuffixTree.h"
#include "SuffixTreeNode.h"

DllExport SuffixTree* CreateSuffixTree() {
	return new SuffixTree();
}

DllExport void DestroySuffixTree(SuffixTree* tree) {
	delete tree;
}

DllExport void Build(SuffixTree* tree, char* input, int position, int size) {
	tree->Build(input, position, size);
}

DllExport void FindLongestMatch(SuffixTree* tree, char* input, int position, int size, int* displacement, int* length) {
	Traverse(tree->root, input, position, size, displacement, length);
}

void Traverse(SuffixTreeNode* node, char* input, int position, int size, int* displacement, int* length)
{
	if (node == nullptr)
		return; // no match possible

	//If node n is not root node, then traverse edge 
	//from node n's parent to node n. 
	if (node->start != -1)
	{
		int res = TraverseEdge(node, input, position, size, displacement, length);
		if (res != 0)
			return;  // matching stopped (res = -1) 
	}

	//Get the character index to search 
	position += node->CalculateLength();

	//If there is an edge from node n going out 
	//with current character str[idx], traverse that edge 
	if (node->children[input[position] & 0xFF] != nullptr)
		Traverse(node->children[input[position] & 0xFF], input, position, size, displacement, length);
	else
		return;  // no more children
}

int TraverseEdge(SuffixTreeNode * node, char* input, int position, int size, int* displacement, int* length)
{
	*displacement = node->start;

	//Traverse the edge with character by character matching 
	for (int k = node->start; k <= *node->end && position < size; k++, position++)
	{
		if (input[k] != input[position])
			return -1;  // stopped matching

		(*length)++;
	}

	return 0;  // matched whole node
}