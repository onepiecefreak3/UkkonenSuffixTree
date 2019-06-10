#pragma once

#include "SuffixTree.cpp"
#include "SuffixTreeNode.cpp"

#define DllExport   __declspec( dllexport )
#define MAX_CHAR 256

// Function headers
// Tree related
SuffixTree* CreateSuffixTree();
void DestroySuffixTree(SuffixTree* tree);
void Build(SuffixTree* tree, char* input, int position, int size);

// Find longest match
void FindLongestMatch(SuffixTree* tree, char* input, int position, int size, int* displacement, int* length);

void Traverse(SuffixTreeNode* node, char* input, int position, int size, int* displacement, int* length);
int TraverseEdge(SuffixTreeNode* node, char* input, int position, int size, int* displacement, int* length);