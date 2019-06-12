#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "Logging.cpp"
#include "UkkonenExport.h"
#include "SuffixTree.h"
#include "SuffixTreeNode.h"

void Traverse(SuffixTreeNode* node, char* input, int position, int size, int* displacement, int* length);
int TraverseEdge(SuffixTreeNode* node, char* input, int position, int size, int* displacement, int* length);

extern "C" DllExport SuffixTree* CreateSuffixTree() {
	SuffixTree* tree = new SuffixTree();
	LogPointer(std::string("Tree pointer: "), tree);
	return tree;
}

extern "C" DllExport void DestroySuffixTree(SuffixTree* tree) {
	delete tree;
}

extern "C" DllExport void Build(SuffixTree* tree, char* input, int position, int size) {
	tree->Build(input, position, size);
}

extern "C" DllExport void FindLongestMatch(SuffixTree* tree, char* input, int position, int size, int* displacement, int* length) {
	Traverse(tree->root, input, position, size, displacement, length);
}

//int main(int argc, char* argv[]) {
//	SuffixTree* tree = CreateSuffixTree();
//	Build(tree, new char[7]{ 0, 0,0,0,1,1,1 }, 0, 7);
//
//	int* disp = new int(0);
//	int* len = new int(0);
//	//FindLongestMatch(tree, new char[7]{ 0, 0,0,0,1,1,1 }, 1, 7, disp, len);
//	FindLongestMatch(tree, new char[7]{ 0, 0,0,0,1,1,1 }, 5, 7, disp, len);
//}

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

		//Get the character index to search 
		position += node->CalculateLength();
	}

	//If there is an edge from node n going out 
	//with current character str[idx], traverse that edge 
	if (node->children[input[position] & 0xFF] != nullptr)
		if (node->children[input[position] & 0xFF]->start < position)
			Traverse(node->children[input[position] & 0xFF], input, position, size, displacement, length);
		else
			return;  // no more children
}

int TraverseEdge(SuffixTreeNode * node, char* input, int position, int size, int* displacement, int* length)
{
	*displacement = position - node->start;

	//Traverse the edge with character by character matching 
	for (int k = node->start; k <= *node->end && position < size; k++, position++)
	{
		if (input[k] != input[position])
			return -1;  // stopped matching

		(*length)++;
	}

	return 0;  // matched whole node
}