#pragma once

#define DllExport   __declspec( dllexport )
#define MAX_CHAR 256

// Tree related
extern "C" DllExport SuffixTree* CreateSuffixTree();
extern "C" DllExport void DestroySuffixTree(SuffixTree* tree);
extern "C" DllExport void Build(SuffixTree* tree, char* input, int position, int size);

// Find longest match
extern "C" DllExport void FindLongestMatch(SuffixTree* tree, char* input, int position, int size, int* displacement, int* length);

void Traverse(SuffixTreeNode* node, char* input, int position, int size, int* displacement, int* length);
int TraverseEdge(SuffixTreeNode* node, char* input, int position, int size, int* displacement, int* length);